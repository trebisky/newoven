/* glue.c
 * -- stubs for things that once were provided by SPP code
 * In particular hgkey.x
 *  tjt  6-21-2019
 */
#include "oven.h"
#include "protos.h"

/* HGKEY.x */

/* Some kind of raw mode interface to the console.
 * hgkey calls thgkey.
 */
void
hgkey_ ( int *a, int *b, int *c )
{
}

void
thgkey_ ( XINT *val )
{
}

/* Page through help file
 */

void
hpageh_ ( void )
{
}

/* Flush stderr and stdout */
void
flgkey_ ( int *val )
{
}

/* Calls clgstr (in domenus.c ?? ) - get string */
void
hclgst_ ( XCHAR *a, XCHAR *b, XINT *c )
{
}

/* From IRAF sys/libc */

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
	return 0;
}

void
c_xttysize ( int *c, int* l )
{
}

void
c_ttyclear ( int tty_fd, int tty )
{
}

void
c_ttygoto ( int tty_fd, int tty, int xyz, int nlines )
{
}

void
c_ttyclearln ( int tty_fd, int tty )
{
}

void
c_ttycdes ( int tty )
{
}

void
c_ttyputline ( int tty_fd, int tty, char * text, int map_cc )
{
}

void
c_ttyctrl ( int tty_fd, int tty, char *cmd, int xyz )
{
}

int
c_timcur ( char *string, int num )
{
}

void
c_strupk ( char *param, XCHAR *x_param, int num )
{
}

void
c_strpak ( XCHAR *x_outstr, char *outstr, int max)
{
}

/* THE END */
