/* oven_shm.c
 *
 * Tom Trebisky  9-8-2020
 *
 * This is a new tool to check and initialize shared memory.
 * In general, it will not be invoked directly, but will be
 *  called by the "cleaner" script, which is a user friendly
 *  front end.
 * This will never write to the oven.
 * It will, as requested, read B and P data from the oven in
 *  order to initialize a shared memory segment
 *
 * Options:
 *	- oload - load database from oven (create if necessary)
 *	- dload - load database from disk (create if necessary)
 *	- check - see if database exists (return number of zones).
 */
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

#ifdef notdef
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
#endif

static int
oven_setup ( void )
{
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
#endif
	return 0;
}

/* I thought about having the ability to "zero" the database,
 * but I can't really think how that would be useful.
 * There should ALWAYS be a way to immediately load the database
 * with something valid.
 */

enum shm_mode { STATUS, DISK_LOAD, OVEN_LOAD };

static enum shm_mode my_mode = STATUS;

/* -status is the default.
 * This can be provided (as well any any other thing)
 * but is not necessary.
 * If some idiot puts several options on the line,
 * they get the last load option specified.
 */
static void
shm_set_args ( int argc, char **argv )
{
	int x_oven, x_comp;
	char *p;

	--argc;
	++argv;

	while ( argc ) {
	    p = *argv;
	    // printf ( "ARG: %s\n", p );

	    if ( p[0] == '-' && strncmp ( "oload", &p[1], 5 ) == 0 ) {
		my_mode = OVEN_LOAD;
	    }

	    if ( p[0] == '-' && strncmp ( "dload", &p[1], 5 ) == 0 ) {
		my_mode = DISK_LOAD;
	    }

	    --argc;
	    ++argv;
	}
}

static void
do_disk_load ( void )
{
	int status;

	// printf ( "Disk load\n" );
        status = init_database ( oven, comp, readonly );
	if ( status ) {
	    printf ( "Cannot init database (%d)\n", status );
	    // return 1;
	}
	db_bpread_disk ();
}

static void
do_oven_load ( void )
{
	int status;

	// printf ( "Oven load\n" );
        status = init_database ( oven, comp, readonly );
	if ( status ) {
	    printf ( "Cannot init database (%d)\n", status );
	    // return 1;
	}
	db_bpread_oven ();
}

/* Active zones have a non-zero schedule node count
 */
static int
zone_count ( void )
{
	p_database *pdb;
	int i;
	int count = 0;

	pdb = Pdb;
	for ( i=0; i<N_ZONE; i++ ) {
	    //printf ( "Zone %d:  %d\n", i, pdb->zone[i].n_node );
	    if ( pdb->zone[i].n_node > 0 )
		count++;
	}
	return count;
}

static int
do_status ( void )
{
	int status;

	//printf ( "status " );
	//printf ( " oven = %d, comp = %d\n", oven, comp );

	readonly = 1;
        status = init_database ( oven, comp, readonly );
	if ( status ) {
	    // printf ( "Cannot init database (%d)\n", status );
	    return 999;
	}
	return zone_count();
}

int
main ( int argc, char **argv )
{
	int rv;

	/* This checks the size of the database structure,
	 * verifying that things are compiled right.
	 * It does nothing with the actual database
	 */
	oven_check_db ();

	/* Neither of these change the args, they just set
	 * variables according to the contents.
	 */
	oven_set_args ( argc, argv );
	shm_set_args ( argc, argv );

	if ( oven_setup () ) {
	    printf ( "Trouble in setup, exiting\n" );
	    return 0;
	}

#ifdef notdef
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
#endif

	switch ( my_mode ) {
	    case DISK_LOAD:
		do_disk_load ();
		break;
	    case OVEN_LOAD:
		do_oven_load ();
		break;
	    default:
		rv = do_status ();
		if ( rv < 100 )
		    printf ( "%d Zones\n", rv );
		else
		    printf ( "Sorry\n" );
	}

	return 0;
}

/* The END */
