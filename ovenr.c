/* ovenr - rotation data harvester
 */
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "oven.h"
#include "protos.h"

/* ovenr is a bit of an odd duck.
 * It doesn't need access to global memory or the database.
 * It is a very simple network client.
 *
 * Delays and time synchronization take place in the V computer.
 * When a connection is established, the V computer asks for the
 * speed and will delay until fresh data is obtained.
 * Because of this only one instance of ovenr should ever be run.
 */

/* Two 4 byte objects are read */
static int buf[2];

int get_ovenip ( int noven, int ncomp );

void
ovenr ( void )
{
	// char *host = "oven0v0";
	int noven = 0;
	int ncomp = 0;
	int port = 5107;
	int oven_ip;
	int n;
	int speed1, speed2;

	n = sizeof ( buf );
	oven_ip = get_ovenip ( noven, ncomp );
	printf ( "Oven IP = %08x\n", oven_ip );
	if ( oven_ip == 0 ) {
	    printf ( "Trouble\n" );
	    exit ( 1 );
	}

	buf[0] = 999;
	buf[1] = 999;

	for ( ;; ) {
	    ipportread ( oven_ip, port, (char *) buf, n );
	    speed1 = ntohl ( buf[0] );
	    speed2 = ntohl ( buf[1] );
	    printf ( "%d %d\n", speed1, speed2 );
	    break;
	}

}

int
main ()
{
	ovenr ();
	// printf ( "Game over.\n" );
}

/* getovenip - get the oven computer's ip number.
 * Note that ntohl() is no longer defined to work on long types
 * it works on 32 bit objects, which are uint32_t or u_int32_t
 */
int
get_ovenip ( int noven, int ncomp )
{
        char    hostname[33];
        struct  hostent *hp;
        uint32_t        ipnumber;
        uint32_t        ip;

        /* ovens #3 to #8 not implemented yet */
        if (noven >= 3 && noven <= 8)
            return (0);

        sprintf (hostname, "oven%dv%d", noven, ncomp);
        if ((hp = gethostbyname (hostname)) == 0)
            return (0);

        bcopy ((char *)hp->h_addr, (char *)&ipnumber,
            MIN (hp->h_length, sizeof(ipnumber)));

        ip = ntohl (ipnumber);
        return (ip);
}

/* The END */
