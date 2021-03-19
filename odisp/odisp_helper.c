/* odisp_helper.c
 *
 * Tom Trebisky  3-19-2021
 *
 * This is a new tool to pull information from shared memory
 * to be passed to the new odisp.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"
#include "protos.h"

extern int oven;
extern int comp;
extern int readonly;

enum db_type { DB_SHM, DB_LOCAL };

enum db_type db_type = DB_LOCAL;

/* -status is the default.
 * This can be provided (as well any any other thing)
 * but is not necessary.
 * If some idiot puts several options on the line,
 * they get the last load option specified.
 */
static void
set_args ( int argc, char **argv )
{
	int x_oven, x_comp;
	char *p;

	--argc;
	++argv;

	while ( argc ) {
	    p = *argv;
	    // printf ( "ARG: %s\n", p );

	    --argc;
	    ++argv;
	}
}

p_database *
attach_shm_database ( void )
{
	int status;

	oven = 0;
	comp = 0;
	readonly = 1;

        status = init_globals ( oven, comp );
	if ( status < 0 ) {
	    fprintf ( stderr, "Cannot init globals (%d)\n", status );
	    return NULL;
	}

	status = init_database ( oven, comp, readonly );

        if ( status ) {
            fprintf ( stderr, "Cannot init database (%d)\n", status );
            return NULL;
        }

	return Pdb;
}

static char database_name[] = "database";

p_database *
attach_local_database ( void )
{
	p_database *local_db;
	b_database *extra;
        FILE    *fp;
	int e_size = sizeof(p_database) + sizeof(b_database);
	struct stat st_buf;

	// -- should be 133548 bytes
	// printf ( "Total database size = %d\n", e_size );

	if ( stat ( database_name, &st_buf ) < 0 ) {
	    fprintf ( stderr, "Cannot find database file: %s\n", database_name );
	    return NULL;
	}

	if ( st_buf.st_size != e_size ) {
	    fprintf ( stderr, "database file wrong size: %s\n", database_name );
	    return NULL;
	}

        fp = fopen (database_name, "r");
	if ( fp == NULL ) {
	    fprintf ( stderr, "Cannot open database: %s\n", database_name );
	    return NULL;
	}

	local_db = (p_database *) malloc ( sizeof(p_database) );
	extra = (b_database *) malloc ( sizeof(b_database) );

	fread ( (char *)extra, sizeof(b_database), 1, fp);
	fread ( (char *)local_db, sizeof(p_database), 1, fp);
	fclose (fp);

	free ( (char *) extra );

	printf ( "Local database loaded !!\n" );

	return local_db;
}

int
main ( int argc, char **argv )
{
	p_database *db;

	/* This checks the size of the database structure,
	 * verifying that things are compiled right.
	 * It does nothing with the actual database
	 */
	oven_check_db ();

	set_args ( argc, argv );

	if ( db_type == DB_SHM ) {
	    db = attach_shm_database ();
	} else {
	    db = attach_local_database ();
	}

	if ( db == NULL ) {
	    fprintf ( stderr, "Sorry\n" );
	    return 1;
	}

	return 0;
}

/* The END */
