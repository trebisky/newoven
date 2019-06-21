#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
zoido (s)
char	*s;
{
	int	t = 0;

	s += przone (s, Gzone);
	*s++ = ' ';
	s += prclstat (s, Bzone->clock);
	*s++ = ' ';
	s += prtime (s, Bzone->time);
	*s++ = ' ';
	s += prtemp (s, Dzone->etmp);
	return (t);
}

int
zoadg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zoado (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

int
zostt ()
{
	int	t = 0;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = toggle (&Bzone->clock);
	return (t);
}

int
zosto (s)
char	*s;
{
	int	t = 0;

	prclstatus (s, Bzone->clock);
	return (t);
}

int
zocti (s)
char	*s;
{
	int	t = 0;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = sctime (s, &Bzone->time);
	return (t);
}

int
zocto (s)
char	*s;
{
	int	t = 0;

	prtime (s, Bzone->time);
	return (t);
}

int
zoetg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zoeto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->etmp);
	return (t);
}

int
zozto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->ztmp);
	return (t);
}

int
zodto (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->zdiff);
	return (t);
}

int
zoerg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zoero (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->delta*3600.0);
	return (t);
}

int
zozso (s)
char	*s;
{
	int	t = 0;

	prtemp (s, Dzone->zsct);
	return (t);
}

int
zoedg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zoedo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtemp (s, Pnode->deviation);
	return (t);
}

int
zotdg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zotdo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtemp (s, Pnode->tdeviation);
	return (t);
}

int
zosct ()
{
	int	t = 0;

	t = toggle (&Pzone->ck_down);
	return (t);
}

int
zosco (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pzone->ck_down);
	return (t);
}

int
zohrt ()
{
	int	t = 0;

	t = toggle (&Pzone->he_ck_down);
	return (t);
}

int
zohro (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pzone->he_ck_down);
	return (t);
}

int
zozpo (s)
char	*s;
{
	int	t = 0;

	prpower (s, Dzone->zpower);
	return (t);
}

int
zonng ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zonno (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnoden (s, Dzone->c_node);
	return (t);
}

int
zonlg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zonlo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnodel (s, Pnode->nlabel);
	return (t);
}

int
zonpg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zonpo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->p_coef);
	return (t);
}

int
zonig ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zonio (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->i_coef);
	return (t);
}

int
zondg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zondo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->d_coef);
	return (t);
}

int
zonhg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zonho (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	pridepth (s, Pnode->i_depth);
	return (t);
}

int
zonqg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zonqo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prpower (s, Pnode->limit);
	return (t);
}

int
zonwg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zonwo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnodew (s, Pnode);
	return (t);
}

int
zoncg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zonco (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtemp (s, Pnode->temp);
	return (t);
}

int
zongo (s)
char	*s;
{
	int	t = 0;

	prtime (s, Dzone->togo);
	return (t);
}

int
zontg ()
{
	int	t = 0;

	t = menugoto ("ts");
	return (t);
}

int
zonto (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtime (s, Pnode->time);
	return (t);
}

int
zonro (s)
char	*s;
{
	int	t = 0;

	prrtime (s, Dzone->togo);
	return (t);
}
