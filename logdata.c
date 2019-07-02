/* logdata.c
 * 7-1-2019  T. Trebisky
 * Implemented using Cfitsio.
 */

#include <time.h>
#include <cfitsio/fitsio.h>

/* typical call:
logdata ("htmp", htmp, sizeof(htmp)/sizeof(htmp[0]));
*/

#define MPD     (24*60)         /* 1440 minutes per day */
#define INDEFR  1.6e38

#define MAX_COLS        720             /* ttmp have 720 values */
#define MAX_ROWS        MPD

#define NDIM    2               /* we have a 2 dimensional image */

/* Big enough for anything we know about at this time */
#define MAX_ARRAY	(MAX_ROWS*MAX_COLS)

static float array [MAX_ARRAY];

static void mkname ( char *buf, char *prefix );
static fitsfile * mk_new_file ( char *filename, int nvals );
static void insert_record ( fitsfile *fptr, float *data, int num );

void
logdata ( char *prefix, float *data, int num )
{
	char filename[128];
	int status = 0;
	fitsfile *fptr;
	int anynul;
	int n = MPD * num;

	if ( num > MAX_COLS ) {
	    fprintf ( stderr, "Panic, data logging row too long: %d\n", num );
	    exit ( 1 );
	}

        mkname ( filename, prefix );

	if ( fits_open_file ( &fptr, filename, READWRITE, &status ) ) {
	    printf ( "Open fails: %d\n", status );
	    fptr = mk_new_file ( filename, num );
	    if ( ! fptr ) {
		fprintf ( stderr, "Cannot open %s for logging\n", filename );
		return;
	    }
	    status = 0;
	} else {
	    if ( fits_read_img ( fptr, TFLOAT, 1, n, 0, array, &anynul, &status) ) {
		fprintf ( stderr, "Cannot read %s for logging\n", filename );
		return;
	    }
	}

	insert_record ( fptr, data, num );

	if ( fits_write_img ( fptr, TFLOAT, 1, n, array, &status) ) {
	    fprintf ( stderr, "Cannot write %s for logging\n", filename );
	    return;
	}

	fits_close_file ( fptr, &status );
}

static void
mkname ( char *buf, char *prefix )
{
        struct tm *tp;
        time_t now;

        (void) time ( &now );
        tp = localtime ( &now );

        sprintf ( buf, "%s%02d%02d%02d.fits", prefix,
            tp->tm_year % 100,
            tp->tm_mon + 1,
            tp->tm_mday );
}

static int
mkindex ( void )
{
        struct tm *tp;
        time_t now;

        (void) time ( &now );
        tp = localtime ( &now );

	// x = TM_HOUR(tm)*60 + TM_MIN(tm)+1
	return tp->tm_hour * 60 + tp->tm_min;
}

static fitsfile *
mk_new_file ( char *filename, int nvals )
{
	fitsfile *fptr;
	int status;
	long naxes[NDIM];	/* Yes, long on 64 bit */
	int n;
	int i;

	status = 0;
	if ( fits_create_file ( &fptr, filename, &status) ) {
	    return ( (fitsfile *) 0 );
	}

	naxes[0] = MPD;
	naxes[1] = nvals;
	n = MPD * nvals;

	if ( fits_create_img ( fptr,  FLOAT_IMG, NDIM, naxes, &status) ) {
	    return ( (fitsfile *) 0 );
	}

	for ( i=0; i<n; i++ )
	    array[i] = INDEFR;

#ifdef notdef
	/* Do we really want to write this here and now ?
	 *  it is better to defer until the data record is added.
	 */
	if ( fits_write_img ( fptr, TFLOAT, 1, n, array, &status) ) {
	    return ( (fitsfile *) 0 );
	    exit ( 1 );
	}
#endif

	return fptr;
}

static void
insert_record ( fitsfile *fptr, float *data, int num )
{
	int index = mkindex ();
}

/* THE END */
