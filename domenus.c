#ifdef IRAF_SPP
#define	import_libc
#define	import_stdio
#define	import_spp
#include <iraf.h>
#endif /* IRAF_SPP */

#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"
#include "mcodes.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "protos.h"

/* tjt - here is a use note that is dangerous and important to know if you want
 * to either test the software, or to do dangerous things.
 * type P to enable parameter editing
 * type PK to enable biparameter editing (namely clock run/stop state).
 */

#define	XINTCHAR	'\003'

#define	DOER_OK		0
#define	DOER_REPAINT	1
#define	DOER_ERROR	(-1)

/* Prototypes 6-21-2019 */
/* from cproto: domenus.c */
/* XXX - this file is not yet fully ANSI */
int do_menus(int period, int offset);

/* XXX - dead wood warning.
 * now with the migration to libcurses, the first two arguments (tty_fd and tty)
 * are now completely useless and could be eradicated.
 */

static int doline(int tty_fd, int tty, ITEM *ip, int itime, int nline, BOOL repaint, BOOL refresh);

int nmline(void);
static int dostline(int tty_fd, int tty, int nlines, int top, int bot, int total, int autom);
int statusline(int tty_fd, int tty, int nlines, char *string);
int ring(void);
static int domgoto(int tty_fd, int tty, int nlines, int eswitch);
static int domwritep(int tty_fd, int tty, int nlines);
static int domeditp(int tty_fd, int tty, int nlines);
static int domeditb(int tty_fd, int tty, int nlines);
int menugoto(char *id);
static int domreturn(int tty_fd, int tty, int nlines);
static int domenter(int tty_fd, int tty, int nlines, int ncols, int iswitch);
int dopcur(int tty_fd, int tty);
void updateitem(void);
static int eunseen ( void );

static int domkey(int tty_fd, int tty, int nlines, int mlines, int top, int bot, int ncols, int period, int offset, int oldstatus);
static int mkey(int autom, int period, int offset);


/* The init and free calls used to be in init.c
 */

/* in menusm/menus.c */
MENUS	*menus ( void );

#ifdef notdef
typedef struct  {
    PFI ntimes;     /* number of iterations (NULL => 1) */
    int mtimes;     /* cached result of above       */
    char    *text;      /* the text string  */
    PFI gfunc;      /* go     function  */
    PFI ofunc;      /* output function  */
    PFI ifunc;      /* input  function  */
    PFI tfunc;      /* toggle function  */
    int text_start; /* text start column    */
    int text_end;   /* text end   column    */
    int func_start; /* func start column    */
    int func_end;   /* func end   column    */
} ITEM;

typedef struct  {
    char    *id;        /* menu id string   */
    int mlines;     /* sum of above mtimes  */
    int nitems;
    ITEM    *item[MAX_ITEMS];
} MENU;

typedef struct  {
    int nmenus;
    MENU    *menu[MAX_MENUS];
} MENUS;
#endif

extern int oven;

/* Hack added by tjt 1-16-2025
 */
static void
hack_menus ( MENUS *msp )
{
		MENU *mp;
		ITEM *ip;
		char *new;
		char *ov;
		char host[128];
		int i;
		char *p;

		/* I see:
		30 menus!
		aa
		Oven Main Menu
		*/

		// printf ( "%d menus!\n", msp->nmenus );
		// for ( i=0; i<msp->nmenus; i++ ) {
		// 	mp = msp->menu[i];
		// 	printf ( "%s\n", mp->id );
		// }
		mp = msp->menu[0];
		// printf ( "%s\n", mp->id );
		if ( strcmp ( mp->id, "aa" ) != 0 )
			return;

		/* The first item should be the text for the title
		 */
		ip = mp->item[0];
		if ( ! ip->text )
			return;
		if ( strncmp ( ip->text, "Oven", 4 ) != 0 )
			return;
		// printf ( "%s\n", ip->text );

		if ( oven == 0 ) {
			ov = "Main oven (oven0v0)";
		} else if ( oven == 1 ) {
			ov = "Meter cube (oven1v0)";
		} else
			ov = "unknown oven !! ??";

		if ( gethostname ( host, sizeof(host) ) != 0 )
			strcpy ( host, "anonymous" );

		for ( p=host; *p; p++ ) {
			if ( *p == '.' ) {
				*p = '\0';
				break;
			}
		}
			
		new = malloc ( 128 );
		// ip->text = "The quick brown fox jumped over the dog";

		sprintf ( new, "Oven Main Menu -- %s (running on %s)", ov, host );
		ip->text = new;
		ip->text_end = strlen(ip->text) - 1;

		// printf ( "OK\n" );
}

