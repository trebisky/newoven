/* ovene - error harvester
 * (this is ovenz, which is ovene hacked for testing)
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "oven.h"
#include "protos.h"

// At one time this switched on the ovenz additions.
#define ZTEST

#ifdef ZTEST
#include "menus.h"
#include "context.h"
#include "global.h"
#endif

// From errorreport.c
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>

#include <time.h>
#include <string.h>

/* Oven specific */
#include "errnum.h"
#include "errmes.h"

extern int oven;
extern int comp;
extern int readonly;
extern int period;
extern int offset;

#ifdef notdef
static void
set_hof ( int p, int f, int e, int count )
{
	Edb->panel[p].fase[f].element[e].hof=count;
}

void
bogus_eread_oven ( void )
{
	bzero ( Edb, sizeof(e_database) );

	set_hof ( 0, 0, 1, 1 );
	set_hof ( 0, 0, 4, 5 );
	set_hof ( 0, 1, 0, 10 );
	set_hof ( 0, 1, 1, 1187404 );
}
#endif

int
ovenz ( void )
{

	time_t now;
	int status;

	printf ( "Starting error monitor for oven%dv%d\n", oven, comp );

        status = init_globals ( oven, comp );
	if ( status < 0 ) {
	    printf ( "Cannot init globals (%d)\n", status );
	    return 1;
	}

        status = init_database ( oven, comp, readonly );
	if ( status ) {
	    printf ( "Cannot init database (%d)\n", status );
	    return 1;
	}

#ifdef OLDZTEST
	bogus_eread_oven ();
	errorreport ();
#else
	for ( ;; ) {
	    printf ( "Sleeping\n" );
            if (period) {
                time (&now);
                sleep (period - now%period + offset);
            }

	    printf ( "Getting data\n" );
	    if (db_eread_oven ())
                errorreport ();
        }
#endif

	return 0;
}

int
main ( int argc, char **argv )
{
	int s;

	if ( oven_check_db () )
	    return 1;

	offset = 10;
	/* sun code did 11, and 12 for -c1 and -c2 */
	oven_set_args ( argc, argv );

	s = ovenz ();
	return s;
}

/* --------------------------------------------------------------------------------------*/
/* --------------------------------------------------------------------------------------*/
/* --------------------------------------------------------------------------------------*/

/* I copied in errorreport.c here */

void elogger ( database *, FILE *, int *, int , int );

// called only if count is non-zero
void
my_elogger ( FILE *fp, int count, int errnum, int addr )
{
	database *db = globalp->db;

	printf ( "MY elogger: count = %d\n", count );

	elogger (db,fp,&count,errnum,addr);
}

// #define	ELOG(count,errnum,addr)	if (count) elogger (db,fp,&count,errnum,addr);
#define	ELOG(count,errnum,addr)	if (count) my_elogger (fp,count,errnum,addr);

/* Prototypes - 6-21-2019 */
/* XXX - many could probably be static */
static void timestamp ( char * );
// void elogger ( database *, FILE *, int *, int , int );
void zoneereport ( database *, FILE * );
void pripanelereport ( database *, FILE * );
void panelereport ( database *, FILE * );
void dcuereport ( database *, FILE * );
void adcereport ( database *, FILE * );
void miscereport ( database *, FILE * );
void ereport ( database *, FILE * );

#ifdef notdef
/* Moved to oveng.c */

/* timeoutreport - do timeout reporting
 */
void
timeoutreport ( void )
{
	FILE	*fp;
	int	fd;
	int	count = 1;
	database *db = globalp->db;

	while ((fd = open ("errors.lok", O_RDWR | O_CREAT | O_EXCL)) < 0)
	    sleep (1);
	close (fd);
	fp = fopen ("errors.log", "a");

	ELOG (count, ERRTIME, 0)

	fclose (fp);
	unlink ("errors.lok");
}
#endif

/* errorreport - do error reporting
 */
void
errorreport ( void )
{
	FILE	*fp;
	int	fd;

	while ((fd = open ("errors.lok", O_RDWR | O_CREAT | O_EXCL)) < 0)
	    sleep (1);
	close (fd);
	fp = fopen ("errors.log", "a");

	ereport (globalp->db, fp);

	fclose (fp);
	unlink ("errors.lok");
}

/* ereport - do error reporting
 */
void
ereport ( database *db, FILE *fp )
{
	pripanelereport (db, fp);
	miscereport (db, fp);
	panelereport (db, fp);
	dcuereport (db, fp);
	zoneereport (db, fp);
	adcereport (db, fp);
}

/* miscereport - do misc error reporting
 */
void
miscereport ( database *db, FILE *fp)
{
	e_misc	*emisc = &db->error.misc;

	ELOG (emisc->texc, ERRTEXC, 0)
}

/* adcereport - do adc error reporting
 */
