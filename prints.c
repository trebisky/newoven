#include <string.h>
#include <stdio.h>
#include <time.h>

#include "oven.h"
#include "protos.h"

int
prspace ( char *s, int n )
{
	int	i;

	for (i = 0; i < n; i++)
	    *s++ = ' ';
	*s = 0;
	return (n);
}

int
prstring ( char *s, char *string )
{
	strcpy (s, string);
	return (strlen (s));
}

int
prloc ( char *s, LOC *ploc )
{
	sprintf (s, "%3d %3d %3d", ploc->r, ploc->t, ploc->z);
	return (strlen (s));
}

int
prlocr ( char *s, LOC *ploc )
{
	sprintf (s, "%3d %3d", ploc->r1, ploc->r2);
	return (strlen (s));
}

int
prloct ( char *s, LOC *ploc )
{
	sprintf (s, "%3d %3d", ploc->t1, ploc->t2);
	return (strlen (s));
}

int
prlocz ( char *s, LOC *ploc )
{
	sprintf (s, "%3d %3d", ploc->z1, ploc->z2);
	return (strlen (s));
}

int
praloc ( char *s, LOC *ploc )
{
	sprintf (s, "%03d%03d%03d", ploc->r, ploc->t, ploc->z);
	return (strlen (s));
}

int
pralocr ( char *s, LOC *ploc )
{
	sprintf (s, "%03d%03d", ploc->r1, ploc->r2);
	return (strlen (s));
}

int
praloct ( char *s, LOC *ploc )
{
	sprintf (s, "%03d%03d", ploc->t1, ploc->t2);
	return (strlen (s));
}

int
pralocz ( char *s, LOC *ploc )
{
	sprintf (s, "%03d%03d", ploc->z1, ploc->z2);
	return (strlen (s));
}

int
prsped ( char *s, TEMP tmp )
{
	if (IS_INDEFT (tmp))
	    strcpy (s, "  INDEF");
	else
	    sprintf (s, "%7.3f", tmp);
	return (strlen (s));
}

int
prtemp ( char *s, TEMP tmp )
{
	if (IS_INDEFT (tmp))
	    strcpy (s, "  INDEF");
	else
	    sprintf (s, "%7.1f", tmp);
	return (strlen (s));
}

int
prtempd ( char *s, BOOL down, TEMP tmp )
{
	if (down)
	    strcpy (s, "    OFF");
	else if (IS_INDEFT (tmp))
	    strcpy (s, "  INDEF");
	else
	    sprintf (s, "%7.1f", tmp);
	return (strlen (s));
}

int
prenable ( char *s, BOOL enable )
{
	if (enable)
	    strcpy (s, "Enabled");
	else
	    strcpy (s, "Disabled");
	return (strlen (s));
}

int
prenab ( char *s, BOOL enable )
{
	if (enable)
	    strcpy (s, "E");
	else
	    strcpy (s, "D");
	return (strlen (s));
}

int
prstatus ( char *s, BOOL down )
{
	if (down)
	    strcpy (s, "Disabled");
	else
	    strcpy (s, "Enabled");
	return (strlen (s));
}

int
prstat ( char *s, BOOL down )
{
	if (down)
	    strcpy (s, "D");
	else
	    strcpy (s, "E");
	return (strlen (s));
}

int
prignore ( char *s, BOOL ignore )
{
	if (ignore)
	    strcpy (s, "Enabled");
	else
	    strcpy (s, "Disabled");
	return (strlen (s));
}

int
prign ( char *s, BOOL ignore )
{
	if (ignore)
	    strcpy (s, "I");
	else
	    strcpy (s, " ");
	return (strlen (s));
}

int
prdeviant ( char *s, BOOL deviant )
{
	if (deviant)
	    strcpy (s, "!");
	else
	    strcpy (s, " ");
	return (strlen (s));
}

int
prclstatus ( char *s, BOOL clock )
{
	if (clock)
	    strcpy (s, "Running");
	else
	    strcpy (s, "Stopped");
	return (strlen (s));
}

