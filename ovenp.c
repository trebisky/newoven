/* ovenp - server to hand out P database
 */
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"
#include "protos.h"

int
ovenp ( void )
{
	int oven = 0;
	int comp = 0;
	int readonly = 0;
	int period = 60;
	int offset = 0;

	int status;

        status = init_globals ( oven, comp );
	if ( status < 0 ) {
	    printf ( "Cannot init globals (%d)\n", status );
	    return 1;
	}

        init_all_databases ( oven, comp, readonly );

	status = tportwrite(N_OVEN,N_COMP,PORTRP,globalp->pdbn,sizeof(p_database));
	if ( status ) {
	    printf ( "Cannot start P database server (%d)\n", status );
	    return 1;
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

	s = ovenp ();
	// printf ( "Game over.\n" );
	return s;
}

/* The END */
