#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
dcido (s)
char	*s;
{
	int	t = 0;

	prd000 (s, Gdntx);
	return (t);
}

int
dcado (s)
char	*s;
{
	int	t = 0;

	prd000 (s, Gdntx);
	return (t);
}

int
dcstt ()
{
	int	t = 0;

	t = toggle (&Pdcu->down);
	return (t);
}

int
dcsto (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pdcu->down);
	return (t);
}

int
dcvni (s)
char	*s;
{
	int	t = 0;

	t = scvnumb (s, &Pdcu->connect);
	return (t);
}

int
dcvno (s)
char	*s;
{
	int	t = 0;

	prvnumb (s, Pdcu->connect);
	return (t);
}

int
dcvpi (s)
char	*s;
{
	int	t = 0;

	t = scvportd (s, &Pdcu->port);
	return (t);
}

int
dcvpo (s)
char	*s;
{
	int	t = 0;

	prvport (s, Pdcu->port);
	return (t);
}

int
dctig (n)
int	n;
{
	int	t = 0;

	t = menugoto ("ti");
	if (!t)
	    Gdntx = Gdntx - Gdntx%(N_COUNTER*N_TIC*N_TC) + n*N_TC;
	return (t);
}

int
dctit (n)
int	n;
{
	int	t = 0;
	DNTX	dntx = Gdntx - Gdntx%(N_COUNTER*N_TIC*N_TC) + n*N_TC;
	p_tic	*ptic = ptic_dntx (Pdb, dntx);

	t = toggle (&ptic->down);
	return (t);
}

int
dctio (n, s)
int	n;
char	*s;
{
	int	t = 0;
	DNTX	dntx = Gdntx - Gdntx%(N_COUNTER*N_TIC*N_TC) + n*N_TC;
	p_tic	*ptic = ptic_dntx (Pdb, dntx);
	d_tic	*dtic = dtic_dntx (Ddb, dntx);

	s += prdntx (s, dntx);
	*s++ = ' ';
	s += prstat (s, ptic->down);
	s += prtemp (s, dtic->jtmp);
	return (t);
}

int
dcnt ()
{
	int	n = 0;

	n = N_COUNTER*N_TIC;
	return (n);
}

int
dcsbg (n)
int	n;
{
	int	t = 0;

	t = menugoto ("sb");
	if (!t)
	    Gdsb = Gdntx/(N_COUNTER*N_TIC*N_TC)*(N_SELECT*N_BUSS) + n;
	return (t);
}

int
dcsbt (n)
int	n;
{
	int	t = 0;
	DSB	dsb = Gdntx/(N_COUNTER*N_TIC*N_TC)*(N_SELECT*N_BUSS) + n;
	p_buss	*pbuss = pbuss_dsb (Pdb, dsb);

	t = toggle (&pbuss->down);
	return (t);
}

int
dcsbo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	DSB	dsb = Gdntx/(N_COUNTER*N_TIC*N_TC)*(N_SELECT*N_BUSS) + n;
	p_buss	*pbuss = pbuss_dsb (Pdb, dsb);

	s += prdsb (s, dsb);
	*s++ = ' ';
	s += prstat (s, pbuss->down);
	return (t);
}

int
dcns ()
{
	int	n = 0;

	n = N_SELECT*N_BUSS;
	return (n);
}
