/* ovend - data harvester
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "oven.h"
#include "protos.h"

int db_dread_oven ( void );

int
ovend ( void )
{
	int oven = 0;
	int comp = 0;
	int readonly = 0;
	int period = 60;
	int offset = 0;
	int log_data = 0;

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

	    /*
            if (status && log_data)
                datalogger ();
	    */
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

	s = ovend ();
	printf ( "Game over.\n" );
	return s;
}

/* The END */
