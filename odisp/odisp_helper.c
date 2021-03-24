/* odisp_helper.c
 *
 * Tom Trebisky  3-19-2021
 *
 * This is a tool to pull information from shared memory
 * to be passed to the new odisp.
 *
 * It can now be built without linking in any other oven code.
 *  (well, it does need oven.h)
 * It can read tc/he locations and config either from
 *  shared memory (i.e. during a casting) or later from a
 *  binary database file.
 * It would not be hard to make it capable of reading an
 *  ascii config/database file.
 *
 * We can specify 3 kinds of data (ttmp, htmp, or hpwr)
 *  this was called "info" in the original odisp
 * We can display 5 views (previously called "aspect")
 *  base, lid, wall, mold, alum
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <math.h>

#include "oven.h"

static float degtorad = 3.14159 / 180.0;

enum db_type { DB_SHM, DB_LOCAL };

enum db_type db_type = DB_LOCAL;

#define NUM_TC	720
#define NUM_HE	300

/* Can describe either a heater or a thermocouple */
struct info {
	int r;		/* radius in inches */
	int t;		/* theta in degrees */
	int z;
	int flags;
	int moniker;
	float theta;
	float xpos;
	float ypos;
	float zpos;
	float value;	/* tc or he value */
};

/* flags
 *  these do double duty as a sort of enum also.
 */
#define F_TC	0x0001
#define F_HE	0x0002
#define F_LID	0x0010
#define F_BASE	0x0020
#define F_WALL	0x0040
#define F_MOLD	0x0080
#define F_ALUM	0x0100

char *info = "ttmp";
int view = F_LID;

/* This pad business is some belt and suspenders checking.
 * if the oven database changes so that the expected TC and HE
 * count grows beyond what this program expects, we will catch
 * the change and complain.
 */
#define PAD_TC	NUM_TC + 10
#define PAD_HE	NUM_HE + 10

struct info tc_data[PAD_TC];
struct info he_data[PAD_HE];

#ifdef OLD_SPP_CODE
/* Parameters and macros to decide what part of the oven
 * a tc or he is located in.
 * Note that the polar coordinates within the oven are stored
 * as integers (believe it or not) in the oven shared memory.
 */

/* There are different limits for heaters vs thermocouples
 * for the wall tests.
 */
//  but only for the wall tests
//  walls tests r,z -- IS_WALL (radius, z)
// all the rest just test z
define HBMIN     -1.                    # base minimum and maximum z values
define HBMAX      6.
define HLMIN     52.                    # lid minimum and maximum z values
define HLMAX     95.
define HWMIN    191.                    # wall minimum and maximum radii
define HWMAX    193.
define HMMIN     19.                    # mold minimum and maximum z values
define HMMAX     21.
define HAMIN     -11.                   # alum minimum and maximum z values
define HAMAX     -9.

define TBMIN     -1.                    # base minimum and maximum z values
define TBMAX      6.
define TLMIN     52.                    # lid minimum and maximum z values
define TLMAX     95.
define TWMIN    187.                    # wall minimum and maximum radii
define TWMAX    193.
define TMMIN     19.                    # mold minimum and maximum z values
define TMMAX     21.
define TAMIN     -11.                   # alum minimum and maximum z values
define TAMAX     -9.

define IS_BASE  ($1 >= bmin && $1 <= bmax)
define IS_LID   ($1 >= lmin && $1 <= lmax)
define IS_WALL  ($1 >= wmin && $1 <= wmax && $2 > bmax && $2 < lmin)
define IS_MOLD  ($1 >= mmin && $1 <= mmax)
define IS_ALUM  ($1 >= umin && $1 <= umax)
#endif

#define BMIN     -1
#define BMAX      6
#define LMIN     52
#define LMAX     95

#define MMIN     19
#define MMAX     21
#define AMIN     -11
#define AMAX     -9

#define HE_WMIN    191
#define HE_WMAX    193
#define TC_WMIN    187
#define TC_WMAX    193

#define is_base(x)  (x->z >= BMIN && x->z <= BMAX)
#define is_lid(x)   (x->z >= LMIN && x->z <= LMAX)
#define is_mold(x)  (x->z >= MMIN && x->z <= MMAX)
#define is_alum(x)  (x->z >= AMIN && x->z <= AMAX)

