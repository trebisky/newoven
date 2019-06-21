#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
glstt ()
{
	int	t = 0;
	int	zone = 0;
	BOOL	clock;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = toggle (&Bzonex(zone)->clock);
	if (t)
	    return (t);
	clock = Bzonex(zone)->clock;
	for (zone = 0; zone < N_ZONE; zone++)
	    Bzonex(zone)->clock = clock;
	return (t);
}

int
glsto (s)
char	*s;
{
	int	t = 0;
	int	zone = 0;

	prclstatus (s, Bzonex(zone)->clock);
	return (t);
}

int
glcti (s)
char	*s;
{
	int	t = 0;
	int	zone = 0;
	TIME	time;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = sctime (s, &Bzonex(zone)->time);
	if (t)
	    return (t);
	time = Bzonex(zone)->time;
	for (zone = 0; zone < N_ZONE; zone++)
	    Bzonex(zone)->time = time;
	return (t);
}

int
glcto (s)
char	*s;
{
	int	t = 0;
	int	zone = 0;

	prtime (s, Bzonex(zone)->time);
	return (t);
}

int
glsct ()
{
	int	t = 0;
	int	zone = 0;
	BOOL	ck_down;

	t = toggle (&Pzonex(zone)->ck_down);
	if (t)
	    return (t);
	ck_down = Pzonex(zone)->ck_down;
	for (zone = 0; zone < N_ZONE; zone++)
	    Pzonex(zone)->ck_down = ck_down;
	return (t);
}

int
glsco (s)
char	*s;
{
	int	t = 0;
	int	zone = 0;

	prstatus (s, Pzonex(zone)->ck_down);
	return (t);
}

int
glhrt ()
{
	int	t = 0;
	int	zone = 0;
	BOOL	he_ck_down;

	t = toggle (&Pzonex(zone)->he_ck_down);
	if (t)
	    return (t);
	he_ck_down = Pzonex(zone)->he_ck_down;
	for (zone = 0; zone < N_ZONE; zone++)
	    Pzonex(zone)->he_ck_down = he_ck_down;
	return (t);
}

int
glhro (s)
char	*s;
{
	int	t = 0;
	int	zone = 0;

	prstatus (s, Pzonex(zone)->he_ck_down);
	return (t);
}

int
glpst ()
{
	int	t = 0;
	PFE	pfe = 0;
	BOOL	ss_down;

	t = toggle (&Ppanelx(pfe)->ss_down);
	if (t)
	    return (t);
	ss_down = Ppanelx(pfe)->ss_down;
	for (pfe = 0; pfe < N_PANEL*N_FASE*N_ELEMENT; pfe += N_FASE*N_ELEMENT)
	    Ppanelx(pfe)->ss_down = ss_down;
	return (t);
}

int
glpso (s)
char	*s;
{
	int	t = 0;
	PFE	pfe = 0;

	prstatus (s, Ppanelx(pfe)->ss_down);
	return (t);
}

int
glfmi (s)
char	*s;
{
	int	t = 0;
	PFE	pfe = 0;
	int	max_load;

	t = scmaxload (s, &Pfasex(pfe)->max_load);
	if (t)
	    return (t);
	max_load = Pfasex(pfe)->max_load;
	for (pfe = 0; pfe < N_PANEL*N_FASE*N_ELEMENT; pfe += N_ELEMENT)
	    Pfasex(pfe)->max_load = max_load;
	return (t);
}

int
glfmo (s)
char	*s;
{
	int	t = 0;
	PFE	pfe = 0;

	prmaxload (s, Pfasex(pfe)->max_load);
	return (t);
}

int
glfei (s)
char	*s;
{
	int	t = 0;
	PFE	pfe = 0;
	int	epsilon;

	t = scepsilon (s, &Pfasex(pfe)->epsilon);
	if (t)
	    return (t);
	epsilon = Pfasex(pfe)->epsilon;
	for (pfe = 0; pfe < N_PANEL*N_FASE*N_ELEMENT; pfe += N_ELEMENT)
	    Pfasex(pfe)->epsilon = epsilon;
	return (t);
}

int
glfeo (s)
char	*s;
{
	int	t = 0;
	PFE	pfe = 0;

	prepsilon (s, Pfasex(pfe)->epsilon);
	return (t);
}
