#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include "oven.h"
#include "protos.h"

int
toggle ( BOOL *status )
{
	*status = *status ? NO : YES;
	return (SCER_OK);
}

int
toggleb ( int *flag, int mask )
{
	*flag ^= mask;
	return (SCER_OK);
}

int
sctcloc ( char *s, p_database *pdb, DNTX *dntx )
{
	int	status;
	DNTX	addr, minaddr;
	LOC	loc, *ploc;
	float	del, dist, mindist = 8000000.0;

	if ((status = scloc (s, &loc)) != SCER_OK)
	    return (status);
	for (addr = 0; addr < N_DCU*N_COUNTER*N_TIC*N_TC; addr++) {
	    if (addr%N_TC >= N_TTMP)
		continue;
	    ploc = &(ptc_dntx (pdb, addr))->loc;
	    dist = 0.0;
	    del = ploc->x - loc.x;	dist += del*del;
	    del = ploc->y - loc.y;	dist += del*del;
	    del = ploc->z - loc.z;	dist += del*del;
	    if (dist == 0.0) {
		*dntx = addr;
		return (status);
	    }
	    if (dist < mindist) {
		mindist = dist;
		minaddr = addr;
	    }
	}
	*dntx = minaddr;
	return (status);
}

int
scheloc ( char *s, p_database *pdb, PFE *pfe )
{
	int	status;
	PFE	addr, minaddr;
	LOC	loc, *ploc;
	float	del, dist, mindist = 8000000.0;

	if ((status = scloc (s, &loc)) != SCER_OK)
	    return (status);
	for (addr = 0; addr < N_PANEL*N_FASE*N_ELEMENT; addr++) {
	    ploc = &(phe_pfe (pdb, addr))->loc;
	    dist = 0.0;
	    del = ploc->x - loc.x;	dist += del*del;
	    del = ploc->y - loc.y;	dist += del*del;
	    del = ploc->z - loc.z;	dist += del*del;
	    if (dist == 0.0) {
		*pfe = addr;
		return (status);
	    }
	    if (dist < mindist) {
		mindist = dist;
		minaddr = addr;
	    }
	}
	*pfe = minaddr;
	return (status);
}

int
scloc ( char *s, LOC *ploc )
{
	int	r, t, z;

	if (sscanf (s, "%d%d%d", &r, &t, &z) != 3)
	    return (SCER_SYNTAX);
	ploc->r = r;
	ploc->t = t;
	ploc->z = z;
	ploc->x = r * sin (t / RADIAN);
	ploc->y = r * cos (t / RADIAN);
	return (SCER_OK);
}

int
sclocr ( char *s, LOC *ploc )
{
	int	l1, l2;

	if (sscanf (s, "%d%d", &l1, &l2) != 2)
	    return (SCER_SYNTAX);
	ploc->r1 = MIN (l1, l2);
	ploc->r2 = MAX (l1, l2);
	return (SCER_OK);
}

int
scloct ( char *s, LOC *ploc )
{
	int	l1, l2;

	if (sscanf (s, "%d%d", &l1, &l2) != 2)
	    return (SCER_SYNTAX);
	ploc->t1 = MIN (l1, l2);
	ploc->t2 = MAX (l1, l2);
	return (SCER_OK);
}

int
sclocz ( char *s, LOC *ploc )
{
	int	l1, l2;

	if (sscanf (s, "%d%d", &l1, &l2) != 2)
	    return (SCER_SYNTAX);
	ploc->z1 = MIN (l1, l2);
	ploc->z2 = MAX (l1, l2);
	return (SCER_OK);
}

int
scaloc ( char *s, LOC *ploc )
{
	int	r, t, z;

	if (sscanf (s, "%3d%3d%3d", &r, &t, &z) != 3)
	    return (SCER_SYNTAX);
	ploc->r = r;
	ploc->t = t;
	ploc->z = z;
	ploc->x = r * sin (t / RADIAN);
	ploc->y = r * cos (t / RADIAN);
	return (SCER_OK);
}

