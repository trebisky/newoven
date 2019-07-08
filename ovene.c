/* ovene - error harvester
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "oven.h"
#include "protos.h"

int oven = 0;
int comp = 0;
int readonly = 0;

int
ovene ( void )
{
	int period = 60;
	int offset = 0;
	int log_data = 1;

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

void
set_args ( int argc, char **argv )
{
	int x_oven, x_comp;
	char *p;

	--argc;
	++argv;

	while ( argc ) {
	    p = *argv;
	    printf ( "ARG: %s\n", p );
	    if ( p[0] == '-' && p[1] == 'o' ) {
		x_oven = atoi ( &p[2] );
		printf ( "Oven: %d\n", x_oven );
		if ( x_oven >=0 && x_oven <= 1 )
		    oven = x_oven;
	    }
	    if ( p[0] == '-' && p[1] == 'c' ) {
		x_comp = atoi ( &p[2] );
		printf ( "Comp: %d\n", x_comp );
		if ( x_comp >=0 && x_comp <= 2 )
		    comp = x_comp;
	    }
	    --argc;
	    ++argv;
	}
}

int
main ( int argc, char **argv )
{
	int s;

	if ( sizeof(database) != CORRECT_DB_SIZE ) {
	    show_sizes ();
	    printf ( "ERROR - bad compile, database size wrong\n" );
	    return 1;
	}

	set_args ( argc, argv );

	s = ovene ();
	printf ( "Game over.\n" );
	return s;
}

/* The END */
