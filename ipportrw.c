#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* XXX - replace perror with better messages */

#define	MAXNBYTES	4096

#define	ABS(a)		( ((a) >= 0) ? (a) : -(a) )
#define	MIN(a,b)	( ((a) < (b)) ? (a) : (b) )
#define	MAX(a,b)	( ((a) > (b)) ? (a) : (b) )

// Let the system do this.
// #define bzero(a,n)      memset(a,0,n)
// #define bcopy(a,b,n)    memmove(b,a,n)

/* long is not 4 bytes on a 64 bit machine.
 * and htonl() no longer works with longs either.
 * temp is used, just in case htonl() is buggy and has side effects.
 */
static inline void
endian_fix ( char *b, int n )
{
	int n32 = n / sizeof(uint32_t);
	u_int32_t *p32;
	int i;

	p32 = ( u_int32_t * ) b;
	for ( i=0; i < n32; i++ ) {
	    u_int32_t temp;

	    temp = *p32;
	    *p32++ = htonl ( temp );

	}
}

/* getovenip - get the oven computer's ip number.
 * Note that ntohl() is no longer defined to work on long types
 * it works on 32 bit objects, which are uint32_t or u_int32_t
 */
int
getovenip ( int noven, int ncomp )
{
	char	hostname[33];
	struct	hostent	*hp;
	uint32_t	ipnumber;
	uint32_t	ip;

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

/* ipportwrite - write to   an IP port.
 */
int
ipportwrite (ip, port, buf, nbytes)
int	ip;
int	port;
char	*buf;
int	nbytes;
{
	struct	sockaddr_in	sockaddr;
	unsigned short	portnumber = port;
	long	ipnumber = ip;
	int	s;
	int	nxfer;
	int	n;
	char	*buffer;
	// char	*malloc();

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
	if (htonl(1) == 1) {
	    buffer = buf;
	} else {
	    if ((buffer = malloc (nbytes)) == 0) {
		close (s);
		return (-1);
	    }

	    memcpy ( buffer, buf, nbytes );
	    endian_fix ( buffer, nbytes );

#ifdef notdef
	    long *lbuf = (long *)buf;
	    long *lbuffer = (long *)buffer;
	    int nlongs = nbytes/sizeof(long);
	    /* swap and copy quickly and in atomic 4 byte pieces */
	    for (n = 0; n < nlongs; n++) {
		long temp = *lbuf++;
	        *lbuffer++ = htonl(temp);
	    }
#endif
	}

	for (n = 0; n < nbytes; n += nxfer) {
	    if ((nxfer = write (s, buffer+n, MIN (MAXNBYTES, nbytes-n))) < 0) {
		perror ("write");
		if (htonl(1) != 1)
		    free (buffer);
		close (s);
		return (nxfer);
	    }
	}
	if (htonl(1) != 1)
	    free (buffer);
	close (s);
	return (0);
}

/* ipportread  - read  from an IP port.
 */
int
ipportread (ip, port, buf, nbytes)
int	ip;
int	port;
char	*buf;
int	nbytes;
{
	struct	sockaddr_in	sockaddr;
	unsigned short	portnumber = port;
	long	ipnumber = ip;
	int	s;
	int	nxfer;
	int	n;
	char	*buffer;
	// char	*malloc();

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

	if ((buffer = malloc (nbytes)) == 0) {
	    close (s);
	    return (-1);
	}

	for (n = 0; n < nbytes; n += nxfer) {
	    if ((nxfer = read (s, buffer+n, MIN (MAXNBYTES, nbytes-n))) < 0) {
		perror ("read");
		free (buffer);
		close (s);
		return (nxfer);
	    }
	    if (nxfer == 0) {
		free (buffer);
		close (s);
		return (-1);
	    }
	}

#ifdef notdef
	if (ntohl(1) == 1) {
	    bcopy (buffer, buf, nbytes);
	} else {

	    long *lbuffer = (long *)buffer;
	    long *lbuf = (long *)buf;
	    int nlongs = nbytes/sizeof(long);
	    /* swap and copy quickly and in atomic 4 byte pieces */
	    for (n = 0; n < nlongs; n++) {
		long temp = *lbuffer++;
	        *lbuf++ = ntohl(temp);
	    }
	}
#endif

	if (ntohl(1) != 1)
	    endian_fix ( buffer, nbytes );

	memcpy ( buf, buffer, nbytes );
	free (buffer);
	close (s);
	return (0);
}

/* tportwrite - listen for connections and write data from buffer
 * (i.e. this implements a server).
 */
int
tportwrite (noven, ncomp, port, buf, nbytes)
int	noven;
int	ncomp;
int	port;
char	*buf[];
int	nbytes;
{
	struct	sockaddr_in	rsockaddr;
	struct	sockaddr_in	sockaddr;
	char	*buffer;
	int	s, r;
	// int	n;
	socklen_t n;
	int	nxfer;
	int	on = 1;
	unsigned short	portnumber = port;
	unsigned long	ip;
	int	iip;
	int	oven, comp;
	// char	*malloc();

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    return (-1);

	if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR,(char *)&on,sizeof(on)) < 0)
	    return (-2);

	bzero ((char *)(&sockaddr), sizeof(sockaddr));
	sockaddr.sin_family	= AF_INET;
	sockaddr.sin_port	= htons (portnumber);
	sockaddr.sin_addr.s_addr= INADDR_ANY;

	if (bind (s, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
	    return (-3);

	if (listen (s, 1) < 0)
	    return (-4);

	while (1) {
	    bzero ((char *)(&rsockaddr), sizeof(rsockaddr));
	    n = sizeof(rsockaddr);
	    if ((r = accept (s, (struct sockaddr *)&rsockaddr, &n)) < 0)
	        continue;
	    ip = ntohl (rsockaddr.sin_addr.s_addr);
	    bcopy ((char *)&ip, (char *)&iip, sizeof(iip));
	    for (oven = 0; oven < noven; oven++) {
		for (comp = 0; comp < ncomp; comp++) {
		    if (iip == getovenip (oven, comp)) {
			if (buf[oven]) {

			    if (htonl(1) == 1) {
				buffer = buf[oven];
			    } else {
				if ((buffer = malloc (nbytes)) == 0) {
				    break;
				}

				memcpy ( buffer, buf[oven], nbytes );
				endian_fix ( buffer, nbytes );


#ifdef notdef
				long *lbuf = (long *)buf[oven];
				long *lbuffer = (long *)buffer;
				int nlongs = nbytes/sizeof(long);
				/* swap and copy quickly and in atomic 4 byte pieces */
				for (n = 0; n < nlongs; n++) {
				    long temp = *lbuf++;
				    *lbuffer++ = htonl(temp);
				}
#endif
			    }

			    for (n = 0; n < nbytes; n += nxfer) {
				if ((nxfer = write(r, buffer+n, MIN (MAXNBYTES, nbytes-n))) < 0)
				    break;
			    }

			    if (htonl(1) != 1)
				free (buffer);
			}
			break;
		    }
		}
		if (comp < ncomp)
		    break;
	    }
	    close (r);
	}
}

/* THE END */