int
scalocr ( char *s, LOC *ploc )
{
	int	l1, l2;

	if (sscanf (s, "%3d%3d", &l1, &l2) != 2)
	    return (SCER_SYNTAX);
	ploc->r1 = MIN (l1, l2);
	ploc->r2 = MAX (l1, l2);
	return (SCER_OK);
}

int
scaloct ( char *s, LOC *ploc )
{
	int	l1, l2;

	if (sscanf (s, "%3d%3d", &l1, &l2) != 2)
	    return (SCER_SYNTAX);
	ploc->t1 = MIN (l1, l2);
	ploc->t2 = MAX (l1, l2);
	return (SCER_OK);
}

int
scalocz ( char *s, LOC *ploc )
{
	int	l1, l2;

	if (sscanf (s, "%3d%3d", &l1, &l2) != 2)
	    return (SCER_SYNTAX);
	ploc->z1 = MIN (l1, l2);
	ploc->z2 = MAX (l1, l2);
	return (SCER_OK);
}

int
sctemp ( char *s, TEMP *tmp )
{
	TEMP	t;

	if (sscanf (s, "%f", &t) != 1)
	    return (SCER_SYNTAX);
	*tmp = t;
	return (SCER_OK);
}

int
scijb ( char *s, int *ijblock )
{
	int	ijb;

	if (sscanf (s, "%d", &ijb) != 1)
	    return (SCER_SYNTAX);
	*ijblock = ijb;
	return (SCER_OK);
}

int
scserial ( char *s, int *serial )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	*serial = n;
	return (SCER_OK);
}

int
scbeam ( char *s, int *beam )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	*beam = n;
	return (SCER_OK);
}

int
sctwist ( char *s, int *twist )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	*twist = n;
	return (SCER_OK);
}

int
scidepth ( char *s, int *idepth )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	*idepth = n;
	return (SCER_OK);
}

int
scepsilon ( char *s, int *epsilon )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	if (n < 0 || n > MAX_POWER)
	    return (SCER_RANGE);
	*epsilon = n;
	return (SCER_OK);
}

int
scmaxload ( char *s, int *maxload )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	if (n < 0 || n > N_ELEMENT)
	    return (SCER_RANGE);
	*maxload = n;
	return (SCER_OK);
}

int
scnoden ( char *s, int *noden )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	if (n < 0 || n > N_NODE)
	    return (SCER_RANGE);
	*noden = n;
	return (SCER_OK);
}

int
scnnnb ( char *s, int *nnnb )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	if (n < 0 || n > N_NNEIGHBOR)
	    return (SCER_RANGE);
	*nnnb = n;
	return (SCER_OK);
}

int
scnfnb ( char *s, int *nfnb )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	if (n < 0 || n > N_FNEIGHBOR)
	    return (SCER_RANGE);
	*nfnb = n;
	return (SCER_OK);
}

int
scvnumb ( char *s, int *vnumb )
{
	int	n;

	if (strcmp (s, "OFF") == 0) {
	    n = 0;
	} else if (strcmp (s, "off") == 0) {
	    n = 0;
	} else {
	    if (sscanf (s, "%d", &n) != 1)
	        return (SCER_SYNTAX);
	    if (n < 0 || n >= N_COMP)
	        return (SCER_RANGE);
	    n++;
	}
	*vnumb = n;
	return (SCER_OK);
}

int
scvportd ( char *s, int *vport )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	if (n < 0 || n >= N_DCU)
	    return (SCER_RANGE);
	*vport = n;
	return (SCER_OK);
}

int
scvportp ( char *s, int *vport )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	if (n < 0 || n >= 18)			/* (N_PANEL-1) ? */
	    return (SCER_RANGE);
	*vport = n;
	return (SCER_OK);
}

int
scpower ( char *s, POWER *power )
{
	int	n;

	if (sscanf (s, "%d", &n) != 1)
	    return (SCER_SYNTAX);
	if (n < 0 || n > MAX_POWER)
	    return (SCER_RANGE);
	*power = n;
	return (SCER_OK);
}

