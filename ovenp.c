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

extern int oven;
extern int comp;
extern int readonly;
extern int period;
extern int offset;

int
ovenp ( void )
{
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

int
main ( int argc, char **argv )
{
	int s;

	if ( oven_check_db () )
	    return 1;
	oven_set_args ( argc, argv );

	s = ovenp ();
	// printf ( "Game over.\n" );
	return s;
}

/* The END */
