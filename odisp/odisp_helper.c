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

database *
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

	return Gdb;
}

static char database_name[] = "database";

database *
attach_local_database ( void )
{
	database *local_db;
	// b_database *extra;
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

	local_db = (database *) malloc ( sizeof(database) );
	// local_db = (p_database *) malloc ( sizeof(p_database) );
	// extra = (b_database *) malloc ( sizeof(b_database) );

	fread ( (char *)&local_db->biparameter, sizeof(b_database), 1, fp);
	fread ( (char *)&local_db->parameter, sizeof(p_database), 1, fp);
	fclose (fp);

	// free ( (char *) extra );

	printf ( "Local database loaded !!\n" );

	return local_db;
}

/* tc_loc - tc locations
 */
int
get_tc_locs ( database *db, int *tc_index, int *tc_radius, int *tc_theta, int *tc_z )
{
	// database	*db = globalp->db;
	int count = 0;
	DCL_DCU

	for (dcu = 0; dcu < N_DCU; DCU__) {

	    DCL_COUNTER

	    for (counter = 0; counter < N_COUNTER; COUNTER__) {

		DCL_TIC

		for (tic = 0; tic < N_TIC; TIC__) {

		    p_tc	*ptc = ptic->tc;
		    int		tc;

		    for (tc = 0; tc < N_TTMP; ptc++, tc++) {

		      DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TTMP+tc;

			*tc_index++ = dntx+1;
			*tc_radius++ = ptc->loc.r;
			*tc_theta++ = ptc->loc.t;
			*tc_z++ = ptc->loc.z;
			count++;
		    }
		}
	    }
	}
	return count;
}

/* he_loc - heater element locations
 */
int
get_he_locs ( database *db, int *he_index, int *he_radius, int *he_theta, int *he_z )
{
	// database	*db = globalp->db;
	int count = 0;
	DCL_PANEL

	for (panel = 0; panel < N_PANEL; PANEL__) {

	    DCL_FASE

	    for (fase = 0; fase < N_FASE; FASE__) {

		DCL_ELEMENT

		for (element = 0; element < N_ELEMENT; ELEMENT__) {

		    PFE		pfe = (panel*N_FASE+fase)*N_ELEMENT+element;

		    *he_index++ = pfe+1;
		    *he_radius++ = pelement->loc.r;
		    *he_theta++ = pelement->loc.t;
		    *he_z++ = pelement->loc.z;
		    count++;
		}
	    }
	}
	return count;
}

void
fetch_tc_he ( database *db )
{
	int tc_index[1000];
	int tc_radius[1000];
	int tc_theta[1000];
	int tc_z[1000];
	int n_tc;

	int he_index[500];
	int he_radius[500];
	int he_theta[500];
	int he_z[500];
	int n_he;

	n_tc = get_tc_locs ( db, tc_index, tc_radius, tc_theta, tc_z );
	printf ( "%d TC's fetched\n", n_tc );
	n_he = get_he_locs ( db, he_index, he_radius, he_theta, he_z );
	printf ( "%d HE's fetched\n", n_he );
}

int
main ( int argc, char **argv )
{
	database *db;

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

	fetch_tc_he ( db );

	return 0;
}

/* The END */