int
init_menus ( void )
{
	MENUS	*msp;
	long	status;

	msp = menus();

	/* move this to a long to satisfy the 64 bit compiler */
	status = (long) msp;

	switch (status) {
	case 0:
	case 1:
	case 2:
	    return (status+40);
	    break;
	default:
	    status = 0;
	    break;
	}

	/* tjt */
	hack_menus ( msp );

	globalp->msp = msp;
	return (status);
}

void
free_menus ( void )
{
	free ((char *)globalp->msp);
	globalp->msp = (MENUS *)0;
}

/* The show starts here */
int
do_menus ( int period, int offset)
{
	int	tty_fd;
	int	tty;
	MENU	*mp;
	ITEM	*ip;
	int	iitems, itime;
	int	mline, top_mline, bot_mline;	/* menu   line numbers	*/
	int	nline, top_nline, bot_nline;	/* screen line numbers	*/
	int	ncols, nlines;			/* screen size		*/
	BOOL	repaint;
	BOOL	refresh;
	int	status = M_REPAINT;
	// XINT	one = 1, two = 2;
	// int	key;

	con_init ();
	// tty = 999;
	// tty_fd = fileno (stdout);

	/* loop until the user selects Exit, which will
	 * set this index negative.
	 * until then, Gmenu indexes the current menu.
	 */
	while (Gmenu >= 0) {
	    con_size ( &ncols, &nlines );

	    mp = Menus[Gmenu];

	    repaint = status & M_REPAINT;
	    refresh = status & M_REFRESH;

	    if (repaint)
		con_clear ();

	    if (repaint || !strcmp (mp->id, "er")) {

		/* cache and calculate the menu line count
		 */
		mp->mlines = 0;
		for (iitems = 0; iitems < mp->nitems; iitems++) {
		    ip = mp->item[iitems];
		    ip->mtimes = (ip->ntimes) ? (*(ip->ntimes))() : 1;
		    mp->mlines += ip->mtimes;
		}
	    }

	    /* calculate the scrolling region
	     */
	    Gmline = MAX (0, MIN ((mp->mlines-1) - (nlines-2), Gmline));
	    top_nline = mp->item[0]->mtimes;
	    top_mline = Gmline + top_nline;
	    bot_mline = MIN (Gmline + nlines-2, mp->mlines-1);
	    bot_nline = bot_mline - top_mline + top_nline;
	    nline = top_nline;
	    mline = 0;
	    Gnline = MAX (top_mline, MIN (bot_mline, Gnline));

	    /* repaint or refresh the scrolling region
	     */
	    for (iitems = 0; iitems < mp->nitems; iitems++) {
		ip = mp->item[iitems];
		for (itime = 0; itime < ip->mtimes; mline++, itime++) {
		    if (mline < top_mline)
			continue;
		    if (mline > bot_mline)
			break;
		    doline (tty_fd, tty, ip, itime, nline, repaint, refresh);
		    nline++;
		    con_flush ();
		    // FLGKEY (&key);
		}
		if (mline > bot_mline)
		    break;
	    }

	    /* now do the ID line - not part of the scrolling region,
	     * so it gets skipped above.
	     */
	    nline = 0;
	    mline = 0;
	    ip = mp->item[0];
	    for (itime = 0; itime < ip->mtimes; mline++, itime++) {
		doline (tty_fd, tty, ip, itime, nline, repaint, refresh);
		nline++;
		// con_flush ();
		// FLGKEY (&key);
	    }

	    /* now do the status line
	     */
	    dostline (tty_fd, tty, nlines, top_mline, bot_mline, mp->mlines,
	      status & M_AUTO);
	    // con_flush ();
	    // FLGKEY (&key);

	    /* now get the cursor mode key
	     */
	    // con_raw ();

	    status = domkey ( tty_fd,tty, nlines,mp->mlines,
		top_mline,bot_mline,
	        ncols, period, offset, status );

	    // con_noraw ();
	    // con_debug ( "Loop 6\n" );
	}

	/* This is just going to return and exit,
	 * so all of this is really unnecessary
	 */
	// con_noraw ();
	// con_move ( 1, nlines );
	// con_clear_line ();

	con_close ();
	return (0);
}

