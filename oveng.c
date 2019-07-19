/* ovend - data harvester
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"

#include "errnum.h"
#include "errmes.h"
#include "protos.h"

static int db_gread_oven ( void );
static void timeoutreport ( void );

extern int oven;
extern int comp;
extern int readonly;
extern int period;
extern int offset;

/* db_gread_oven - database gong read from  oven
 */
static int
db_gread_oven ( void )
{
	int	ip;
	int	ncomp;
	int	gong;
	int	maxgong = 0;
	int	status;
	int	result = 0;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    if (!Pdb->misc.vup[ncomp])
		continue;
	    globalp->ncomp = ncomp;
	    if (ip = getovenip (globalp->noven, globalp->ncomp)) {
		status = ipportread (ip, PORTGN, (char *)&gong, sizeof(gong));
		if (status == -1) {
		    gong = 0;
		    timeoutreport ();
		}
	    } else {
		continue;
	    }
	    maxgong = MAX (maxgong, gong);
	    result += (status != 0);
	}
	return (result + maxgong);
}

/* Copied here to avoid pulling in errorreport.c
 *  and all of its dependencies.
 */
static void
g_timestamp ( char *string)
{
        time_t  now;

        /* ctime yields something like:
         * Wed Jun 30 21:49:08 1993\n
         */

        time (&now);
        strcpy (string, ctime (&now));

        if (strchr (string, '\n'))
            *(strchr (string, '\n')) = 0;
        string[strlen(string)-5] = 0;
}

/* Special trimmed version of elogger for gong timeouts only */
static void
gong_elogger ( database *db, FILE *fp, int *count, int errnum, int addr)
{
	char	saddr[6];
	char	sloc[12];
	char	string[80];

	saddr[0] = 0;
	strcpy (sloc, saddr);

	// if (!strcmp (errmenu[errnum], "ti")) prdnt0 (saddr, addr);

	g_timestamp (string);
	/* This yields something like:
	 *    Wed Jun 30 21:49:08 1993
	 * the day of week gets trimmed off below to
	 *    Jun 30 21:49:08 1993
	 */

	/* errnum should be 42
	 * errmenu[errnum] should be
	 * errmes[errnum]  should be "Timeout !!",
	 */

	/* should be LEN_ELOGLINE long
	 */

	fprintf (fp,
	  "%16.16s %1.1dv%1.1d#%6.6d@%-4.4s<%2.2s %-11.11s>%2.2d %-27.27s\n",
	  string+4,
	  globalp->noven,
	  globalp->ncomp,
	  *count,
	  saddr,
	  // errmenu[errnum],
	  "ti",
	  sloc,
	  errnum,
	  //errmes[errnum] );
	  "Timeout !!" );

	// *count = 0;
}


/* timeoutreport - do timeout reporting
 */
static void
timeoutreport ( void )
{
	FILE	*fp;
	int	fd;
	int	count = 1;
	database *db = globalp->db;

	/* Acquire lock */
	while ((fd = open ("errors.lok", O_RDWR | O_CREAT | O_EXCL)) < 0)
	    sleep (1);
	close (fd);

	fp = fopen ("errors.log", "a");

	// ELOG (count, ERRTIME, 0)
	// elogger ( db, fp, &count, ERRTIME, 0 );
	gong_elogger ( db, fp, &count, ERRTIME, 0 );
	fclose (fp);

	/* release lock */
	unlink ("errors.lok");
}

#define BELL_CH		007

static void
ring ( void )
{
	fprintf ( stdout, "%c", BELL_CH );
	fflush ( stdout );
}

int
oveng ( void )
{
	time_t now;
	int status;
        int     gong;
        int     n;

        status = init_globals ( oven, comp );
	if ( status < 0 ) {
	    printf ( "Cannot init globals (%d)\n", status );
	    return 1;
	}

#ifdef notdef
        status = init_context ();
	if ( status ) {
	    printf ( "Cannot init context (%d)\n", status );
	    return 1;
	}
#endif

        status = init_database ( oven, comp, readonly );
	if ( status ) {
	    printf ( "Cannot init database (%d)\n", status );
	    return 1;
	}

	for ( ;; ) {
            if (period) {
                time (&now);
                sleep (period - now%period + offset);
            }

            gong = db_gread_oven ();

            gong = (gong+7)/8;                  /* sunview */
            for (n = 0; n < gong; n++) {
                sleep (2);                      /* sunview */
                ring ();
            }
        }

	return 0;
}

int
main ( int argc, char **argv )
{
	int s;

	if ( oven_check_db () )
	    return 1;
	oven_set_args ( argc, argv );

	s = oveng ();
	// printf ( "Game over.\n" );
	return s;
}

/* The END */