int
sctime ( char *s, TIME *ptime )
{
	int	mm, hh, dd;

	if (sscanf (s, "%d:%d:%d", &dd, &hh, &mm) != 3)
	    return (SCER_SYNTAX);
	if (mm < 0 || mm >= 60)
	    return (SCER_RANGE);
	if (hh < 0 || hh >= 60)
	    return (SCER_RANGE);
	if (dd < 0 || dd >= 100)
	    return (SCER_RANGE);
	*ptime = ((dd*24+hh)*60+mm)*60;
	return (SCER_OK);
}

int
screfv ( char *s, float *reference )
{
	float	refv;

	if (sscanf (s, "%f", &refv) != 1)
	    return (SCER_SYNTAX);
	*reference = refv;
	return (SCER_OK);
}

int
scpoly ( char *s, float *polycoef )
{
	float	poly;

	if (sscanf (s, "%f", &poly) != 1)
	    return (SCER_SYNTAX);
	*polycoef = poly;
	return (SCER_OK);
}

int
sccoef ( char *s, float *coef )
{
	float	f;

	if (sscanf (s, "%f", &f) != 1)
	    return (SCER_SYNTAX);
	*coef = f;
	return (SCER_OK);
}

int
scvolts ( char *s, float *value )
{
	float	f;

	if (sscanf (s, "%f", &f) != 1)
	    return (SCER_SYNTAX);
	*value = f;
	return (SCER_OK);
}

int
scamps ( char *s, float *value )
{
	float	f;

	if (sscanf (s, "%f", &f) != 1)
	    return (SCER_SYNTAX);
	*value = f;
	return (SCER_OK);
}

int
scstat ( char *s, BOOL *status )
{
	if (!strcmp (s, "d") || !strcmp (s, "D"))
	    *status = YES;
	else if (!strcmp (s, "e") || !strcmp (s, "E"))
	    *status = NO;
	else
	    return (SCER_RANGE);
	return (SCER_OK);
}

int
scenab ( char *s, BOOL *status )
{
	if (!strcmp (s, "e") || !strcmp (s, "E"))
	    *status = YES;
	else if (!strcmp (s, "d") || !strcmp (s, "D"))
	    *status = NO;
	else
	    return (SCER_RANGE);
	return (SCER_OK);
}

int
scclstat ( char *s, BOOL *status )
{
	if (!strcmp (s, "r") || !strcmp (s, "R"))
	    *status = YES;
	else if (!strcmp (s, "s") || !strcmp (s, "S"))
	    *status = NO;
	else
	    return (SCER_RANGE);
	return (SCER_OK);
}

int
scdel ( char *s )
{
	int	status = 0;

	status = status || !strcmp (s, "del");
	status = status || !strcmp (s, "DEL");
	status = status || !strcmp (s, "delete");
	status = status || !strcmp (s, "DELETE");
	return (status);
}

int
scins ( char *s )
{
	int	status = 0;

	status = status || !strcmp (s, "ins");
	status = status || !strcmp (s, "INS");
	status = status || !strcmp (s, "insert");
	status = status || !strcmp (s, "INSERT");
	return (status);
}

int
scnodel ( char *s, int *label )
{
	char	c;
	char	*pc, *lc = NODELABELS;
	char	*strchr();

	if (sscanf (s, "%c", &c) != 1)
	    return (SCER_SYNTAX);
	if (pc = strchr (lc, c))
	    *label = pc - lc;
	else
	    return (SCER_RANGE);
	return (SCER_OK);
}

int
scanodel ( char *s, int *label )
{
	char	c;
	char	*pc, *lc = ANODELABELS;
	char	*strchr();

	if (sscanf (s, "%c", &c) != 1)
	    return (SCER_SYNTAX);
	if (pc = strchr (lc, c))
	    *label = pc - lc;
	else
	    return (SCER_RANGE);
	return (SCER_OK);
}

