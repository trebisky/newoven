#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
tiido (s)
char	*s;
{
	int	t = 0;

	s += prdnt0 (s, Gdntx);
	s += prtemp (s, Dtic->jtmp);
	return (t);
}

int
tiado (s)
char	*s;
{
	int	t = 0;

	prdnt0 (s, Gdntx);
	return (t);
}

int
tistt ()
{
	int	t = 0;

	t = toggle (&Ptic->down);
	return (t);
}

int
tisto (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Ptic->down);
	return (t);
}

int
tijto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dtic->jtmp);
	return (t);
}

int
tiatg ()
{
	int	t = 0;
	DNTX	dntx = adjtic (Pdb, Gdntx);

	if (dntx >= N_DCU*N_COUNTER*N_TIC*N_TC)
	    return (GOER_NOWHERE);
	t = menugoto ("ti");
	if (!t)
	    Gdntx = dntx;
	return (t);
}

int
tiato (s)
char	*s;
{
	int	t = 0;

	prtemp (s, adjtmp (Pdb, Ddb, Gdntx));
	return (t);
}

int
tisni (s)
char	*s;
{
	int	t = 0;

	t = scserial (s, &Ptic->serial);
	return (t);
}

int
tisno (s)
char	*s;
{
	int	t = 0;

	prserial (s, Ptic->serial);
	return (t);
}

int
tirvi (s)
char	*s;
{
	int	t = 0;

	t = screfv (s, &Ptic->reference);
	return (t);
}

int
tirvo (s)
char	*s;
{
	int	t = 0;

	prrefv (s, Ptic->reference);
	return (t);
}

int
tiiji (s)
char	*s;
{
	int	t = 0;

	t = scijb (s, &Ptic->ijblock);
	return (t);
}

int
tiijo (s)
char	*s;
{
	int	t = 0;

	prijb (s, Ptic->ijblock);
	return (t);
}

int
tityg ()
{
	int	t = 0;

	t = menugoto ("ty");
	if (!t)
	    Gtctype = Ptic->tctype;
	return (t);
}

int
tityi (s)
char	*s;
{
	int	t = 0;

	t = sctctype (s, &Ptic->tctype);
	return (t);
}

int
tityo (s)
char	*s;
{
	int	t = 0;

	prtctype (s, Ptic->tctype);
	return (t);
}

int
tisag ()
{
	int	t = 0;

	t = menugoto ("sb");
	if (!t)
	    Gdsb = Ptic->select;
	return (t);
}

int
tisai (s)
char	*s;
{
	int	t = 0;

	t = scdsb (s, &Ptic->select);
	return (t);
}

int
tisao (s)
char	*s;
{
	int	t = 0;

	prdsb (s, Ptic->select);
	return (t);
}

int
tissg ()
{
	int	t = 0;

	t = menugoto ("sb");
	if (!t)
	    Gdsb = Ptic->select;
	return (t);
}

int
tisso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pbusst->down);
	return (t);
}

int
tidag ()
{
	int	t = 0;

	t = menugoto ("dc");
	return (t);
}

int
tidao (s)
char	*s;
{
	int	t = 0;

	prd000 (s, Gdntx);
	return (t);
}

int
tidsg ()
{
	int	t = 0;

	t = menugoto ("dc");
	return (t);
}

int
tidso (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pdcu->down);
	return (t);
}

int
titcg (n)
int	n;
{
	int	t = 0;

	t = menugoto ("tc");
	if (!t)
	    Gdntx = Gdntx/N_TC*N_TC+n;
	return (t);
}

int
titct (n)
int	n;
{
	int	t = 0;
	DNTX	dntx = Gdntx/N_TC*N_TC+n;
	p_tc	*ptc = ptc_dntx (Pdb, dntx);

	t = toggle (&ptc->down);
	return (t);
}

int
titco (n, s)
int	n;
char	*s;
{
	int	t = 0;
	DNTX	dntx = Gdntx/N_TC*N_TC+n;
	p_tc	*ptc = ptc_dntx (Pdb, dntx);
	d_tc	*dtc = dtc_dntx (Ddb, dntx);

	*s++ = '[';
	s += prloc (s, &ptc->loc);
	*s++ = ']';
	*s++ = '(';
	s += prdntx (s, dntx);
	*s++ = ')';
	*s++ = ' ';
	s += prstat (s, ptc->down);
	s += prign (s, ptc->ignore);
	s += prtemp (s, dtc->ttmp);
	return (t);
}

int
tint ()
{
	int	n = 0;

	n = N_TTMP;
	return (n);
}
