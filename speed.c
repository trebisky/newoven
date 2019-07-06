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