int
scdntx ( char *s, DNTX *addr )
{
	int	a = 0;
	int	i, n = 4;
	char	c[4];
	int	d[4];
	static int	l[4] = {N_DCU, N_COUNTER, N_TIC, N_TTMP};
	static int	m[4] = {    0, N_COUNTER, N_TIC, N_TC};

	if (sscanf (s, "%c%c%c%c", &c[0], &c[1], &c[2], &c[3]) != n)
	    return (SCER_SYNTAX);
	
	for (i = 0; i < n; i++) {
	    if (isdigit (c[i]))
		d[i] = todigit (c[i]);
	    else
		return (SCER_SYNTAX);
	    if (d[i] < 0 || d[i] >= l[i])
		return (SCER_SYNTAX);
	    a *= m[i];
	    a += d[i];
	}
	*addr = a;
	return (SCER_OK);
}

int
scdsb ( char *s, DSB *addr )
{
	int	a = 0;
	int	i, n = 3;
	char	c[3];
	int	d[3];
	static int	l[3] = {N_DCU, N_SELECT, N_BUSS};
	static int	m[3] = {    0, N_SELECT, N_BUSS};

	if (sscanf (s, "%c%c%c", &c[0], &c[1], &c[2]) != n)
	    return (SCER_SYNTAX);
	
	for (i = 0; i < n; i++) {
	    if (isdigit (c[i]))
		d[i] = todigit (c[i]);
	    else
		return (SCER_SYNTAX);
	    if (d[i] < 0 || d[i] >= l[i])
		return (SCER_SYNTAX);
	    a *= m[i];
	    a += d[i];
	}
	*addr = a;
	return (SCER_OK);
}

int
scpfe ( char *s, PFE *addr )
{
	int	a = 0;
	int	i, n = 3;
	char	c[3];
	int	d[3];
	static int 	l[3] = {N_PANEL, N_FASE, N_ELEMENT};
	static int 	m[3] = {      0, N_FASE, N_ELEMENT};

	if (sscanf (s, "%c%c%c", &c[0], &c[1], &c[2]) != n)
	    return (SCER_SYNTAX);
	
	for (i = 0; i < n; i++) {
	    if (isdigit (c[i]))
		d[i] = todigit (c[i]);
	    else
		return (SCER_SYNTAX);
	    if (d[i] < 0 || d[i] >= l[i])
		return (SCER_SYNTAX);
	    a *= m[i];
	    a += d[i];
	}
	*addr = a;
	return (SCER_OK);
}

int
sczone ( char *s, int *addr )
{
	char	c;
	int	d;

	if (sscanf (s, "%c", &c) != 1)
	    return (SCER_SYNTAX);
	else
	    c = toupper (c);
	switch (c) {
	case 'R':
	    *addr = ROT_ZONE;
	    break;
	case 'Z':
	    if (sscanf (s+1, "%d", &d) != 1)
		return (SCER_SYNTAX);
	    if (d < 0 || d >= N_ZONE)
		return (SCER_SYNTAX);
	    else
		*addr = d;
	    break;
	default:
	    return (SCER_SYNTAX);
	}
	return (SCER_OK);
}

int
sczonea ( char *s, int *addr )
{
	char	c;
	int	d;

	if (sscanf (s, "%c", &c) != 1)
	    return (SCER_SYNTAX);
	else
	    c = toupper (c);
	switch (c) {
	case 'A':
	    *addr = ALL_ZONE;
	    break;
	case 'R':
	    *addr = ROT_ZONE;
	    break;
	case 'Z':
	    if (sscanf (s+1, "%d", &d) != 1)
		return (SCER_SYNTAX);
	    if (d < 0 || d >= N_ZONE)
		return (SCER_SYNTAX);
	    else
		*addr = d;
	    break;
	default:
	    return (SCER_SYNTAX);
	}
	return (SCER_OK);
}

int
sctctype ( char *s, int *addr )
{
	char	c;
	char	*cp;
	char	*strchr();
	char	*tctypes = "NKSTRJEZ";

	if (sscanf (s, "%c", &c) != 1)
	    return (SCER_SYNTAX);
	else
	    c = toupper (c);
	if (cp = strchr (tctypes, c))
	    *addr = cp-tctypes;
	else
	    return (SCER_SYNTAX);
	return (SCER_OK);
}

/* THE END */