void
adcereport ( database *db, FILE *fp )
{
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE
	    e_adc	*eadc = db->error.adc  + panel/N_COMP;
	    ea_panel	*eapanel = eadc->panel + panel%N_COMP;
	    ea_fase	*eafase = eapanel->fase;

	    if (!(panel%N_COMP)) {
		ELOG (eadc->voltage.lower, ERRHVL, pfe)
		ELOG (eadc->voltage.upper, ERRHVU, pfe)
		ELOG (eadc->voltage.indef, ERRHVI, pfe)
	    }

	    ELOG (eapanel->current.lower, ERRPCL, pfe)
	    ELOG (eapanel->current.upper, ERRPCU, pfe)
	    ELOG (eapanel->current.indef, ERRPCI, pfe)

	    for (fase = 0; fase < N_FASE; FASE__, eafase++) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;

		ELOG (eafase->current.lower, ERRFCL, pfe)
		ELOG (eafase->current.upper, ERRFCU, pfe)
		ELOG (eafase->current.indef, ERRFCI, pfe)
		ELOG (eafase->voltage.lower, ERRFVL, pfe)
		ELOG (eafase->voltage.upper, ERRFVU, pfe)
		ELOG (eafase->voltage.indef, ERRFVI, pfe)
	    }
	}
}

/* dcuereport - do dcu error reporting
 */
void
dcuereport ( database *db, FILE *fp)
{
	DCL_DCU
	e_dcu	*edcu = db->error.dcu;

	for (dcu = 0; dcu < N_DCU; DCU__, edcu++) {

	    DNTX dntx = ((dcu*N_COUNTER)*N_TIC)*N_TC;
	    DCL_COUNTER
	    e_counter	*ecounter = edcu->counter;
	    DCL_SELECT
	    e_select	*eselect = edcu->select;

	    ELOG (edcu->ton, ERRTON, dntx)
	    ELOG (edcu->tof, ERRTOF, dntx)
	    ELOG (edcu->dst, ERRDST, dntx)

	    for (counter = 0; counter < N_COUNTER; COUNTER__, ecounter++) {

		DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC)*N_TC;
		DCL_TIC
		e_tic	*etic = ecounter->tic;

		for (tic = 0; tic < N_TIC; TIC__, etic++) {

		    DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC;
		    p_tc	*ptc = ptic->tc;
		    i_tc	*itc = itic->tc;
		    d_tc	*dtc = dtic->tc;
		    int		tc;
		    e_tc	*etc = etic->tc;

		    ELOG (etic->jtl, ERRJTL, dntx)
		    ELOG (etic->ijt, ERRIJT, dntx)

		    for (tc = 0; tc < N_TC; ptc++, itc++, dtc++, tc++, etc++) {

			DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TC+tc;

			if (tc >= N_TTMP)
			    ;
			ELOG (etc->fpe, ERRFPE, dntx)
			ELOG (etc->back, ERRBACK, dntx)
			ELOG (etc->shrt, ERRSHRT, dntx)
			ELOG (etc->melt, ERRMELT, dntx)
			ELOG (etc->open, ERROPEN, dntx)
		    }
		}
	    }

	    for (select = 0; select < N_SELECT; SELECT__, eselect++) {

		DSB	dsb = (dcu*N_SELECT+select)*N_BUSS;
		DCL_BUSS
		e_buss	*ebuss = eselect->buss;

		for (buss = 0; buss < N_BUSS; BUSS__, ebuss++) {

		    DSB	dsb = (dcu*N_SELECT+select)*N_BUSS+buss;

		    ELOG (ebuss->mer, ERRMER, dsb)
		}
	    }
	}
}

/* pripanelereport - do priority panel error reporting
 */
void
pripanelereport ( database *db, FILE *fp)
{
	DCL_PANEL
	e_panel	*epanel = db->error.panel;

	for (panel = 0; panel < N_PANEL; PANEL__, epanel++) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE
	    e_fase	*efase = epanel->fase;

	    for (fase = 0; fase < N_FASE; FASE__, efase++) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;
		DCL_ELEMENT
		e_element	*eelement = efase->element;

		for (element = 0; element < N_ELEMENT; ELEMENT__, eelement++) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    ELOG (eelement->hon, ERRHON, pfe)
		}
	    }
	}
}

/* panelereport - do panel error reporting
 */
void
panelereport ( database *db, FILE *fp)
{
	DCL_PANEL
	e_panel	*epanel = db->error.panel;

	for (panel = 0; panel < N_PANEL; PANEL__, epanel++) {

	    PFE	pfe = (panel*N_FASE)*N_ELEMENT;
	    DCL_FASE
	    e_fase	*efase = epanel->fase;

	    ELOG (epanel->ppe, ERRPPE, pfe)
	    ELOG (epanel->pkp, ERRPKP, pfe)

	    for (fase = 0; fase < N_FASE; FASE__, efase++) {

		PFE	pfe = (panel*N_FASE+fase)*N_ELEMENT;
		DCL_ELEMENT
		e_element	*eelement = efase->element;

		for (element = 0; element < N_ELEMENT; ELEMENT__, eelement++) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    ELOG (eelement->con, ERRCON, pfe)
		    ELOG (eelement->von, ERRVON, pfe)
		    ELOG (eelement->hof, ERRHOF, pfe)
		    ELOG (eelement->vof, ERRVOF, pfe)
		    ELOG (eelement->cof, ERRCOF, pfe)
		    ELOG (eelement->her, ERRHER, pfe)
		    ELOG (eelement->hsc, ERRHSC, pfe)
		    ELOG (eelement->rof, ERRROF, pfe)
		    ELOG (eelement->ron, ERRRON, pfe)
		    ELOG (eelement->ooii, ERROOII, pfe)
		    ELOG (eelement->oioi, ERROIOI, pfe)
		    ELOG (eelement->oiio, ERROIIO, pfe)
		    ELOG (eelement->iooi, ERRIOOI, pfe)
		    ELOG (eelement->ioio, ERRIOIO, pfe)
		    ELOG (eelement->iioo, ERRIIOO, pfe)
		}
	    }
	}
}

