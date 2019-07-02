/* logdata.c
 * 7-1-2019  T. Trebisky
 * Implemented using Cfitsio.
 */

#include <time.h>
#include <string.h>
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

/* We use a single static array for the image buffer.
 * this is fine because only ovend calls this routine, and
 * the calls are serial.  This code does not need to be reentrant.
 */
static float array [MAX_ARRAY];

static void mkname ( char *buf, char *prefix );
static fitsfile * mk_new_file ( char *filename, int nvals );
static void insert_record ( fitsfile *fptr, int index, float *data, int num );

static int mkindex ( void );
static void set_first_record ( fitsfile *, int );
static void set_last_record ( fitsfile *, int );

void
logdata ( char *prefix, float *data, int num )
{
	char filename[128];
	int status = 0;
	fitsfile *fptr;
	int anynul;
	int n = MPD * num;
	int index;

	if ( num > MAX_COLS ) {
	    fprintf ( stderr, "Panic, data logging row too long: %d\n", num );
	    exit ( 1 );
	}

        mkname ( filename, prefix );
	index = mkindex ();

	if ( fits_open_file ( &fptr, filename, READWRITE, &status ) ) {
	    fptr = mk_new_file ( filename, num );
	    if ( ! fptr ) {
		fprintf ( stderr, "Cannot open %s for logging\n", filename );
		return;
	    }
	    status = 0;
	    set_first_record ( fptr, index + 1 );
	} else {
	    if ( fits_read_img ( fptr, TFLOAT, 1, n, 0, array, &anynul, &status) ) {
		fprintf ( stderr, "Cannot read %s for logging\n", filename );
		return;
	    }
	}

	insert_record ( fptr, index, data, num );
	set_last_record ( fptr, index + 1 );

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

static void
upd_long_key ( fitsfile *fptr, char *name, int val, char *comment )
{
	long lval = val;
	int status = 0;

	(void) fits_update_key ( fptr, TLONG, name, &lval, comment, &status);
}

static void
add_long_key ( fitsfile *fptr, char *name, int val, char *comment )
{
	long lval = val;
	int status = 0;

	(void) fits_write_key ( fptr, TLONG, name, &lval, comment, &status);
}

static void
add_str_key ( fitsfile *fptr, char *name, char *val, char *comment )
{
	int status = 0;

	(void) fits_write_key ( fptr, TSTRING, name, val, comment, &status);
}

static void
add_float_key ( fitsfile *fptr, char *name, float val, char *comment )
{
	int status = 0;
	float fval = val;

	(void) fits_write_key ( fptr, TFLOAT, name, &fval, comment, &status);
}

/*
SIMPLE  =                    T  /  FITS STANDARD
BITPIX  =                  -32  /  FITS BITS/PIXEL
NAXIS   =                    2  /  NUMBER OF AXES
NAXIS1  =                   66  /
NAXIS2  =                 1440  /

OBJECT  = 'adcv181001'          /
ORIGIN  = 'KPNO-IRAF'           /
DATE    = '2019-06-03T18:31:35'
IRAFNAME= 'adcv181001'          /  NAME OF IRAF IMAGE FILE
IRAF-MAX=           0.000000E0  /  DATA MAX
IRAF-MIN=           0.000000E0  /  DATA MIN
IRAF-BPX=                   32  /  DATA BITS/PIXEL
IRAFTYPE= 'REAL    '            /  PIXEL TYPE
FIRSTCOL=                    1
LASTCOL =                 1440
END
 */

static void
mk_iraf_date ( char *buf )
{
        struct tm *tp;
        time_t now;

        (void) time ( &now );
        tp = localtime ( &now );

        sprintf ( buf, "%04d-%02d-%02dT%2d:%02d:%02d",
            tp->tm_year + 1900,
            tp->tm_mon + 1,
            tp->tm_mday,
	    tp->tm_hour,
	    tp->tm_min,
	    tp->tm_sec );
}

static void
add_keys ( fitsfile *fptr, char *fname )
{
    // static char *d_str = "2019-06-03T18:31:35";
    char d_buf[80];

    mk_iraf_date ( d_buf );

    add_str_key ( fptr, "OBJECT", fname, NULL );
    add_str_key ( fptr, "ORIGIN", "SOML OVEN", NULL );
    // add_str_key ( fptr, "DATE", d_str, NULL );
    add_str_key ( fptr, "DATE", d_buf, NULL );
    add_str_key ( fptr, "IRAFNAME", fname, NULL );
    add_float_key ( fptr, "IRAF-MAX", 0.0, NULL );
    add_float_key ( fptr, "IRAF-MIN", 0.0, NULL );
    add_long_key ( fptr, "IRAF-BPX", 32, NULL );
    add_str_key ( fptr, "IRAFTYPE", "REAL    ", NULL );

    // add_long_key ( fptr, "EXPOSURE", 1500, "Exposure Time" );
}

static void
set_first_record ( fitsfile *fptr, int rec )
{
    add_long_key ( fptr, "FIRSTCOL", rec, NULL );
    add_long_key ( fptr, "LASTCOL", rec, NULL );
}

static void
set_last_record ( fitsfile *fptr, int rec )
{
    upd_long_key ( fptr, "LASTCOL", rec, NULL );
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

	add_keys ( fptr, filename );

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
insert_record ( fitsfile *fptr, int index, float *data, int num )
{
	int offset = index * num;

	memcpy ( &array[offset], data, num * sizeof(float) );
}

/* THE END */
