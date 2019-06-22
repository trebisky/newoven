/* glue.c
 * -- stubs for things that once were provided by SPP code
 * In particular hgkey.x
 *  tjt  6-21-2019
 */
#include "oven.h"
#include "protos.h"

#include <stdio.h>
#include <stdlib.h>

static void
trap ( char *s, int stop )
{
	printf ( "TRAP: %s\n", s );
	if ( stop ) {
	    printf ( "TRAP: stopping execution\n" );
	    exit ( 2 );
	}
}

/* HGKEY.x */

/* Some kind of raw mode interface to the console.
 * hgkey calls thgkey.
 * reads a key into *key
 */
void
hgkey_ ( int *key, int *period, int *offset )
{
	trap ( "hgkey", 1 );
}

/* Fools around with terminal modes */
void
thgkey_ ( XINT *val )
{
	trap ( "thgkey", 0 );
}

/* Flush stderr and stdout */
void
flgkey_ ( int *val )
{
	trap ( "flgkey", 0 );
}

/* Calls clgstr (in domenus.c ?? )
 * This does some kind of lookup
 * The only call is with name "input".
 * This is involved with the image cursor business.
 - get string */
void
hclgst_ ( XCHAR *name, XCHAR *val, XINT *max )
{
	trap ( "hclgst", 1 );
}

/* Page through a help file
 */
void
hpageh_ ( void )
{
	trap ( "hpageh", 1 );
}


/* XXX - important */
void
backup_ ( void )
{
	trap ( "backup", 1 );
}

/* From IRAF sys/libc */
/* As an example, on my system, I can find source
 * in two places:
 *
 *  /u1/iraf/iraf_2.16.1/sys/libc/cttygoto.c
 *  /u1/iraf/src-2.16.1/sys/libc/cttygoto.c
 *
 */

/*
   40 |  int tty = c_ttyodes ("terminal");
   54 |      c_xttysize (&ncols, &nlines);
   60 |   c_ttyclear (tty_fd, tty);
  124 |  c_ttygoto (tty_fd, tty, 1, nlines);
  125 |  c_ttyclearln (tty_fd, tty);
  126 |  c_ttycdes (tty);
  152 |      c_ttyputline (tty_fd, tty, ip->text, map_cc);
  169 |      c_ttyctrl (tty_fd, tty, "ce", 1);
  491 |      if (c_timcur (string, sizeof(string))) {
  862 |  c_strupk (param, x_param, strlen (param)+1);
  864 |  c_strpak (x_outstr, outstr, maxch);
*/

int
c_ttyodes ( char *s )
{
	trap ( "c_ttyodes", 0 );
	return 0;
}

void
c_xttysize ( int *c, int* l )
{
	trap ( "c_xttysize", 0 );
}

void
c_ttyclear ( int tty_fd, int tty )
{
	trap ( "c_ttyclear", 0 );
}

void
c_ttygoto ( int tty_fd, int tty, int xyz, int nlines )
{
	printf ( "c_ttygoto: %d %d\n", xyz, nlines );
	trap ( "c_ttygoto", 0 );
}

void
c_ttyclearln ( int tty_fd, int tty )
{
	trap ( "c_ttyclearln", 0 );
}

void
c_ttycdes ( int tty )
{
	trap ( "c_ttycdes", 0 );
}

void
c_ttyputline ( int tty_fd, int tty, char * text, int map_cc )
{
	printf ( "c_ttyputline: %s\n", text );
	trap ( "c_ttyputline", 0 );
}

void
c_ttyctrl ( int tty_fd, int tty, char *cmd, int xyz )
{
	trap ( "c_ttyctrl", 0 );
}

int
c_timcur ( char *string, int num )
{
	trap ( "c_timcur", 0 );
}

void
c_strupk ( char *param, XCHAR *x_param, int num )
{
	trap ( "c_strupk", 0 );
}

void
c_strpak ( XCHAR *x_outstr, char *outstr, int max)
{
	printf ( "c_strpak: %s\n", outstr );
	trap ( "c_strpak", 0 );
}

/* THE END */