/* nmline - return the number of visible menu lines
 * (called from er.c)
 */
int
nmline ( void )
{
	int	ncols, nlines;			/* screen size		*/

	/* Subtract 2 (title line and status line). */
	con_size ( &ncols, &nlines );
	return (nlines-2);
}

/* doline - do a line out of the menu
 */
static int
doline ( int tty_fd, int tty, ITEM *ip, int itime, int nline, BOOL repaint, BOOL refresh)
{
	int	map_cc = 0;
	int	n;
	char	string[256];

	if ((repaint) && ip->text) {
	    con_mvstr ( ip->text_start+1, nline+1, ip->text );
	}

	if ((repaint || refresh) && ip->ofunc) {
	    string[0] = 0;
	    if (ip->ntimes)
		(*(ip->ofunc))(itime, string);
	    else
		(*(ip->ofunc))(string);

	    con_mvstr ( ip->func_start+1, nline+1, string );
	    // c_ttyctrl (tty_fd, tty, "ce", 1);
	    con_clear_toeol ();
	}
}

/* dostline - do status line: print %done info
 */
static int
dostline ( int tty_fd,  int tty,  int nlines,  int top,  int bot,  int total,  int autom )
{
	char	string[80];

	sprintf (string, "%c%c%c lines %d to %d out of %d",
	  OPdb ? (OBdb ? 'K' : 'P') : ' ',
	  eunseen() ? 'E' : ' ',
	  autom ? 'A' : ' ',
	  top+1, bot+1, total);

	statusline (tty_fd, tty, nlines, string);

	dopcur (tty_fd, tty);
}

/* statusline - write the status line
 * called from many places
 */
int
statusline ( int tty_fd,  int tty,  int nlines, char *string )
{
	int	map_cc = 0;

	con_move ( 1, nlines );
	con_clear_line ();
	con_str ( string );
}

#ifdef notyet
/* XXX - probably for oveng */
/* ring -- ring the bell
 */
int
ring ()
{
	con_bell ();
}
#endif

/* domgoto - do menu goto
 */