int
prclstat ( char *s, BOOL clock )
{
	if (clock)
	    strcpy (s, "R");
	else
	    strcpy (s, "S");
	return (strlen (s));
}

int
prtctype ( char *s, int tctype )
{
	switch (tctype) {
	case TCTYPEN:
	    strcpy (s, "N");
	    break;
	case TCTYPEK:
	    strcpy (s, "K");
	    break;
	case TCTYPES:
	    strcpy (s, "S");
	    break;
	case TCTYPET:
	    strcpy (s, "T");
	    break;
	case TCTYPER:
	    strcpy (s, "R");
	    break;
	case TCTYPEJ:
	    strcpy (s, "J");
	    break;
	case TCTYPEE:
	    strcpy (s, "E");
	    break;
	case TCTYPEZ:
	    strcpy (s, "Z");
	    break;
	default:
	    strcpy (s, "?");
	    break;
	}
	return (strlen (s));
}

int
prijb ( char *s, int ijblock )
{
	sprintf (s, "%06d", ijblock);
	return (strlen (s));
}

int
prserial ( char *s, int serial )
{
	sprintf (s, "%6d", serial);
	return (strlen (s));
}

int
prbeam ( char *s, int beam )
{
	sprintf (s, "%6d", beam);
	return (strlen (s));
}

int
prtwist ( char *s, int twist )
{
	sprintf (s, "%6d", twist);
	return (strlen (s));
}

int
pridepth ( char *s, int idepth )
{
	sprintf (s, "%3d", idepth);
	return (strlen (s));
}

int
prepsilon ( char *s, int epsilon )
{
	sprintf (s, "%6d", epsilon);
	return (strlen (s));
}

int
prmaxload ( char *s, int maxload )
{
	sprintf (s, "%6d", maxload);
	return (strlen (s));
}

int
prnnb ( char *s, int n )
{
	sprintf (s, "%6d", n);
	return (strlen (s));
}

int
prnoden ( char *s, int noden )
{
	sprintf (s, "%6d", noden);
	return (strlen (s));
}

int
prvnumb ( char *s, int vnumb )
{
	if (vnumb)
	    sprintf (s, "%3d", vnumb-1);
	else
	    strcpy (s, "OFF");
	return (strlen (s));
}

int
prvport ( char *s, int vport )
{
	sprintf (s, "%6d", vport);
	return (strlen (s));
}

int
prnodel ( char *s, int nodel )
{
	char	*labels = NODELABELS;

	s[0] = labels[nodel];
	s[1] = 0;
	return (strlen (s));
}

int
pranodel ( char *s, int nodel )
{
	char	*labels = ANODELABELS;

	s[0] = labels[nodel];
	s[1] = 0;
	return (strlen (s));
}

int
prnodew ( char *s, p_node *pnode )
{
	char	*t = s;

	*t++ = pnode->wflag & WFLAG_T ? 'T' : ' ';
	*t++ = pnode->wflag & WFLAG_D ? 'D' : ' ';
	*t++ = pnode->wflag & WFLAG_W ? 'W' : ' ';
	*t++ = ' ';
	t += pnode->wflag & WFLAG_W ? przone (t, pnode->wzone) : prspace (t, 3);
	*t++ = ' ';
	t += pnode->wflag & WFLAG_W ? prnodel(t, pnode->wlabel): prspace (t, 1);
	return (strlen (s));
}

int
prstatusb ( char *s, int flag, int mask )
{
	if (flag & mask)
	    strcpy (s, "Yes");
	else
	    strcpy (s, "No");
	return (strlen (s));
}

int
prtime ( char *s, TIME ss )
{
	TIME	mm, hh, dd;

	if (ss < 0) {
	    strcpy (s, "**:**:**");
	} else {
	    mm = ss/60;
	    hh = mm/60;	mm %= 60;
	    dd = hh/24;	hh %= 24;
	    sprintf (s, "%02d:%02d:%02d", dd, hh, mm);
	}
	return (strlen (s));
}

/* On a modern linux system, the time call returns an
 * 8 byte object, whereas our TIME is a 4 byte object.
 * Definitions in time.h
 */
