/* ovene - error harvester
 * (this is ovenz, which is ovene hacked for testing)
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "oven.h"
#include "protos.h"

#define ZTEST

#ifdef ZTEST
#include "menus.h"
#include "context.h"
#include "global.h"
#endif

extern int oven;
extern int comp;
extern int readonly;
extern int period;
extern int offset;

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

#ifdef ZTEST
	bogus_eread_oven ();
	errorreport ();
#else
	for ( ;; ) {
	    // printf ( "Sleeping\n" );
            if (period) {
                time (&now);
                sleep (period - now%period + offset);
            }

	    // printf ( "Getting data\n" );
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

/* The END */
