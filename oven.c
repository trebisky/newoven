#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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

static int
init_context ( void )
{
	CONTEXT	*cp;

	cp = (CONTEXT *)calloc (1, sizeof(CONTEXT));

	/* 6-23-2019 */
	if ( ! cp )
	    return 20;

	/*
	status = (int)cp;
	switch (status) {
	case 0:
	    return (status+20);
	    break;
	default:
	    status = 0;
	    break;
	}
	*/
	globalp->contextp = cp;
	return 0;
}

#ifdef notdef
static void
free_context ( void )
{
	free ((char *)globalp->contextp);
	globalp->contextp = (CONTEXT *)0;
}
#endif

int
oven_gui ( void )
{
	int status;


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
        status = init_menus ();
	if ( status ) {
	    printf ( "Cannot init menus (%d)\n", status );
	    return 1;
	}

	/* Run the menus! */
        status = do_menus ( period, offset );

	if ( status ) {
	    printf ( "Cannot run menus (%d)\n", status );
	    return 1;
	}
	return 0;
}

int
main ( int argc, char **argv )
{
	int s;

	oven_check_db ();

	/* XXX - faster update, just for fun.
	 * we never directly talk to the onboard oven any faster,
	 * so there is no harm in this.
	 */
	period = 10;
	offset = 0;

	/* This is the old default */
	// period = 60;
	// offset = 8;

	oven_set_args ( argc, argv );

	s = oven_gui ();
	// printf ( "Game over.\n" );
	return 0;
}

/* The END */