static int
domgoto ( int tty_fd, int tty, int nlines, int eswitch)
{
	MENU	*mp = Menus[Gmenu];
	ITEM	*ip = mp->item[Gitem];
	int	itime = Gtime;
	int	status = GOER_OK;
	// int	key;

	if (eswitch == 0 && ip->gfunc == 0) {
	    statusline (tty_fd, tty, nlines,
		"\007no where to go to");
	    return (DOER_ERROR);
	}
	if (push_context () != 0) {
	    statusline (tty_fd, tty, nlines,
		"\007could not push context");
	    return (DOER_ERROR);
	}

	con_str ( "Executing" );
	con_flush ();
	// FLGKEY (&key);

	if (eswitch)
	    status = menugoto ("er");
	else if (ip->ntimes)
	    status = (*(ip->gfunc)) (itime);
	else
	    status = (*(ip->gfunc)) ();

	if (status != GOER_OK) {
	    pop_context ();
	    switch (status) {
	    case GOER_EXECUTED:
	        statusline (tty_fd, tty, nlines, "Executed");
		break;
	    case GOER_NOMENU:
	        statusline (tty_fd, tty, nlines, "\007goto error");
		break;
	    case GOER_NOWHERE:
	        statusline (tty_fd, tty, nlines, "\007No where to go to");
		break;
	    case GOER_INCONSISTENT:
	        statusline (tty_fd, tty, nlines,"\007inconsistent goto  error");
		break;
	    case GOER_NOEDITP:
	        statusline (tty_fd, tty, nlines,
		  "\007parameter editing not enabled");
		break;
	    default:
	        statusline (tty_fd, tty, nlines, "\007unknown goto  error");
		break;
	    }
	    return (DOER_ERROR);
	}
	return (DOER_OK);
}

/* domwritep - do menu writep
 */
static int
domwritep ( int tty_fd, int tty, int nlines)
{
	int	status = GOER_OK;

	if (!Geditp) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter editing not enabled");
	    return (DOER_ERROR);
	}

	if (push_context () != 0) {
	    statusline (tty_fd, tty, nlines,
		"\007could not push context");
	    return (DOER_ERROR);
	}

	status = menugoto ("zz");

	if (status != GOER_OK) {
	    pop_context ();
	    switch (status) {
	    case GOER_EXECUTED:
	        statusline (tty_fd, tty, nlines, "Executed");
		break;
	    case GOER_NOMENU:
	        statusline (tty_fd, tty, nlines, "\007goto error");
		break;
	    case GOER_NOWHERE:
	        statusline (tty_fd, tty, nlines, "\007No where to go to");
		break;
	    case GOER_INCONSISTENT:
	        statusline (tty_fd, tty, nlines,"\007inconsistent goto  error");
		break;
	    case GOER_NOEDITP:
	        statusline (tty_fd, tty, nlines,
		  "\007parameter editing not enabled");
		break;
	    default:
	        statusline (tty_fd, tty, nlines, "\007unknown goto  error");
		break;
	    }
	    return (DOER_ERROR);
	}
	return (DOER_OK);
}

/* domeditp - do menu editp
 */
static int
domeditp (int tty_fd, int tty, int nlines)
{
	int	status = GOER_OK;

	if (globalp->readonly) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter database is readonly");
	    return (DOER_ERROR);
	}
	if (OPdb) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter database is already cached");
	    return (DOER_ERROR);
	}
	if (Geditp) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter database caching is not necessary");
	    return (DOER_ERROR);
	}
	if (cache_parameter ()) {
	    statusline (tty_fd, tty, nlines,
		"\007could not cache parameters");
	    return (DOER_ERROR);
	}
	Geditp = YES;
	return (DOER_OK);
}

/* domeditb - do menu editb
 */
static int
domeditb ( int tty_fd, int tty, int nlines)
{
	int	status = GOER_OK;


	if (!Geditp) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter editing not enabled");
	    return (DOER_ERROR);
	}
	if (globalp->readonly) {
	    statusline (tty_fd, tty, nlines,
		"\007clock parameter database is readonly");
	    return (DOER_ERROR);
	}
	if (OBdb) {
	    statusline (tty_fd, tty, nlines,
		"\007clock parameter database is already cached");
	    return (DOER_ERROR);
	}
	if (Geditb) {
	    statusline (tty_fd, tty, nlines,
		"\007clock parameter database caching is not necessary");
	    return (DOER_ERROR);
	}
	if (cache_biparameter ()) {
	    statusline (tty_fd, tty, nlines,
		"\007could not cache clock parameters");
	    return (DOER_ERROR);
	}
	Geditb = YES;
	return (DOER_OK);
}

