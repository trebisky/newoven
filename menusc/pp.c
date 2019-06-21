#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
ppido (s)
char	*s;
{
	int	t = 0;

	prp00 (s, Gpfe);
	return (t);
}

int
ppado (s)
char	*s;
{
	int	t = 0;

	prp00 (s, Gpfe);
	return (t);
}

int
pppst ()
{
	int	t = 0;

	t = toggle (&Ppanel->ss_down);
	return (t);
}

int
pppso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Ppanel->ss_down);
	return (t);
}

int
ppvni (s)
char	*s;
{
	int	t = 0;

	t = scvnumb (s, &Ppanel->connect);
	return (t);
}

int
ppvno (s)
char	*s;
{
	int	t = 0;

	prvnumb (s, Ppanel->connect);
	return (t);
}

int
ppvpi (s)
char	*s;
{
	int	t = 0;

	t = scvportp (s, &Ppanel->port);
	return (t);
}

int
ppvpo (s)
char	*s;
{
	int	t = 0;

	prvport (s, Ppanel->port);
	return (t);
}

int
pppag (n)
int	n;
{
	int	t = 0;

	t = menugoto ("ad");
	return (t);
}

int
pppao (n, s)
int	n;
char	*s;
{
	int	t = 0;
	PFE	pfe = Gpfe-Gpfe%(N_FASE*N_ELEMENT) + MIN(n,N_FASE-1)*N_ELEMENT;
	d_fase	*dfase;
	da_fase	*dafase;
	da_panel *dapanel;
	int	fase;
	float	amps = 0.0, load = 0.0, kwattsm = 0.0, kwattsc = 0.0;

	if (Gpfe/(N_FASE*N_ELEMENT) >= N_PANEL-1)
	    return (t);
	dfase = dfase_pfe (Ddb, pfe);
	dafase = dafase_pfe (Ddb, pfe);
	dapanel = dapanel_pfe (Ddb, pfe);
	if (n == N_FASE+1) {
	    for (fase = 0; fase < N_FASE; fase++) {
		PFE	pfe = Gpfe - Gpfe%(N_FASE*N_ELEMENT) + fase*N_ELEMENT;

		dfase = dfase_pfe (Ddb, pfe);
		dafase = dafase_pfe (Ddb, pfe);
		ADS (amps, dafase->Current);
		ADS (load, dfase->calc_load);
		ADS (kwattsm, KW (dafase->Voltage, dafase->Current));
		ADS (kwattsc, KW (dafase->Voltage, H2A (dfase->calc_load)));
	    }
	    s += prapt (s, pfe, amps, load, kwattsm, kwattsc);
	} else if (n == N_FASE) {
	    s += prapn (s, pfe, dapanel->Current);
	} else {
	    s += prapf (s,pfe,dafase->Voltage,dafase->Current,dfase->calc_load);
	}
	return (t);
}

int
ppna ()
{
	int	n = 0;

	n = N_FASE+2;		/* + neutral + total	*/
	return (n);
}