int
prrtime ( char *s, TIME ss )
{
	// TIME	now;
	time_t now;

	// char	*ctime();
	// char	*strchr();

	time (&now);
	now += ss;
	strcpy (s, ctime (&now));

	if (strchr (s, '\n'))
	    *(strchr (s, '\n')) = 0;
	return (strlen (s));
}

int
prrefv ( char *s, float reference )
{
	sprintf (s, "%6.3f", reference);
	return (strlen (s));
}

int
prpoly ( char *s, float polycoef )
{
	sprintf (s, "%e", polycoef);
	return (strlen (s));
}

int
prcoef ( char *s, float coef )
{
	sprintf (s, "%13e", coef);
	return (strlen (s));
}

int
prprod ( char *s, float prod )
{
	sprintf (s, "%13e", prod);
	return (strlen (s));
}

int
prapf ( char *s, PFE pfe, float volts, float amps, float load )
{
	s += prpf0 (s, pfe);
	s += prvolts (s, volts);
	s += pramps (s, amps);
	*s++ = ' ';
	*s++ = '(';
	s += pramps (s, H2A (load));
	*s++ = ')';
	s += prheaters (s, A2H (amps));
	*s++ = ' ';
	*s++ = '(';
	s += prheaters (s, load);
	*s++ = ')';
	s += prkwatts (s, KW (volts, amps));
	*s++ = ' ';
	*s++ = '(';
	s += prkwatts (s, KW (volts, H2A (load)));
	*s++ = ')';
	*s++ = 0;
	return (strlen (s));
}

int
prapfv ( char *s, PFE pfe, float v, float volts )
{
	s += prpf0 (s, pfe);
	*s++ = ' ';
	s += prvolts (s, v);
	s += prvolts (s, volts);
	return (strlen (s));
}

int
prapfc ( char *s, PFE pfe, float a, float amps )
{
	s += prpf0 (s, pfe);
	*s++ = ' ';
	s += pramps (s, a);
	s += pramps (s, amps);
	return (strlen (s));
}

int
prapn ( char *s, PFE pfe, float amps )
{
	s += prp00 (s, pfe);
	*s++ = 'N';
	s += prspace (s, 6);
	s += pramps (s, amps);
	return (strlen (s));
}

int
prapnc ( char *s, PFE pfe, float a, float amps )
{
	s += prp00 (s, pfe);
	*s++ = 'N';
	s += pramps (s, a);
	s += pramps (s, amps);
	return (strlen (s));
}

int
prapt ( char *s, PFE pfe, float amps, float load, float kwattsm, float kwattsc )
{
	s += prp00 (s, pfe);
	*s++ = 'T';
	s += prspace (s, 6);
	s += pramps (s, amps);
	*s++ = ' ';
	*s++ = '(';
	s += pramps (s, H2A (load));
	*s++ = ')';
	s += prheaters (s, A2H (amps));
	*s++ = ' ';
	*s++ = '(';
	s += prheaters (s, load);
	*s++ = ')';
	s += prkwatts (s, kwattsm);
	*s++ = ' ';
	*s++ = '(';
	s += prkwatts (s, kwattsc);
	*s++ = ')';
	*s++ = 0;
	return (strlen (s));
}

int
prat ( char *s, float amps, float load, float kwattsm, float kwattsc )
{
	*s++ = 'T';
	*s++ = 'O';
	*s++ = 'T';
	s += prspace (s, 7);
	s += pramps (s, amps);
	*s++ = ' ';
	*s++ = '(';
	s += pramps (s, H2A (load));
	*s++ = ')';
	s += prheaters (s, A2H (amps));
	*s++ = ' ';
	*s++ = '(';
	s += prheaters (s, load);
	*s++ = ')';
	s += prkwatts (s, kwattsm);
	*s++ = ' ';
	*s++ = '(';
	s += prkwatts (s, kwattsc);
	*s++ = ')';
	*s++ = 0;
	return (strlen (s));
}

