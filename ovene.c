/* ovene - error harvester
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "oven.h"
#include "protos.h"

extern int oven;
extern int comp;
extern int readonly;
extern int period;
extern int offset;

int
ovene ( void )
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

	s = ovene ();
	return s;
}

/* The END */
