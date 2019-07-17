#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// TJT
#include <stdio.h>

#include "oven.h"
#include "protos.h"

/* shmalloc - shared memory allocator
 */
char	*
shmalloc ( int nbytes,  int noven,  int ncomp,  int readonly)
{
#ifndef	TEST
	key_t	key = 256 + noven%16*16 + ncomp%4;
	int	shmflg1 = (readonly) ? 0444 : 0644 | IPC_CREAT;
	int	shmflg2 = (readonly) ? SHM_RDONLY : 0;
	int	shmid;
	char	*shmptr;

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
#else
	char	*malloc();

	return (malloc (nbytes));
#endif
}

/* shmfree - shared memory deallocator
 */
void
shmfree ( char *shmptr, int noven, int ncomp, int remove)
{
#ifndef	TEST
	key_t	key = 256 + noven%16*16 + ncomp%4;
	int	shmid;
	struct	shmid_ds buf;

	if (shmdt (shmptr) == -1) {
/*	    perror ("shmdt");						*/
	    return;
	}
	if (!remove)
	    return;
	if ((shmid = shmget (key, 0, 0)) == -1) {
	    perror ("shmget");
	    return;
	}
	if (shmctl (shmid, IPC_RMID, &buf) == -1) {
	    perror ("shmctl");
	    return;
	}
#else
	free (shmptr);
#endif
}

/* THE END */
