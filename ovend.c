/* ovend - data harvester
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "oven.h"
#include "protos.h"

int db_dread_oven ( void );

extern int oven;
extern int comp;
extern int readonly;
extern int period;
extern int offset;
extern int log_data;

int
ovend ( void )
{

	time_t now;
	int status;

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
	    // printf ( "Sleeping\n" );
            if (period) {
                time (&now);
                sleep (period - now%period + offset);
            }

	    // printf ( "Getting data\n" );
            status = db_dread_oven ();
	    // ( returns 1 if OK )
	    // printf ( "Got data: %d\n", status );

            stalereport ();

            if (status && log_data)
                datalogger ();
        }

	return 0;
}

int
main ( int argc, char **argv )
{
	int s;

	if ( oven_check_db () )
	    return 1;

	offset = 5;
	oven_set_args ( argc, argv );


	s = ovend ();
	printf ( "Game over.\n" );
	return s;
}

/* The END */
