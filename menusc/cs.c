#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
csoog ()
{
	int	t = 0;

	t = menugoto ("oo");
	return (t);
}

int
cstcg ()
{
	int	t = 0;

	t = menugoto ("tc");
	return (t);
}

int
cstci (s)
char	*s;
{
	int	t = 0;

	t = sctcloc (s, Pdb, &Gdntx);
	return (t);
}

int
cstco (s)
char	*s;
{
	int	t = 0;

	prloc (s, &Ptc->loc);
	return (t);
}

int
csheg ()
{
	int	t = 0;

	t = menugoto ("he");
	return (t);
}

int
cshei (s)
char	*s;
{
	int	t = 0;

	t = scheloc (s, Pdb, &Gpfe);
	return (t);
}

int
csheo (s)
char	*s;
{
	int	t = 0;

	prloc (s, &Phe->loc);
	return (t);
}

int
cszog ()
{
	int	t = 0;

	if (Gzone == ROT_ZONE)
	    t = menugoto ("rz");
	else
	    t = menugoto ("zo");
	return (t);
}

int
cszoi (s)
char	*s;
{
	int	t = 0;

	t = sczone (s, &Gzone);
	return (t);
}

int
cszoo (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

int
cstig ()
{
	int	t = 0;

	t = menugoto ("ti");
	return (t);
}

int
cstii (s)
char	*s;
{
	int	t = 0;

	t = scdntx (s, &Gdntx);
	return (t);
}

int
cstio (s)
char	*s;
{
	int	t = 0;

	prdntx (s, Gdntx);
	return (t);
}

int
csdcg ()
{
	int	t = 0;

	t = menugoto ("dc");
	return (t);
}

int
csdci (s)
char	*s;
{
	int	t = 0;

	t = scdntx (s, &Gdntx);
	return (t);
}

int
csdco (s)
char	*s;
{
	int	t = 0;

	prdntx (s, Gdntx);
	return (t);
}

int
cssbg ()
{
	int	t = 0;

	t = menugoto ("sb");
	return (t);
}

int
cssbi (s)
char	*s;
{
	int	t = 0;

	t = scdsb (s, &Gdsb);
	return (t);
}

int
cssbo (s)
char	*s;
{
	int	t = 0;

	prdsb (s, Gdsb);
	return (t);
}

int
cstyg ()
{
	int	t = 0;

	t = menugoto ("ty");
	return (t);
}

int
cstyi (s)
char	*s;
{
	int	t = 0;

	t = sctctype (s, &Gtctype);
	return (t);
}

int
cstyo (s)
char	*s;
{
	int	t = 0;

	prtctype (s, Gtctype);
	return (t);
}

int
csfsg ()
{
	int	t = 0;

	t = menugoto ("fs");
	return (t);
}

int
csfsi (s)
char	*s;
{
	int	t = 0;

	t = scpfe (s, &Gpfe);
	return (t);
}

int
csfso (s)
char	*s;
{
	int	t = 0;

	prpfe (s, Gpfe);
	return (t);
}

int
csppg ()
{
	int	t = 0;

	t = menugoto ("pp");
	return (t);
}

int
csppi (s)
char	*s;
{
	int	t = 0;

	t = scpfe (s, &Gpfe);
	return (t);
}

int
csppo (s)
char	*s;
{
	int	t = 0;

	prpfe (s, Gpfe);
	return (t);
}

int
csadg ()
{
	int	t = 0;

	t = menugoto ("ad");
	return (t);
}

int
csalg ()
{
	int	t = 0;

	t = menugoto ("al");
	return (t);
}

int
csjtg ()
{
	int	t = 0;

	t = menugoto ("jt");
	return (t);
}

int
csjti (s)
char	*s;
{
	int	t = 0;

	t = scdntx (s, &Gdntx);
	return (t);
}

int
csjto (s)
char	*s;
{
	int	t = 0;

	prdntx (s, Gdntx);
	return (t);
}

int
csttg ()
{
	int	t = 0;

	t = menugoto ("tt");
	return (t);
}

int
cstti (s)
char	*s;
{
	int	t = 0;

	t = scdntx (s, &Gdntx);
	return (t);
}

int
cstto (s)
char	*s;
{
	int	t = 0;

	prdntx (s, Gdntx);
	return (t);
}

int
cshtg ()
{
	int	t = 0;

	t = menugoto ("ht");
	return (t);
}

int
csztg ()
{
	int	t = 0;

	t = menugoto ("zt");
	return (t);
}

int
cszti (s)
char	*s;
{
	int	t = 0;

	t = sczone (s, &Gzone);
	return (t);
}

int
cszto (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

int
cshpg ()
{
	int	t = 0;

	t = menugoto ("hp");
	return (t);
}

int
cszpg ()
{
	int	t = 0;

	t = menugoto ("zp");
	return (t);
}

int
cszpi (s)
char	*s;
{
	int	t = 0;

	t = sczone (s, &Gzone);
	return (t);
}

int
cszpo (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

int
csglg ()
{
	int	t = 0;

	t = menugoto ("gl");
	return (t);
}

int
csmsg ()
{
	int	t = 0;

	t = menugoto ("ms");
	return (t);
}
