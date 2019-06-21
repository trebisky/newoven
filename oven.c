#include <stdio.h>
#include <time.h>

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

void
oven ( void )
{
	init_menus ();
}

int
main ()
{
	oven ();
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
