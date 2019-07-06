#include <stdlib.h>
#include <string.h>

#include "oven.h"
#include "menus.h"
#include "context.h"

#define	GLOBALS
#include "global.h"

#include "protos.h"

// extern	char	*calloc();

static int iamthepilot ( void );

int
init_globals ( int noven, int ncomp)
{
	GLOBAL	*gp;
	int	status;

	if (globalp)
	    return (-1);
	gp = (GLOBAL *)calloc (1, sizeof(GLOBAL));

	/* 6-23-2019 */
	if ( ! gp )
	    return 10;
	status = 0;

	/*
	status = (int)gp;
	switch (status) {
	case 0:
	    return (status+10);
	    break;
	default:
	    status = 0;
	    break;
	}
	*/
	gp->noven = noven;
	gp->ncomp = ncomp;
	globalp = gp;
	return (status);
}

#ifdef notdef
/* Moved to oven.c */

int
init_context ( void )
{
	CONTEXT	*cp;
	int	status;

	cp = (CONTEXT *)calloc (1, sizeof(CONTEXT));

	/* 6-23-2019 */
	if ( ! cp )
	    return 20;
	status = 0;

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
	return (status);
}

void
free_context ( void )
{
	free ((char *)globalp->contextp);
	globalp->contextp = (CONTEXT *)0;
}
#endif

void
init_all_databases ( int noven, int ncomp, int readonly)
{
	int	oven;
	int	status;

	for (oven = 0; oven < N_OVEN; oven++)
	    status = init_database (oven, ncomp, readonly);
}

int
init_database ( int noven, int ncomp, int readonly)
{
	database	*db;
	int	status;
	char	*shmalloc ();

	if (!iamthepilot())
	    readonly = 1;
	db = (database *)shmalloc (sizeof(database), noven, ncomp, readonly);

	/* 6-23-2019 */
	if ( ! db )
	    return 30;
	status = 0;

	/*
	status = (int)db;
	switch (status) {
	case 0:
	    return (status+30);
	    break;
	default:
	    status = 0;
	    break;
	}
	*/

	Gdb = db;
	Bdb = &db->biparameter;
	Pdb = &db->parameter;
	Idb = &db->intermediate;
	Ddb = &db->data;
/*	globalp->obdb = Bdb;		*/
/*	globalp->opdb = Pdb;		*/
	globalp->readonly = readonly;
	globalp->gdbn[noven] = Gdb;
	globalp->pdbn[noven] = (char *)Pdb;
	globalp->bdbn[noven] = (char *)Bdb;
	return (status);
}

void
free_globals ( void )
{
	free ((char *)globalp);
	globalp = (GLOBAL *)0;
}

void
free_all_databases ( int noven, int ncomp, int remove )
{
	int	oven;

	for (oven = 0; oven < N_OVEN; oven++)
	    free_database (oven, ncomp, remove);
}

void
free_database ( int noven, int ncomp, int remove )
{
	database	*db;

	if (db = globalp->gdbn[noven]) {
	    shmfree ((char *)db, noven, ncomp, remove);
	    globalp->gdbn[noven] = (database *)0;
	    globalp->pdbn[noven] = (char *)0;
	    globalp->bdbn[noven] = (char *)0;
	}
}

static int
iamthepilot ( void )
{
	char	*getenv ();
	char	*user = getenv ("USER");

#define	TEST_PILOT
#ifndef	TEST_PILOT
	return (user && (!strcmp (user, "pilot") || !strcmp (user, "pilot2")));
#else
	/*
	return (user && !strcmp (user, "skip"));
	return (user && !strcmp (user, "tom"));
	*/
	return ( user && ( !strcmp (user, "pilot") ||
		!strcmp (user, "pilot2") ||
		!strcmp (user, "tom")
		) );
	// return 1;
#endif
}

/* THE END */