#define is_wall_he(x)  (x->r >= HE_WMIN && x->r <= HE_WMAX && x->z > BMAX && x->z < LMIN)
#define is_wall_tc(x)  (x->r >= TC_WMIN && x->r <= TC_WMAX && x->z > BMAX && x->z < LMIN)

/* ---------------------------------------------------------------*/

/*
 * If some idiot puts several options on the line,
 * they get the last options specified.
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

char    *
shmalloc ( int nbytes,  int noven,  int ncomp,  int readonly)
{
        key_t   key = 256 + noven%16*16 + ncomp%4;
        int     shmflg1 = (readonly) ? 0444 : 0644 | IPC_CREAT;
        int     shmflg2 = (readonly) ? SHM_RDONLY : 0;
        int     shmid;
        char    *shmptr;

        /* ovenp and ovenb scan through all possible ovens
         * looking for databases (readonly), so we want to
         * be silent in those cases.
         */
        if ((shmid = shmget (key, nbytes, shmflg1)) == -1) {
            if (!readonly) {
                perror ("shmget");
                // fprintf ( stderr, "TJT error: shmget: %d %d %d %d\n", nbytes, key, readonly, shmid );
                fprintf ( stderr, "shm - cannot get shared memory (key=%x, flag=%x)\n", key, shmflg1 );
            }
            return ((char *)0);
        }


        shmptr = (char *)shmat (shmid, (char *)0, shmflg2);
        if ( shmptr == (char *) -1 ) {
            if (!readonly) {
                perror ("shmat");
                fprintf ( stderr, "shm - cannot attach shared memory\n" );
            }
            return ((char *)0);
        }

        return (shmptr);
}

database *
attach_shm_database ( void )
{
        database        *db;
        char    *shmalloc ();

	int noven = 0;
	int ncomp = 0;
	int readonly = 1;

	/*
        status = init_globals ( oven, comp );
	if ( status < 0 ) {
	    fprintf ( stderr, "Cannot init globals (%d)\n", status );
	    return NULL;
	}
	*/

        db = (database *)shmalloc (sizeof(database), noven, ncomp, readonly);

        if ( ! db ) {
            fprintf ( stderr, "Cannot init database\n" );
            return NULL;
        }

	return db;
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

void
classify ( struct info *dingus )
{
	float radius;

	if ( is_mold(dingus) )
	    dingus->flags |= F_MOLD;
	if ( is_lid(dingus) )
	    dingus->flags |= F_LID;
	if ( is_base(dingus) )
	    dingus->flags |= F_BASE;
	if ( is_alum(dingus) )
	    dingus->flags |= F_ALUM;

	if ( dingus->flags & F_TC ) {
	    if ( is_wall_tc(dingus) )
		dingus->flags |= F_WALL;
	} else {
	    if ( is_wall_he(dingus) )
		dingus->flags |= F_WALL;
	}

	radius = dingus->r;
	dingus->theta = dingus->t * degtorad;

	dingus->xpos = radius * sin (dingus->theta);
	dingus->ypos = radius * cos (dingus->theta);
	dingus->zpos = dingus->z;
}

/* tc_loc - tc locations
 * The TC moniker is a 4 digit number dntx where:
 *  d = dcu (0-5)
 *  n = counter (0-5)
 *  t = tic (0-3)
 *  x = tc (0-4)
 */
// int get_tc_locs ( database *db, int *tc_index, int *tc_radius, int *tc_theta, int *tc_z )
// int get_tc_locs ( database *db, int *tc_radius, int *tc_theta, int *tc_z )
int
get_tc_locs ( database *db, struct info *tc, int limit )
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
		    int		itc;

		    for (itc = 0; itc < N_TTMP; ptc++, itc++) {

		      DNTX dntx = ((dcu*N_COUNTER+counter)*N_TIC+tic)*N_TTMP+itc;

			// *tc_index++ = dntx+1;
			tc->r = ptc->loc.r;
			tc->t = ptc->loc.t;
			tc->z = ptc->loc.z;
			tc->flags = F_TC;
			tc->moniker = ((dcu*10 + counter) * 10 + tic) * 10 + itc;
			classify ( tc );
			// *tc_radius++ = ptc->loc.r;
			// *tc_theta++ = ptc->loc.t;
			// *tc_z++ = ptc->loc.z;
			tc++;
			count++;
			if ( count >= limit )
			    return count;
		    }
		}
	    }
	}
	return count;
}