/* menugoto - set the global menu number according to the id string
 * (can be called from errorreport.c)
 */
int
menugoto ( char * id )
{
	int	nmenu;

	for (nmenu = 0; nmenu < globalp->msp->nmenus; nmenu++)
	    if (strcmp (Menus[nmenu]->id, id) == 0) {
		Gmenu = nmenu;
		return (GOER_OK);
	    }
	return (GOER_NOMENU);
}

/* domreturn - do menu return
 */
static int
domreturn ( int tty_fd, int tty, int nlines)
{
	if (pop_context () != 0) {
	    statusline (tty_fd, tty, nlines,
		"\007no where to return to");
	    return (DOER_ERROR);
	}
	return (DOER_OK);
}

/* domenter - do menu enter
 * Called w/ iswitch = 0 when user types "e" to enter a value
 * Called w/ iswitch = 1 when mcode == IMCUR || mcode == IMCURG
 */
static int
domenter ( int tty_fd, int tty, int nlines, int ncols, int iswitch)
{
	MENU	*mp = Menus[Gmenu];
	ITEM	*ip = mp->item[Gitem];
	int	status = SCER_OK;
	int	map_cc = 0;
	int	n, nmax;
	char	string[256];
	char	clear_string[256];
	// XINT	one = 1;
	// XINT	two = 2;

	if (ip->ifunc == 0 && ip->tfunc == 0) {
	    statusline (tty_fd, tty, nlines,
		"\007cannot enter data");
	    return (DOER_ERROR);
	}

	if (!Geditp && strcmp (mp->id, "cs")) {
	    statusline (tty_fd, tty, nlines,
		"\007parameter editing not enabled");
	    return (DOER_ERROR);
	}

	/* Is there an input function, if so,
	 *  get the necessary input
	 */
	if (ip->ifunc) {

	    // c_ttyctrl (tty_fd, tty, "ce", 1);
	    con_clear_toeol ();

	    dopcur (tty_fd, tty);
	    // string[0] = 0;

	    /* XXX - in progress */
	    con_getstr ( string ); 

#ifdef notdef
	    /* XXX - This is the old logic that handles image cursor
	     * input (if iswitch == 1 )  This involves fancy
	     * interfacing with IRAF SPP.
	     */
	    if (iswitch) {
		strcpy (string, "imcur");
	    } else {
		con_noraw ();
		con_get_param ( "input", string, sizeof(string) );
		con_raw ();
	    }

	    /* This translates the image cursor to oven RTZ coordinates */
	    if (c_timcur (string, sizeof(string))) {

		// c_ttyctrl (tty_fd, tty, "ce", 1);
		con_clear_toeol ();

		dopcur (tty_fd, tty);
		con_str ( string );
		dopcur (tty_fd, tty);
	    }
#endif
	}

	if (ip->ifunc)
	    if (ip->ntimes)
		status = (*(ip->ifunc)) (Gtime, string);
	    else
		status = (*(ip->ifunc)) (string);
	else
	    if (ip->ntimes)
		status = (*(ip->tfunc)) (Gtime);
	    else
		status = (*(ip->tfunc)) ();

	if (status != SCER_OK) {
	    switch (status) {
	    case SCER_REPAINT:
	        return (DOER_REPAINT);
		break;
	    case SCER_SYNTAX:
	        statusline (tty_fd, tty, nlines, "\007Syntax error");
		break;
	    case SCER_RANGE:
	        statusline (tty_fd, tty, nlines, "\007Out of range");
		break;
	    case SCER_INCONSISTENT:
	        statusline (tty_fd, tty, nlines,"\007inconsistent enter error");
		break;
	    case SCER_NOEDITB:
	        statusline (tty_fd, tty, nlines,
		  "\007clock parameter editing not enabled");
		break;
	    default:
	        statusline (tty_fd, tty, nlines, "\007unknown enter error");
		break;
	    }
	    return (DOER_ERROR);
	}
	return (DOER_OK);
}

