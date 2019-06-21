#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
rzido (s)
char	*s;
{
	int	t = 0;

	s += przone (s, Gzone);
	*s++ = ' ';
	s += prclstat (s, Bzone->clock);
	*s++ = ' ';
	s += prtime (s, Bzone->time);
	*s++ = ' ';
	s += prsped (s, Dzone->etmp);
	return (t);
}

int
rzadg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rzado (s)
char	*s;
{
	int	t = 0;

	przone (s, Gzone);
	return (t);
}

int
rzstt ()
{
	int	t = 0;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = toggle (&Bzone->clock);
	return (t);
}

int
rzsto (s)
char	*s;
{
	int	t = 0;

	prclstatus (s, Bzone->clock);
	return (t);
}

int
rzcti (s)
char	*s;
{
	int	t = 0;

	if (!Geditb)
	    return (SCER_NOEDITB);
	t = sctime (s, &Bzone->time);
	return (t);
}

int
rzcto (s)
char	*s;
{
	int	t = 0;

	prtime (s, Bzone->time);
	return (t);
}

int
rzetg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rzeto (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->etmp);
	return (t);
}

int
rzzto (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->ztmp);
	return (t);
}

int
rzdto (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->zdiff);
	return (t);
}

int
rzerg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rzero (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->delta*3600.0);
	return (t);
}

int
rzzso (s)
char	*s;
{
	int	t = 0;

	prsped (s, Dzone->zsct);
	return (t);
}

int
rzedg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rzedo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prsped (s, Pnode->deviation);
	return (t);
}

int
rzsct ()
{
	int	t = 0;

	t = toggle (&Pzone->ck_down);
	return (t);
}

int
rzsco (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pzone->ck_down);
	return (t);
}

int
rzhrt ()
{
	int	t = 0;

	t = toggle (&Pzone->he_ck_down);
	return (t);
}

int
rzhro (s)
char	*s;
{
	int	t = 0;

	prstatus (s, Pzone->he_ck_down);
	return (t);
}

int
rzzpo (s)
char	*s;
{
	int	t = 0;

	prpower (s, Dzone->zpower);
	return (t);
}

int
rznng ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rznno (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnoden (s, Dzone->c_node);
	return (t);
}

int
rznlg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rznlo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnodel (s, Pnode->nlabel);
	return (t);
}

int
rznpg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rznpo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->p_coef);
	return (t);
}

int
rznig ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rznio (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->i_coef);
	return (t);
}

int
rzndg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rzndo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prcoef (s, Pnode->d_coef);
	return (t);
}

int
rznhg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rznho (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	pridepth (s, Pnode->i_depth);
	return (t);
}

int
rznqg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rznqo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prpower (s, Pnode->limit);
	return (t);
}

int
rznwg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rznwo (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prnodew (s, Pnode);
	return (t);
}

int
rzncg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rznco (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prsped (s, Pnode->temp);
	return (t);
}

int
rzngo (s)
char	*s;
{
	int	t = 0;

	prtime (s, Dzone->togo);
	return (t);
}

int
rzntg ()
{
	int	t = 0;

	t = menugoto ("rt");
	return (t);
}

int
rznto (s)
char	*s;
{
	int	t = 0;

	if (Pzone->n_node == 0)
	    return (t);
	prtime (s, Pnode->time);
	return (t);
}

int
rznro (s)
char	*s;
{
	int	t = 0;

	prrtime (s, Dzone->togo);
	return (t);
}

int
rzaso (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prsped (s, Ddb->adc[ncomp].rztmp);
	}
	return (t);
}

int
rza0o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[0]);
	}
	return (t);
}

int
rza1o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[1]);
	}
	return (t);
}

int
rza2o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[2]);
	}
	return (t);
}

int
rza3o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[3]);
	}
	return (t);
}

int
rza4o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[4]);
	}
	return (t);
}

int
rza5o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[5]);
	}
	return (t);
}

int
rza6o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[6]);
	}
	return (t);
}

int
rza7o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prvolts (s, Ddb->adc[ncomp].r[7]);
	}
	return (t);
}

int
rza8o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += pramps (s, Ddb->adc[ncomp].r[8]);
	}
	return (t);
}

int
rza9o (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prpercent (s, Ddb->adc[ncomp].r[9]);
	}
	return (t);
}

int
rzaao (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prpercent (s, Ddb->adc[ncomp].r[10]);
	}
	return (t);
}

int
rzabo (s)
char	*s;
{
	int	t = 0;
	int	ncomp;

	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prpercent (s, Ddb->adc[ncomp].r[11]);
	}
	return (t);
}

int
rzlvo (n, s)
int	n;
char	*s;
{
	int	t = 0;
	int	ncomp;

	s += prspace (s, 4);
	s += prnnb (s, n);
	for (ncomp = 0; ncomp < N_COMP; ncomp++) {
	    s += prspace (s, 3);
	    s += prtemp (s, Ddb->adc[ncomp].r[O_LVDT+n]);
	}

	return (t);
}

int
rzla ()
{
	int	n = 0;

	n = N_LVDT;
	return (n);
}

int
rzloi (n, s)
int	n;
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Pdb->misc.lvdt[n]);
	return (t);
}

int
rzloo (n, s)
int	n;
char	*s;
{
	int	t = 0;

	s += prspace (s, 4);
	s += prnnb (s, n);
	s += prspace (s, 3);
	s += prtemp (s, Pdb->misc.lvdt[n]);
	return (t);
}

int
rzlb ()
{
	int	n = 0;

	n = rzla ();
	return (n);
}