/* he_loc - heater element locations
 * The HE moniker is a 3 digit number pfe where:
 *  p = panel (0-9)
 *  f = fase (0-2)
 *  e = element (0-9)
 */
// int get_he_locs ( database *db, int *he_index, int *he_radius, int *he_theta, int *he_z )
// int get_he_locs ( database *db, int *he_radius, int *he_theta, int *he_z )
int
get_he_locs ( database *db, struct info *he, int limit )
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

		    // *he_index++ = pfe+1;
		    he->r = pelement->loc.r;
		    he->t = pelement->loc.t;
		    he->z = pelement->loc.z;
		    he->flags = F_HE;
		    he->moniker = panel * 100 + fase * 10 + element;
		    classify ( he );
		    // *he_radius++ = pelement->loc.r;
		    // *he_theta++ = pelement->loc.t;
		    // *he_z++ = pelement->loc.z;
		    he++;
		    count++;
		    if ( count >= limit )
			return count;
		}
	    }
	}
	return count;
}


int
fetch_tc_he ( database *db )
{
	int n_tc;
	int n_he;

	// n_tc = get_tc_locs ( db, tc_radius, tc_theta, tc_z );
	n_tc = get_tc_locs ( db, tc_data, PAD_TC );
	printf ( "%d TC's fetched\n", n_tc );
	if ( n_tc != NUM_TC ) {
	    fprintf ( stderr, "TC database mismatch!\n" );
	    return 0;
	}

	// n_he = get_he_locs ( db, he_radius, he_theta, he_z );
	n_he = get_he_locs ( db, he_data, PAD_HE );
	printf ( "%d HE's fetched\n", n_he );
	if ( n_he != NUM_HE ) {
	    fprintf ( stderr, "HE database mismatch!\n" );
	    return 0;
	}

	return 1;
}

char *
flags_to_s ( int f )
{
	static char ss[10];
	char *p;

	p = ss;
	if ( f & F_BASE ) *p++ = 'B';
	if ( f & F_LID ) *p++ = 'L';
	if ( f & F_WALL ) *p++ = 'W';
	if ( f & F_MOLD ) *p++ = 'M';
	if ( f & F_ALUM ) *p++ = 'A';
	*p = '\0';
	return ss;
}

void
print_tc_he ( void )
{
	int i;

	for ( i=0; i<NUM_TC; i++ )
	    printf ( "TC %3d tc-%d %12d %12d %12d %8s\n", i+1,
		tc_data[i].moniker,
		tc_data[i].r, tc_data[i].t, tc_data[i].z,
		flags_to_s(tc_data[i].flags) );

	for ( i=0; i<NUM_HE; i++ )
	    printf ( "HE %3d he-%d %12d %12d %12d %8s\n", i+1,
		he_data[i].moniker,
		he_data[i].r, he_data[i].t, he_data[i].z,
		flags_to_s(he_data[i].flags) );
}

/*
 *  theta = 0 is NORTH at top of image (large y).
 *   theta increases to EAST, in clockwise sense.
 */
void
mk_grid ( int who, int what )
{
	float xx[NUM_TC];
	float yy[NUM_TC];
	int i;
	int nxy;

	struct info *source;
	int n_source;

	if ( who == F_TC ) {
	    source = tc_data;
	    n_source = NUM_TC;
	} else {
	    source = he_data;
	    n_source = NUM_HE;
	}

	nxy = 0;
	for ( i=0; i<n_source; i++ ) {
	    if ( ! source[i].flags & what )
		continue;
	    if ( what == F_WALL ) {
		xx[nxy] = source[i].theta;
		yy[nxy] = source[i].zpos;
	    } else {
		xx[nxy] = source[i].xpos;
		yy[nxy] = source[i].ypos;
	    }
	    /* XXX - legacy code carefully checked that
	     * no x,y is a duplicate.
	     */
	}
}

#define CORRECT_DB_SIZE 263748

int
oven_check_db ( void )
{
        if ( sizeof(database) != CORRECT_DB_SIZE ) {
            // show_sizes ();
            printf ( "ERROR - bad compile, database size wrong\n" );
            return 1;
        }

        return 0;
}

int
main ( int argc, char **argv )
{
	database *db;

	/* This checks the size of the database structure,
	 * verifying that things are compiled right.
	 * It does nothing with the actual database
	 */
	if ( oven_check_db () )
	    return 1;

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
	print_tc_he ();
	mk_grid ( F_TC, F_LID );

	return 0;
}

/* The END */
