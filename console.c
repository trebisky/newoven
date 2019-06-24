/* glue.c
 * -- stubs for things that once were provided by SPP code
 * In particular hgkey.x
 *  tjt  6-21-2019
 */
#include "oven.h"
#include "protos.h"

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

static void
trap ( char *s, int stop )
{

	if ( stop ) {
	    refresh ();
	    getch ();
	    endwin ();
	    printf ( "TRAP: %s\n", s );
	    printf ( "TRAP: stopping execution\n" );
	    exit ( 2 );
	} else {
	    printf ( "TRAP: %s\n", s );
	}
}

#define NEW_CURSES

#ifdef NEW_CURSES
void
con_init ( void )
{
	initscr ();
	/*
        cbreak();
        noecho();
	*/
}

void
con_close ( void )
{
	endwin ();
}

void
con_size ( int *ncols, int *nlines )
{
	*ncols = COLS;
	*nlines = LINES;
}

void
con_clear ( void )
{
        clear ();
	getch ();
}

void
con_clear_line ( void )
{
        /* XXX */
}

/* Curses uses a zero indexed system.
 * arguments are x, y
 * curses reverses that to y, x
 */
void
con_move ( int col, int line )
{
	/* row, col */
        move ( line-1, col-1 );
}

void
con_str ( char *text )
{
	addstr ( text );
}

/* Combines the above */
void
con_mvstr ( int col, int line, char *text )
{
	mvaddstr ( line-1, col-1, text );
}

void
con_bell ( void )
{
	addch ( 007 );
}

/* Flush stderr and stdout */
void
con_flush ( void )
{
	fflush ( stdout );
	fflush ( stderr );
}

void
con_debug ( char *msg )
{
	addstr ( msg );
	refresh ();
}

void
con_raw ( void )
{
	raw ();
	// cbreak ();
}

void
con_noraw ( void )
{
	noraw ();
	// nocbreak ();
}

void
con_raw2 ( void )
{
	con_raw ();
}

int
con_key ( int period, int offset )
{
	return getch ();
}

/* Called to get "input" */
void
con_get_param ( char *param, char *val, int max )
{
	strcpy ( val, "0" );
}

#endif

#ifdef OLD_IRAF

/* For some crazy reason the API to the IRAF library calls
 * requires you to keep track of both tty and tty_fd and
 * pass both to the various functions.
 * I keep those both here as global variables and hide
 * all of that nonsense.
 */

static int tty;
static int tty_fd;

/* The IRAF way returns a handle we need to
 * keep track of.
 */
int
con_init ( void )
{
	/* odes = open descriptor */
	tty = c_ttyodes ("terminal");
	tty_fd = fileno (stdout);
}

void
con_close ( void )
{
	c_ttycdes (tty);
}

void
con_clear ( void )
{
	c_ttyclear ( tty_fd, tty );
}

/* oddly enough, needs no handle */
void
con_size ( int *ncols, int *nlines )
{
	c_xttysize ( ncols, nlines );
}

void
con_clear_line ( void )
{
	c_ttyclearln ( tty_fd, tty );
}

void
con_move ( int col, int line )
{
	c_ttygoto ( tty_fd, tty, col, line );
}

/* The final argument to c_ttyputline is "map_cc",
 *  whatever the heck it is -- it is/was zero in
 *  every call in this software, so we encapsulate
 *  that here.
 */
void
con_str ( char *text )
{
	c_ttyputline ( tty_fd, tty, text, 0 )
}

void
con_mvstr ( int col, int line, char *text )
{
	c_ttygoto ( tty_fd, tty, col, line );
	c_ttyputline ( tty_fd, tty, text, 0 )
}

void
con_bell ( void )
{
	int     l_tty_fd = fileno (stdout);
        int     l_tty = c_ttyodes ("terminal");
        int     key;

	c_ttyputline ( l_tty_fd, l_tty, "\007", 0 );
        FLGKEY (&key);
}

/* Flush stderr and stdout */
void
con_flush ( void )
{
        int     key;

        FLGKEY (&key);
}

void
con_debug ( char *msg )
{
	/* XXX  */
}

void
con_raw ( void )
{
	XINT    two = 2;
	THGKEY ( &two );
}

void
con_noraw ( void )
{
	XINT    one = 1;
	THGKEY ( &one );
}

void
con_raw2 ( void )
{
	con_raw ();
	// XINT    three = 2;
	// THGKEY ( &three );
}

int
con_key ( int period, int offset )
{
	int key;

	HGKEY (&key, &period, &offset);
	return key;
}

/* Calls clgstr
 * This does some kind of lookup
 * The only call is with name "input".
 * This is involved with the image cursor business.
 */
void
con_get_param ( char *param, char *val, int max )
{
	clgstr ( param, val, max );
}

/* clgstr - CL get string parameter
 */
static int
clgstr ( char *param, char *outstr, int maxch )
{
        XCHAR   x_param[256];
        XCHAR   x_outstr[256];
        XINT    x_maxch = maxch;

        c_strupk (param, x_param, strlen (param)+1);
        HCLGST (x_param, x_outstr, &x_maxch);
        c_strpak (x_outstr, outstr, maxch);
}

#endif