int
prah ( char *s, int n, float volts )
{
	*s++ = 'H';
	*s++ = 'K';
	*s++ = '0' + n;
	s += prvolts (s, volts);
	return (strlen (s));
}

int
prahv ( char *s, int n, float v, float volts )
{
	*s++ = 'H';
	*s++ = 'K';
	*s++ = '0' + n;
	*s++ = ' ';
	s += prvolts (s, v);
	s += prvolts (s, volts);
	return (strlen (s));
}

int
prpercent ( char *s, float value )
{
	if (IS_INDEFA (value))
	    strcpy (s, "  INDEF");
	else
	    sprintf (s, "%6.1f%%", value);
	return (strlen (s));
}

int
prvolts ( char *s, float value )
{
	if (IS_INDEFA (value))
	    strcpy (s, " INDEFV");
	else
	    sprintf (s, "%6.1fV", value);
	return (strlen (s));
}

int
pramps ( char *s, float value )
{
	if (IS_INDEFA (value))
	    strcpy (s, " INDEFA");
	else
	    sprintf (s, "%6.1fA", value);
	return (strlen (s));
}

int
prheaters ( char *s, float value )
{
	if (IS_INDEFA (value))
	    strcpy (s, " INDEF  HE");
	else
	    sprintf (s, "%8.3fHE", value);
	return (strlen (s));
}

int
prkwatts ( char *s, float value )
{
	if (IS_INDEFA (value))
	    strcpy (s, " INDEF  KW");
	else
	    sprintf (s, "%8.3fKW", value);
	return (strlen (s));
}

int
prpower ( char *s, POWER power )
{
	sprintf (s, "%3d", power);
	return (strlen (s));
}

int
prpowerd ( char *s, BOOL down, POWER power )
{
	if (down)
	    strcpy (s, "OFF");
	else
	    sprintf (s, "%3d", power);
	return (strlen (s));
}

int
prdntx ( char *s, DNTX addr )
{
	int	x = addr%N_TC;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	sprintf (s, "%d%d%d%d", d, n, t, x);
	return (strlen (s));
}

int
prdnt0 ( char *s, DNTX addr )
{
	int	x = 0;
	int	t = addr/N_TC%N_TIC;
	int	n = addr/N_TC/N_TIC%N_COUNTER;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	sprintf (s, "%d%d%d%d", d, n, t, x);
	return (strlen (s));
}

int
prd000 ( char *s, DNTX addr )
{
	int	x = 0;
	int	t = 0;
	int	n = 0;
	int	d = addr/N_TC/N_TIC/N_COUNTER%N_DCU;

	sprintf (s, "%d%d%d%d", d, n, t, x);
	return (strlen (s));
}

int
prdsb ( char *s, DSB addr )
{
	int	b = addr%N_BUSS;
	int	t = addr/N_BUSS%N_SELECT;
	int	d = addr/N_BUSS/N_SELECT%N_DCU;

	sprintf (s, "%d%d%d", d, t, b);
	return (strlen (s));
}

int
prpfe ( char *s, PFE addr )
{
	int	e = addr%N_ELEMENT;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	sprintf (s, "%d%d%d", p, f, e);
	return (strlen (s));
}

int
prpf0 ( char *s, PFE addr )
{
	int	e = 0;
	int	f = addr/N_ELEMENT%N_FASE;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	sprintf (s, "%d%d%d", p, f, e);
	return (strlen (s));
}

int
prp00 ( char *s, PFE addr )
{
	int	e = 0;
	int	f = 0;
	int	p = addr/N_ELEMENT/N_FASE%N_PANEL;

	sprintf (s, "%d%d%d", p, f, e);
	return (strlen (s));
}

int
przone ( char *s, int addr )
{
	switch (addr) {
	case ALL_ZONE:
	    strcpy (s, "A  ");
	    break;
	case ROT_ZONE:
	    strcpy (s, "R  ");
	    break;
	default:
	    if (addr < 10)
	        sprintf (s, "Z%d ", addr);
	    else
	        sprintf (s, "Z%d", addr);
	}
	return (strlen (s));
}

/* THE END */
