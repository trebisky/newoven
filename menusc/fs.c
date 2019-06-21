#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
fsido (s)
char	*s;
{
	int	t = 0;

	prpf0 (s, Gpfe);
	return (t);
}

int
fsado (s)
char	*s;
{
	int	t = 0;

	prpf0 (s, Gpfe);
	return (t);
}

int
fsfei (s)
char	*s;
{
	int	t = 0;

	t = scepsilon (s, &Pfase->epsilon);
	return (t);
}

int
fsfeo (s)
char	*s;
{
	int	t = 0;

	prepsilon (s, Pfase->epsilon);
	return (t);
}

int
fsfmi (s)
char	*s;
{
	int	t = 0;

	t = scmaxload (s, &Pfase->max_load);
	return (t);
}

int
fsfmo (s)
char	*s;
{
	int	t = 0;

	prmaxload (s, Pfase->max_load);
	return (t);
}

int
fspsg ()
{
	int	t = 0;

	t = menugoto ("pp");
	return (t);
}

int
fspso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Ppanel->ss_down);
	return (t);
}

int
fsfag ()
{
	int	t = 0;

	t = menugoto ("ad");
	return (t);
}

int
fsfao (s)
char	*s;
{
	int	t = 0;
	da_fase	*dafase;

	if (Gpfe/(N_FASE*N_ELEMENT) >= N_PANEL-1)
	    return (t);
	dafase = dafase_pfe (Ddb, Gpfe);
	prapf (s, Gpfe, dafase->Voltage, dafase->Current, Dfase->calc_load);
	return (t);
}

int
fsheg (n)
int	n;
{
	int	t = 0;

	t = menugoto ("he");
	if (!t)
	    Gpfe = Gpfe - Gpfe%N_ELEMENT + n;
	return (t);
}

int
fshet (n)
int	n;
{
	int	t = 0;
	PFE	pfe = Gpfe - Gpfe%N_ELEMENT + n;
	p_element	*phe = phe_pfe (Pdb, pfe);

	t = toggle (&phe->he_down);
	return (t);
}

int
fsheo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	PFE	pfe = Gpfe - Gpfe%N_ELEMENT + n;
	p_element	*phe = phe_pfe (Pdb, pfe);
	d_element	*dhe = dhe_pfe (Ddb, pfe);

	*s++ = '[';
	s += prloc (s, &phe->loc);
	*s++ = ']';
	*s++ = '(';
	s += prpfe (s, pfe);
	*s++ = ')';
	*s++ = ' ';
	s += prstat (s, phe->he_down);
	s += prign (s, phe->ignore);
	s += prtemp (s, dhe->htmp);
	*s++ = ' ';
	s += prpower (s, dhe->heat);
	return (t);
}

int
fsne ()
{
	int	n = 0;

	n = N_ELEMENT;
	return (n);
}
