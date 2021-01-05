/* ovenb - bi-param data server
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
ovenb ( void )
{
	int status;

        status = init_globals ( oven, comp );
	if ( status < 0 ) {
	    printf ( "Cannot init globals (%d)\n", status );
	    return 1;
	}

	/* We must be readonly, otherwise we will create
	 * a multitude of databases.
	 */
	readonly = 1;
        init_all_databases ( oven, comp, readonly );

	status = tportwrite(N_OVEN,N_COMP,PORTRB,globalp->bdbn,sizeof(b_database));
	if ( status ) {
	    printf ( "Cannot start B database server (%d)\n", status );
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

	s = ovenb ();
	// printf ( "Game over.\n" );
	return s;
}

/* The END */