/* dopcur - position cursor
 */
int
dopcur ( int tty_fd, int tty )
{
	int	nline = Gnline - Gmline;
	int	ncol;
	MENU	*mp;
	ITEM	*ip;

	updateitem ();
	mp = Menus[Gmenu];
	ip = mp->item[Gitem];

	if (ip->ifunc)
	    ncol = ip->func_end+1;
	else if (ip->tfunc)
	    ncol = ip->func_end;
	else if (ip->ofunc)
	    ncol = ip->func_start;
	else if (ip->text)
	    ncol = ip->text_end+1;
	else
	    ncol = 0;

	con_move ( ncol+1, nline+1 );
}

/* updateitem -- update the item pointers (Gitem & Gtime) from Gnline
 * (the cursor line number)
 */
void
updateitem ( void )
{
	MENU	*mp = Menus[Gmenu];
	ITEM	*ip;
	int	mline = 0;

	for (Gitem = 0; Gitem < mp->nitems; Gitem = Gitem+1) {
	    ip = mp->item[Gitem];
	    for (Gtime = 0; Gtime < ip->mtimes; mline++, Gtime = Gtime+1)
		if (mline == Gnline)
		    return;
	}
}

/* domkey - do menu key
 */
static int
domkey ( int tty_fd, int tty, int nlines, int mlines, int top, int bot, int ncols, int period, int offset, int oldstatus )
{
	int	mcode;
	int	status = 0;
	int	gmline = Gmline;
	// XINT	three = 3;

	do {
	    dopcur (tty_fd, tty);

	    /* Get keystroke from user */
	    mcode = mkey (oldstatus & M_AUTO, period, offset);

	    oldstatus &= ~M_AUTO;

	    switch (mcode) {
	    case DATA:
		/* tjt added 6-30-2019 */
		db_dread_oven ();
		break;
	    case SCR_U0:
		Gmline = 0;
		break;
	    case SCR_D0:
		Gmline = mlines;
		break;
	    case SCR_U1:
		Gmline -= nlines - 3;
		break;
	    case SCR_D1:
		Gmline += nlines - 3;
		break;
	    case SCR_U2:
		Gmline -= (nlines-2)/2;
		break;
	    case SCR_D2:
		Gmline += (nlines-2)/2;
		break;
	    case CUR_U:
		if (Gnline == top)
		    Gmline -= 1;
		else
		    mcode = UNKNOWN;
		Gnline -= 1;
		break;
	    case CUR_D:
		if (Gnline == bot)
		    Gmline += 1;
		else
		    mcode = UNKNOWN;
		Gnline += 1;
		break;
	    case REPAINT:
		status |= M_REPAINT;
		break;
	    case REFRESH:
		status |= M_REFRESH;
		break;
	    case AUTO:
		status |= M_REFRESH | M_AUTO;
		break;
	    case WRITEP:
		switch (domwritep (tty_fd, tty, nlines)) {
		case DOER_OK:
		    status |= M_REPAINT;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case CACHEP:
		switch (domeditp (tty_fd, tty, nlines)) {
		case DOER_OK:
		    status |= M_REFRESH;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case CACHEB:
		switch (domeditb (tty_fd, tty, nlines)) {
		case DOER_OK:
		    status |= M_REFRESH;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case RETURN:
		switch (domreturn (tty_fd, tty, nlines)) {
		case DOER_OK:
		    status |= M_REPAINT;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case ENTERG:
	    case IMCURG:
	    case ENTER:
	    case IMCUR:
		switch (domenter (tty_fd, tty, nlines, ncols,
			mcode == IMCUR || mcode == IMCURG)) {
		case DOER_REPAINT:
		    status |= M_REPAINT;
		    break;
		case DOER_OK:
		    status |= M_REFRESH;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		if (mcode == ENTERG || mcode == IMCURG)
		    mcode = GOTO;
		else
		    break;
	    case GOERR:
	    case GOTO:
		switch (domgoto (tty_fd, tty, nlines, mcode == GOERR)) {
		case DOER_OK:
		    status |= M_REPAINT;
		    break;
		default:
		    mcode = UNKNOWN;
		    break;
		}
		break;
	    case HELP:
		// HPAGEH ();
		con_help ();
		status |= M_REPAINT;
		break;
	    default:
		statusline (tty_fd, tty, nlines,
		    "\007unrecognized key - press `?' for help");
		break;
	    }
	} while (mcode == UNKNOWN || mcode == HELP);

	/* Normalize Gmline
	 */
	if ((status & M_REPAINT) == 0) {
	    Gmline = MAX (0, MIN ((mlines-1) - (nlines-2), Gmline));
	    if (Gmline != gmline)
	        status |= M_REPAINT;
	}

	if (status & M_REPAINT || status & M_REFRESH)
	    con_raw2 ();
	    // THGKEY (&three);

	return (status);
}

/* mkey - return a menu encoded key
 * this calls con_key() (which calls getch())
 *  to get a keystroke from the user, then maps
 *  it into an "action code" or whatever you want to call it.
 */
static int
mkey ( int autom,  int period,  int offset)
{
	int	key;
	int	mcode;

	if (autom) {
	    /* XXX - this is a mystery */
	    con_flush ();
	    // FLGKEY (&key);
	    /* Always returns 'q' these days */
	    if (!(key = automkey (period, offset)))
		return (AUTO);
	} else {
	    forever {
		// HGKEY (&key, &period, &offset);
		key = con_key ( period, offset );
		if (key == '\033')
		    ;
		else if (key == '[')
		    ;
		else if (key == 'O')
		    ;
		else
		    break;
	    }
	}

	switch (key) {
	case 'd':
	    mcode = SCR_D2;
	    break;
	case 'u':
	    mcode = SCR_U2;
	    break;
	case 'f':
	case ' ':
	    mcode = SCR_D1;
	    break;
	case 'b':
	    mcode = SCR_U1;
	    break;
	case 'g':
	    mcode = SCR_D0;
	    break;
	case '.':
	    mcode = SCR_U0;
	    break;
	case '?':
	    mcode = HELP;
	    break;
	case 'B':
	case 'j':
	case '\012':
	case '\015':
	case '\032':
	case XEOF:
	case XINTCHAR:
	    mcode = CUR_D;
	    break;
	case 'A':
	case 'k':
	case '\027':
	    mcode = CUR_U;
	    break;
	case 'D':
	case 'n':
	case '\031':
	    mcode = GOTO;
	    break;
	case '!':
	    mcode = GOERR;
	    break;
	case 'C':
	case 'p':
	case '\030':
	    mcode = RETURN;
	    break;
	case 'l':
	    mcode = REPAINT;
	    break;
	case 'q':
	    mcode = REFRESH;
	    break;
	case 'a':
	    mcode = AUTO;
	    break;
	/* tjt added E next to e to match documentation */
	case 'e':
	case 'E':
	    mcode = ENTER;
	    break;
	/* tjt - added this too, but it accomplishes little
	 * if ovend is already running.
	 */
	case 'r':
	    mcode = DATA;
	    break;
	case 'i':
	    mcode = IMCUR;
	    break;
	case 'm':
	    mcode = ENTERG;
	    break;
	case 'o':
	    mcode = IMCURG;
	    break;
	case 'P':
	    mcode = CACHEP;
	    break;
	case 'K':
	    mcode = CACHEB;
	    break;
	case 'W':
	    mcode = WRITEP;
	    break;
	default:
	    mcode = UNKNOWN;
	    break;
	}

	return (mcode);
}

/*
 *  The following routines moved here from errorreport.c 7-11-2019
 *  This is called from ./menusc/er.c
 */

#define	NSEEN	( globalp->db->intermediate.misc.msclock )

// Now defined in oven.h
// #define	LEN_ELOGLINE	79

/* nelogline - return the number of lines in the error log
 */
int
nelogline ( void )
{
	struct	stat st;
	int	nlines;

	nlines = stat ("errors.log", &st) ? 0 : st.st_size/LEN_ELOGLINE;
	if (NSEEN > nlines)
	    if (!globalp->readonly)
		NSEEN = 0;
	return (nlines);
}

/* eunseen - check for unseen errors
 */
static int
eunseen ( void )
{
	return ( nelogline () > NSEEN );
}

/* elogline - return the n'th line of the error log
 */
void
elogline ( int n, char *s)
{
	int	fd;
	off_t	offset = n * LEN_ELOGLINE;
	int	nlines = nelogline ();

	if ((n < nlines) && ((fd = open ("errors.log", O_RDONLY)) >= 0)) {
	    lseek (fd, offset, L_SET);
	    if (read (fd, s, LEN_ELOGLINE) == LEN_ELOGLINE) {
		s[LEN_ELOGLINE-1] = 0;
		if (n == nlines-1)
		    if (!globalp->readonly)
			NSEEN = MAX (n+1, NSEEN);
	    } else {
		*s = 0;
	    }
	    close (fd);
	}
}


/* eloggoto - go to a menu based on where we are in the error log.
 */
int
eloggoto ( char *s )
{
	char	mm[3];
	char	sa[5];
	char	*cp;
	char	*strchr();
	int	t;
	int	zone;
	DNTX	dntx;
	DSB	dsb;
	PFE	pfe;

	if ((cp = strchr (s, '<')) == 0)
	    return (GOER_NOWHERE);
	cp++;
	mm[0] = *cp++;
	mm[1] = *cp++;
	mm[2] = 0;

	if ((cp = strchr (s, '@')) == 0)
	    return (GOER_NOWHERE);
	cp++;
	sa[0] = *cp++;
	sa[1] = *cp++;
	sa[2] = *cp++;
	sa[3] = *cp++;
	sa[4] = 0;

	     if (!strcmp (mm, "zo")) t = sczone (sa, &zone);
	else if (!strcmp (mm, "he")) t = scpfe  (sa, &pfe);
	else if (!strcmp (mm, "pp")) t = scpfe  (sa, &pfe);
	else if (!strcmp (mm, "tc")) t = scdntx (sa, &dntx);
	else if (!strcmp (mm, "sb")) t = scdsb  (sa, &dsb);
	else if (!strcmp (mm, "dc")) t = scdntx (sa, &dntx);
	else if (!strcmp (mm, "ti")) t = scdntx (sa, &dntx);
	else if (!strcmp (mm, "al")) t = scpfe  (sa, &pfe);
	else if (!strcmp (mm, "ms")) t = SCER_OK;
	else return (GOER_NOWHERE);
	if (t)
	    return (GOER_NOWHERE);

	t = menugoto (mm);
	if (t)
	    return (GOER_NOWHERE);

	     if (!strcmp (mm, "zo")) Gzone = zone;
	else if (!strcmp (mm, "he")) Gpfe  = pfe;
	else if (!strcmp (mm, "pp")) Gpfe  = pfe;
	else if (!strcmp (mm, "tc")) Gdntx = dntx;
	else if (!strcmp (mm, "sb")) Gdsb  = dsb;
	else if (!strcmp (mm, "dc")) Gdntx = dntx;
	else if (!strcmp (mm, "ti")) Gdntx = dntx;
	else if (!strcmp (mm, "al")) Gpfe  = pfe;
	else if (!strcmp (mm, "ms")) ;
	else return (GOER_NOWHERE);

	return (t);
}

/* THE END */
