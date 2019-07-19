#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"
#include "protos.h"

#define DEFAULT_PERIOD	60

int oven = 0;
int comp = 0;
int readonly = 0;
int period = DEFAULT_PERIOD;
int offset = 0;

static void
show_sizes ( void )
{
	printf ( "B database: %9d bytes\n", sizeof(b_database) );
	printf ( "P database: %9d bytes\n", sizeof(p_database) );
	printf ( "I database: %9d bytes\n", sizeof(i_database) );
	printf ( "D database: %9d bytes\n", sizeof(d_database) );
	printf ( "E database: %9d bytes\n", sizeof(e_database) );
	printf ( "total database: %9d bytes\n", sizeof(database) );
}

void
oven_set_args ( int argc, char **argv )
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

#define CORRECT_DB_SIZE 263748

int
oven_check_db ( void )
{
	if ( sizeof(database) != CORRECT_DB_SIZE ) {
	    show_sizes ();
	    printf ( "ERROR - bad compile, database size wrong\n" );
	    return 1;
	}

	return 0;
}

#ifdef not_example
int
main ( int argc, char **argv )
{
	int s;

	if ( oven_check_db () ) {
	    return 1;
	}

	oven_set_args ( argc, argv );

	s = oven_gui ();
	return 0;
}
#endif

#ifdef notdef
static time_t xyz;

void
sizecheck ( void )
{
	printf ( "Sz int %d\n", sizeof(int) );
	printf ( "Sz long %d\n", sizeof(long) );
	printf ( "Sz timet %d\n", sizeof(time_t) );

	xyz = time ( (void *) 0 );
	printf ( "Time %ld  %016lx\n", xyz, xyz );
}
#endif

/* The END */
