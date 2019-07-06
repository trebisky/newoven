/* ovenr - rotation data harvester
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

/* Two 4 byte objects are read.
 * The first is a clock.
 * The second is speed as rpm * 1000
 */
static int buf[2];

int get_ovenip ( int noven, int ncomp );
int speed_read ( int ip, int port, char *buf, int nbytes );

void
ovenr ( void )
{
	// char *host = "oven0v0";
	int noven = 0;
	int ncomp = 0;
	int port = 5107;
	int oven_ip;
	int n;
	int clock, speed;

	n = sizeof ( buf );
	oven_ip = get_ovenip ( noven, ncomp );
	if ( oven_ip == 0 ) {
	    printf ( "Cannot resolve oven IP\n" );
	    exit ( 1 );
	}

	printf ( "Oven IP = %08x\n", oven_ip );

	for ( ;; ) {
	    speed_read ( oven_ip, port, (char *) buf, n );
	    clock = ntohl ( buf[0] );
	    speed = ntohl ( buf[1] );
	    printf ( "%d %d\n", clock, speed );
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

#define	MAXNBYTES	4096

int
speed_read ( int ip, int port, char *buf, int nbytes )
{
	struct	sockaddr_in	sockaddr;
	unsigned short	portnumber = port;
	long	ipnumber = ip;
	int	s;
	int	nxfer;
	int	n;
	// char	*buffer;

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
	    perror ("socket");
	    return (s);
	}

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= htonl (ipnumber);

	if (connect (s, (struct sockaddr *)&sockaddr, sizeof (sockaddr)) < 0) {
/*	    perror ("connect");						*/
	    return (-1);
	}

	/*
	if ((buffer = malloc (nbytes)) == 0) {
	    close (s);
	    return (-1);
	}
	*/

	for (n = 0; n < nbytes; n += nxfer) {
	    if ((nxfer = read (s, buf+n, MIN (MAXNBYTES, nbytes-n))) < 0) {
		perror ("read");
		// free (buffer);
		close (s);
		return (nxfer);
	    }
	    if (nxfer == 0) {
		// free (buffer);
		close (s);
		return (-1);
	    }
	}

	// memcpy ( buf, buffer, nbytes );
	// free (buffer);
	close (s);
	return (0);
}

/* The END */
