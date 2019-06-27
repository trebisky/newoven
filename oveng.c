/* ovend - data harvester
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"
#include "protos.h"

/* db_gread_oven - database gong read from  oven
 */
int
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

int
oveng ( void )
{
	int oven = 0;
	int comp = 0;
	int readonly = 0;
	int period = 60;
	int offset = 0;

	time_t now;
	int status;
        int     gong;
        int     n;

        status = init_globals ( oven, comp );
	if ( status < 0 ) {
	    printf ( "Cannot init globals (%d)\n", status );
	    return 1;
	}

        status = init_context ();
	if ( status ) {
	    printf ( "Cannot init context (%d)\n", status );
	    return 1;
	}

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
                // ring ();
		con_bell ();
            }
        }

	return 0;
}

void
show_sizes ( void )
{
	printf ( "B database: %9d bytes\n", sizeof(b_database) );
	printf ( "P database: %9d bytes\n", sizeof(p_database) );
	printf ( "I database: %9d bytes\n", sizeof(i_database) );
	printf ( "D database: %9d bytes\n", sizeof(d_database) );
	printf ( "E database: %9d bytes\n", sizeof(e_database) );
	printf ( "total database: %9d bytes\n", sizeof(database) );
}

#define CORRECT_DB_SIZE 263748

int
main ()
{
	int s;

	if ( sizeof(database) != CORRECT_DB_SIZE ) {
	    show_sizes ();
	    printf ( "ERROR - bad compile, database size wrong\n" );
	    return 1;
	}

	s = oveng ();
	// printf ( "Game over.\n" );
	return s;
}

/* The END */
