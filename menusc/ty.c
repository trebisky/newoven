#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
tyido (s)
char	*s;
{
	int	t = 0;

	prtctype (s, Gtctype);
	return (t);
}

int
tytyo (s)
char	*s;
{
	int	t = 0;

	prtctype (s, Gtctype);
	return (t);
}

int
tymti (n, s)
int	n;
char	*s;
{
	int	t = 0;

	scpoly (s, &Ptctype->mt_polycoef[n]);
	return (t);
}

int
tymto (n, s)
int	n;
char	*s;
{
	int	t = 0;

	prpoly (s, Ptctype->mt_polycoef[n]);
	return (t);
}

int
tynm ()
{
	int	n = 0;

	n = N_POLYCOEF;
	return (n);
}

int
tycvi (n, s)
int	n;
char	*s;
{
	int	t = 0;

	scpoly (s, &Ptctype->cv_polycoef[n]);
	return (t);
}

int
tycvo (n, s)
int	n;
char	*s;
{
	int	t = 0;

	prpoly (s, Ptctype->cv_polycoef[n]);
	return (t);
}

int
tync ()
{
	int	n = 0;

	n = N_POLYCOEF;
	return (n);
}

int
tyjti (n, s)
int	n;
char	*s;
{
	int	t = 0;

	scpoly (s, &Ptctype->jt_polycoef[n]);
	return (t);
}

int
tyjto (n, s)
int	n;
char	*s;
{
	int	t = 0;

	prpoly (s, Ptctype->jt_polycoef[n]);
	return (t);
}

int
tynj ()
{
	int	n = 0;

	n = N_POLYCOEF;
	return (n);
}