/* ----------------------------------------------- */
/* ----------------------------------------------- */
/* ----------------------------------------------- */

/* old glue below here */
/* HGKEY.x */

/*
include <fset.h>
include <chars.h>

# HGKEY -- Hoven get ukey routine

define	SZ_KEYSTR	80

# Why period and offset ?
# Period is the autoupdate period

procedure hgkey (key, period, offset)
int	key
int	period
int	offset

int	ch, delay, getci()
long	clktime()
long	now
long	wakeuptime
int	notraw

begin
	call flush (STDOUT)
	call flush (STDERR)
	key = 'q'

	notraw = -1
	call thgkey (notraw)
	if (notraw == YES) {
	    call thgkey (2)
	}

	now = clktime(0)
	wakeuptime = (period - mod (now, period) + offset) + now
	delay = 0
	repeat {
	    ch = getci (STDIN, ch)
	    switch (ch) {
	    case '\033', 'O', '[':
		next
	    case EOF:
		call decisleep
		delay = delay-1
		if (delay <= 0) {
		    delay = (wakeuptime - clktime(0))*10
		    if (delay <= 0)
			break
		    if (delay > 100)
			delay = 100
		}
	    default:
		key = ch
		break
	    }
	}
end


# tjt - never called with -1
# tjt - one call with 3, no idea what that would
#       do different than calling with 2.
procedure thgkey (sw)
int	sw

int	notraw
data	notraw/YES/

begin
	if (sw == -1) {
	    sw = notraw
	    return
	}

	call flush (STDOUT)
	call flush (STDERR)
#	if ((sw & 1) != 0) {
	if (iand(sw,1) != 0) {
	    call fseti (STDIN, F_IOMODE, IO_NORMAL)
	    notraw = YES
	}
	call flush (STDOUT)
	call flush (STDERR)
#	if ((sw & 2) != 0) {
	if (iand(sw, 2) != 0) {
	    call fseti (STDIN, F_IOMODE, IO_RAW+IO_NDELAY)
	    notraw = NO
	}
	call flush (STDOUT)
	call flush (STDERR)
end


# HPAGEH - Hoven page cursor mode help file

procedure hpageh ()

begin
	call thgkey(1)
	call pagefiles ("oven$cm.hlp")
	call thgkey(2)
end

# HCLGST - Hoven CL get string

procedure hclgst (param, outstr, maxch)
char	param[ARB]
char	outstr[maxch]
int	maxch

errchk	clgstr

begin
	call clgstr (param, outstr, maxch)
end

# FLGKEY -- Flush before getting the next key.

procedure flgkey (key)

int	key

begin
	# Flush any buffered text output.
	call flush (STDERR)
	call flush (STDOUT)
end
*/

/* Some kind of raw mode interface to the console.
 * hgkey calls thgkey.
 * reads a key into *key
 */
void
hgkey_ ( int *key, int *period, int *offset )
{
	trap ( "hgkey", 1 );
}

/* Fools around with terminal modes */
void
thgkey_ ( XINT *val )
{
	// trap ( "thgkey", 0 );
}

/* Page through a help file
 */
void
hpageh_ ( void )
{
	trap ( "hpageh", 1 );
}


/* XXX - important */
void
backup_ ( void )
{
	trap ( "backup", 1 );
}

/* From IRAF sys/libc */
/* As an example, on my system, I can find source
 * in two places:
 *
 *  /u1/iraf/iraf_2.16.1/sys/libc/cttygoto.c
 *  /u1/iraf/src-2.16.1/sys/libc/cttygoto.c
 *
 */

/*
   40 |  int tty = c_ttyodes ("terminal");
   54 |      c_xttysize (&ncols, &nlines);
   60 |   c_ttyclear (tty_fd, tty);
  124 |  c_ttygoto (tty_fd, tty, 1, nlines);
  125 |  c_ttyclearln (tty_fd, tty);
  126 |  c_ttycdes (tty);
  152 |      c_ttyputline (tty_fd, tty, ip->text, map_cc);
  169 |      c_ttyctrl (tty_fd, tty, "ce", 1);
  491 |      if (c_timcur (string, sizeof(string))) {
  862 |  c_strupk (param, x_param, strlen (param)+1);
  864 |  c_strpak (x_outstr, outstr, maxch);
*/

void
c_xttysize ( int *c, int* l )
{
	trap ( "c_xttysize", 0 );
}

void
c_ttyclear ( int tty_fd, int tty )
{
	trap ( "c_ttyclear", 0 );
}

void
c_ttyctrl ( int tty_fd, int tty, char *cmd, int xyz )
{
	trap ( "c_ttyctrl", 0 );
}

/* --------------------------------------------------------------------- */
/* Mysterious image cursor stuff */

int
c_timcur ( char *string, int num )
{
	trap ( "c_timcur", 0 );
}

/* --------------------------------------------------------------------- */
/* unpack and pack XCHAR strings as per SPP */

void
c_strupk ( char *param, XCHAR *x_param, int num )
{
	trap ( "c_strupk", 0 );
}

void
c_strpak ( XCHAR *x_outstr, char *outstr, int max)
{
	printf ( "c_strpak: %s\n", outstr );
	trap ( "c_strpak", 0 );
}

/* THE END */
