#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
msvct (n)
int	n;
{
	int	t = 0;

	t = toggle (&Pdb->misc.vup[n]);
	return (t);
}

int
msvco (n, s)
int	n;
char	*s;
{
	int	t = 0;

	s += prvnumb (s, n+1);
	s += prspace (s, 1);
	s += prenable (s, Pdb->misc.vup[n]);
	return (t);
}

int
msvn ()
{
	int	n = 0;

	n = N_COMP;
	return (n);
}

int
msjti (s)
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Pdb->misc.djtmp);
	return (t);
}

int
msjto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Pdb->misc.djtmp);
	return (t);
}

int
msxjt ()
{
	int	t = 0;

	t = toggle (&Pdb->misc.xjtmp);
	return (t);
}

int
msxjo (s)
char	*s;
{
	int	t = 0;

	prenable (s, Pdb->misc.xjtmp);
	return (t);
}

int
msxat ()
{
	int	t = 0;

	t = toggle (&Pdb->misc.xadc);
	return (t);
}

int
msxao (s)
char	*s;
{
	int	t = 0;

	prenable (s, Pdb->misc.xadc);
	return (t);
}

int
mscft ()
{
	int	t = 0;

	t = toggle (&Pdb->misc.config);
	return (t);
}

int
mscfo (s)
char	*s;
{
	int	t = 0;

	prenable (s, Pdb->misc.config);
	return (t);
}

int
mslrt ()
{
	int	t = 0;

	t = toggle (&Pdb->misc.logrotv);
	return (t);
}

int
mslro (s)
char	*s;
{
	int	t = 0;

	prenable (s, Pdb->misc.logrotv);
	return (t);
}

int
msrpi (n, s)
int	n;
char	*s;
{
	int	t = 0;

	t = scvportp (s, &Pdb->misc.rport[n]);
	return (t);
}

int
msrpo (n, s)
int	n;
char	*s;
{
	int	t = 0;

	s += prvnumb (s, n+1);
	s += prspace (s, 1);
	s += prvport (s, Pdb->misc.rport[n]);
	return (t);
}

int
msvr ()
{
	int	n = 0;

	n = N_COMP;
	return (n);
}
