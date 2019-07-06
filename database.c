#include <stdio.h>
#include <unistd.h>

#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"
// #include "oxnames.h"

#include "protos.h"

static void
backup ( void )
{
}

/* db_bpwrite_disk - database parameters write to  disk
 */
void
db_bpwrite_disk ( void )
{
	FILE	*fp;
#ifdef	OLD_BACKUP
	char	*cp;
	char	*strchr();
	char	path2backup[256];
#endif

	unlink ("database.9");
	link ("database.8", "database.9"); unlink ("database.8");
	link ("database.7", "database.8"); unlink ("database.7");
	link ("database.6", "database.7"); unlink ("database.6");
	link ("database.5", "database.6"); unlink ("database.5");
	link ("database.4", "database.5"); unlink ("database.4");
	link ("database.3", "database.4"); unlink ("database.3");
	link ("database.2", "database.3"); unlink ("database.2");
	link ("database.1", "database.2"); unlink ("database.1");
	link ("database.0", "database.1"); unlink ("database.0");
	link ("database", "database.0"); unlink ("database");
	if (fp = fopen ("database", "w")) {
	    fwrite ((char *)Bdb, sizeof(b_database), 1, fp);
	    fwrite ((char *)Pdb, sizeof(p_database), 1, fp);
	    fclose (fp);
	}
#ifdef	OLD_BACKUP
	if (fp = fopen ("PATH2BACKUP", "r")) {
	    fread (path2backup, sizeof(path2backup), 1, fp);
	    fclose (fp);
	    if (cp = strchr (path2backup, '\n')) {
		*cp = 0;
		strcat (path2backup, "database");
		if (fp = fopen (path2backup, "w")) {
		    fwrite ((char *)Bdb, sizeof(b_database), 1, fp);
		    fwrite ((char *)Pdb, sizeof(p_database), 1, fp);
		    fclose (fp);
		}
	    }
	}
#else
	// BACKUP ();
	backup ();
#endif
}

/* db_bpread_disk - database parameters read from disk
 */
void
db_bpread_disk ( void )
{
	FILE	*fp;

	if (fp = fopen ("database", "r")) {
	    fread ((char *)Bdb, sizeof(b_database), 1, fp);
	    fread ((char *)Pdb, sizeof(p_database), 1, fp);
	    fclose (fp);
	}
}

/* db_bpwrite_oven - database biparameters and parameters write to  oven
 */
void
db_bpwrite_oven ( void )
{
	db_bwrite_oven ();
	db_pwrite_oven ();
}

/* db_bpread_oven - database biparameters and parameters read to  oven
 */
void
db_bpread_oven ( void )
{
	db_bread_oven ();
	db_pread_oven ();
}

/* db_pwrite_oven - database parameters write to  oven
 */
void
db_pwrite_oven ( void )
{
	int	ncomp, ip;

	for (ncomp = 0; ncomp < N_COMP; ncomp++)
	    if (Pdb->misc.vup[ncomp] && (ip=getovenip (globalp->noven,ncomp))) {
		Pdb->misc.vnumber = ncomp;
		if (ipportwrite(ip,PORTWP,(char *)Pdb,sizeof(p_database)) == -1)
		    ncompperror (globalp->noven, ncomp);
	    }
}

/* db_bwrite_oven - database biparameters write to  oven
 */
void
db_bwrite_oven ( void )
{
	int	ncomp, ip;

	for (ncomp = 0; ncomp < N_COMP; ncomp++)
	    if (Pdb->misc.vup[ncomp] && (ip=getovenip (globalp->noven,ncomp))) {
		if (ipportwrite(ip,PORTWB,(char *)Bdb,sizeof(b_database)) == -1)
		    ncompperror (globalp->noven, ncomp);
	    }
}

/* db_pread_oven - database parameters read to  oven
 */
void
db_pread_oven ( void )
{
	int	ip;

	if (ip = getovenip (globalp->noven, globalp->ncomp)) {
	    if (ipportread(ip,PORTRP,(char *)Pdb,sizeof(p_database)) == -1)
		ncompperror (globalp->noven, globalp->ncomp);
	}
}

/* db_bread_oven - database biparameters read to  oven
 */
void
db_bread_oven ( void )
{
	int	ip;

	if (ip = getovenip (globalp->noven, globalp->ncomp)) {
	    if (ipportread(ip,PORTRB,(char *)Bdb,sizeof(b_database)) == -1)
		ncompperror (globalp->noven, globalp->ncomp);
	}
}

/* db_dread_oven - database data read from  oven
 */
int
db_dread_oven ( void )
{
	int	ip;

	if (!Pdb->misc.vup[globalp->ncomp])
	    return (NO);
	if (ip = getovenip (globalp->noven, globalp->ncomp)) {
	    if (ipportread(ip,PORTRB,(char *)Bdb,sizeof(b_database)) == -1)
		ncompperror (globalp->noven, globalp->ncomp);
	    if (ipportread(ip,PORTRD,(char *)Ddb,sizeof(d_database)) == -1)
		ncompperror (globalp->noven, globalp->ncomp);
	} else {
	    return (NO);
	}
	return (YES);
}

/* db_eread_oven - database error read from  oven
 */
int
db_eread_oven ( void )
{
	int	ip;

	if (!Pdb->misc.vup[globalp->ncomp])
	    return (NO);
	if (ip = getovenip (globalp->noven, globalp->ncomp)) {
	    if (ipportread(ip,PORTER,(char *)Edb,sizeof(e_database)) == -1)
		ncompperror (globalp->noven, globalp->ncomp);
	} else {
	    return (NO);
	}
	return (YES);
}

#ifdef notdef
/* -- moved to oveng.c */

/* db_gread_oven - database gong read from  oven
 */
int
db_gread_oven ( void )
{
	int	ip;
	int	ncomp;
	int	gong;
	int	maxgong = 0;
	int	status;
	int	result = 0;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    if (!Pdb->misc.vup[ncomp])
		continue;
	    globalp->ncomp = ncomp;
	    if (ip = getovenip (globalp->noven, globalp->ncomp)) {
		status = ipportread (ip, PORTGN, (char *)&gong, sizeof(gong));
		if (status == -1) {
		    gong = 0;
		    timeoutreport ();
		}
	    } else {
		continue;
	    }
	    maxgong = MAX (maxgong, gong);
	    result += (status != 0);
	}
	return (result + maxgong);
}
#endif

void
ncompperror ( int noven, int ncomp)
{
	char	pmess[32];

	sprintf (pmess, "oven%dv%d", noven, ncomp);
	perror (pmess);
}

/* THE END */