/* zoneereport - do zone error reporting
 */
void
zoneereport ( database *db, FILE *fp)
{
	DCL_ZONE
	e_zone	*ezone = db->error.zone;

	for (zone = 0; zone < N_ZONE; ZONE__, ezone++) {
	    ELOG (ezone->zsc, ERRZSC, zone)
	}
}

/* timestamp - write out a timestamp
 */
static void
timestamp ( char *string)
{
	time_t	now;

	// char	*ctime();
	// char	*strchr ();
	/* ctime yields something like:
	 * Wed Jun 30 21:49:08 1993\n
	 */

	time (&now);
	strcpy (string, ctime (&now));

	if (strchr (string, '\n'))
	    *(strchr (string, '\n')) = 0;
	string[strlen(string)-5] = 0;
}

#define EBUFSIZE	100

/* elogger - error logger
 */
void
elogger ( database *db, FILE *fp, int *count, int errnum, int addr)
{
	char	saddr[6];
	char	sloc[12];
	char	string[80];
	char	safe_buf[EBUFSIZE];

	     if (!strcmp (errmenu[errnum], "zo")) przone (saddr, addr);
	else if (!strcmp (errmenu[errnum], "he")) prpfe  (saddr, addr);
	else if (!strcmp (errmenu[errnum], "pp")) prp00  (saddr, addr);
	else if (!strcmp (errmenu[errnum], "tc")) prdntx (saddr, addr);
	else if (!strcmp (errmenu[errnum], "sb")) prdsb  (saddr, addr);
	else if (!strcmp (errmenu[errnum], "dc")) prd000 (saddr, addr);
	else if (!strcmp (errmenu[errnum], "ti")) prdnt0 (saddr, addr);
	else if (!strcmp (errmenu[errnum], "al")) prpfe  (saddr, addr);
	else saddr[0] = 0;

	if (!strcmp (errmenu[errnum], "he"))
	    prloc (sloc, &phe_pfe (&db->parameter, addr)->loc);
	else if (!strcmp (errmenu[errnum], "tc"))
	    if (addr%N_TC < N_TTMP)
		prloc (sloc, &ptc_dntx(&db->parameter, addr)->loc);
	    else
		strcpy (sloc, saddr);
	else
	    strcpy (sloc, saddr);

	timestamp (string);

#ifdef	VERBOSE_ERRORS
	fprintf (fp, "%s %s\n", string, errmes[errnum]);
	fprintf (fp, "oven%dv%d %3d error count = %6d @ %-5s<%s %s>\n",
	  globalp->noven,
	  globalp->ncomp,
	  errnum,
	  *count,
	  saddr,
	  errmenu[errnum],
	  sloc);
#endif

#ifdef ORIGINAL_ERRORS
	/* should be LEN_ELOGLINE (79) characters long
	 */
	fprintf (fp,
	  "%16.16s %1.1dv%1.1d#%6.6d@%-4.4s<%2.2s %-11.11s>%2.2d %-27.27s\n",
	  string+4,
	  globalp->noven,
	  globalp->ncomp,
	  *count,
	  saddr,
	  errmenu[errnum],
	  sloc,
	  errnum,
	  errmes[errnum]);
#endif

/* In September of 2020, some data corruption yielded huge values for the count
 * value that exceeded the 6 digits of space provided here.  This caused the total
 * length of the string to be longer than 79 bytes.  This breaks logic in the
 * error log viewer that expects fixed length records in the log file.
 * This code ensures that will not happen again.
 * Although proper software wouldn't have caused this in the first place,
 * doing things this way simply makes things more robust.
 */
#define SAFER_ERRORS
#ifdef SAFER_ERRORS
	sprintf ( safe_buf,
	  "%16.16s %1.1dv%1.1d#%6.6d@%-4.4s<%2.2s %-11.11s>%2.2d %-27.27s\n",
	  string+4,
	  globalp->noven,
	  globalp->ncomp,
	  *count,
	  saddr,
	  errmenu[errnum],
	  sloc,
	  errnum,
	  errmes[errnum]);

	  if ( safe_buf[LEN_ELOGLINE-1] != '\n' ) {
	    safe_buf[LEN_ELOGLINE-1] = '\n';
	    safe_buf[LEN_ELOGLINE] = '\0';
	    fprintf ( stderr, "ovene - line trimmed\n" );
	  }

	  fputs ( safe_buf, fp );
	  fflush ( fp );
#endif

	*count = 0;
}

/* THE END */

/* The END */
