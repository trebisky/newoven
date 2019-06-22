#include <stdio.h>
#include <time.h>

#include "oven.h"

/*
HOVEN (noven, ncomp, readonly, remove, period, offset)
int     *noven;
int     *ncomp;
int     *readonly;
int     *remove;
int     *period;
int     *offset;
{
        int     status;

        status = init_globals (*noven, *ncomp);
        if (status == -1) {
            free_database (*noven, *ncomp, 0);
            free_context ();
            free_globals ();
            status = init_globals (*noven, *ncomp);
        }
        if (status)
            return (status);
        status = init_context ();
        if (status)
            return (status);
        status = init_database (*noven, *ncomp, *readonly);
        if (status)
            return (status);
        status = init_menus ();
        if (status)
            return (status);
        status = do_menus (*period, *offset);
        free_menus ();
        free_database (*noven, *ncomp, *remove);
        free_context ();
        free_globals ();
        if (status)
            return (status);
        return (status);
}
*/

int
oven ( void )
{
	int oven = 0;
	int comp = 0;
	int readonly = 0;
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
	// init_menus ();
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
	if ( sizeof(database) != CORRECT_DB_SIZE ) {
	    show_sizes ();
	    printf ( "ERROR - bad compile, database size wrong\n" );
	    return 1;
	}
	oven ();
	printf ( "Game over.\n" );
	return 0;
}

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

/* The END */
