#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"

#include <stdlib.h>
#include <string.h>

/* cache_parameter - cache the parameter database.
 */
int
cache_parameter ( void )
{
	p_database	*pdb;
	// char	*malloc ();

	pdb = (p_database *)malloc (sizeof(p_database));
	if (pdb == 0)
	    return (-1);

	memcpy ((char *)pdb, (char *)Pdb, sizeof(p_database));
	OPdb = Pdb;
	Pdb = pdb;

	return (0);
}

/* uncache_parameter - uncache the parameter database.
 */
int
uncache_parameter ( void )
{
	memcpy ((char *)OPdb, (char *)Pdb, sizeof(p_database));
	free ((char *)Pdb);
	Pdb = OPdb;
	OPdb = 0;
	return (0);
}

/* flcache_parameter - discard the parameter database cache.
 */
int
flcache_parameter ( void )
{
	free ((char *)Pdb);
	Pdb = OPdb;
	OPdb = 0;
	return (0);
}

/* cache_biparameter - cache the biparameter database.
 */
int
cache_biparameter ( void )
{
	b_database	*bdb;
	// char	*malloc ();

	bdb = (b_database *)malloc (sizeof(b_database));
	if (bdb == 0)
	    return (-1);

	memcpy ((char *)bdb, (char *)Bdb, sizeof(b_database));
	OBdb = Bdb;
	Bdb = bdb;

	return (0);
}

/* uncache_biparameter - uncache the biparameter database.
 */
int
uncache_biparameter ( void )
{
	memcpy ((char *)OBdb, (char *)Bdb, sizeof(b_database));
	free ((char *)Bdb);
	Bdb = OBdb;
	OBdb = 0;
	return (0);
}

/* flcache_biparameter - discard the biparameter database cache.
 */
int
flcache_biparameter ( void )
{
	free ((char *)Bdb);
	Bdb = OBdb;
	OBdb = 0;
	return (0);
}
