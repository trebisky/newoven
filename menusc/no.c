#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
noido (s)
char	*s;
{
	int	t = 0;

	s += przone (s, Gzone);
	s += prspace (s, 8);
	s += prtemp (s, Dzone->etmp);
	*s++ = ' ';
	s += prtime (s, Bzone->time);
	*s++ = ' ';
	s += prclstat (s, Bzone->clock);
	return (t);
}

int
nonli (s)
char	*s;
{
	int	t = 0;

	t = scnodel (s, &Pnoden->nlabel);
	return (t);
}

int
nonlo (s)
char	*s;
{
	int	t = 0;

	s += prnodel (s, Pnoden->nlabel);
	*s++ = Dzone->c_node == Gnode ? '*' : ' ';
	*s++ = 0;
	return (t);
}

int
norti (s)
char	*s;
{
	int	t = 0;
	int	n;
	TEMP	rate;
	TIME	dtime;
	TIME	ddtime = 0;
	TIME	newtime;

	t = sctemp (s, &rate);
	if (!t && Gnode) {
	    rate /= 3600.0;
	    if (ABS(rate) < 0.001/3600.0) {
	        Pnoden->temp = Pnodem->temp;
	    } else {
	        dtime = (Pnoden->temp - Pnodem->temp) / rate;
		dtime = (dtime+30)/60*60; /* normalize to integral minutes */
		if (dtime) {
	            ddtime = dtime - (Pnoden->time - Pnodem->time);
		    if (ddtime) {
		        for (n = Gnode; n < Pzone->n_node; n++) {
			    Pnodex(n)->time += ddtime;
			}

			/* update the zone clock when changing the rate
			 * on the current node.
			 */
	                if ((Pnoden->time != Pnodem->time) &&
			    (Bzone ->time >= Pnodem->time) &&
			    (Bzone ->time <= Pnoden->time) &&
	                    (Pnoden->temp != Pnodem->temp) &&
			    (Dzone ->etmp >= Pnodem->temp) &&
			    (Dzone ->etmp <= Pnoden->temp)) {
			    newtime = ((TIME)
				 ((float)(Pnoden->time - Pnodem->time)
				/ (float)(Pnoden->temp - Pnodem->temp)
			        * (float)(Dzone ->etmp - Pnodem->temp)
				+ Pnodem->time)+30)/60*60;
			    if (Bzone->time != newtime)
				if (Geditb) {
				    Bzone->time = newtime;
				} else {
				    for (n = Gnode; n < Pzone->n_node; n++)
					Pnodex(n)->time -= ddtime;
				    t = SCER_NOEDITB;
				}
			}
		    }
		}
	    }
	}
	return (t);
}

int
norto (s)
char	*s;
{
	int	t = 0;
	TEMP	dtemp;
	TIME	dtime;

	dtemp = Pnoden->temp - Pnodem->temp;
	dtime = Pnoden->time - Pnodem->time;
	s += prtemp (s, dtime ? (float)dtemp/(float)dtime*3600.0 : 0.0);
	return (t);
}

int
nodvi (s)
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Pnoden->deviation);
	return (t);
}

int
nodvo (s)
char	*s;
{
	int	t = 0;

	s += prtemp (s, Pnoden->deviation);
	return (t);
}

int
notdi (s)
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Pnoden->tdeviation);
	return (t);
}

int
notdo (s)
char	*s;
{
	int	t = 0;

	s += prtemp (s, Pnoden->tdeviation);
	return (t);
}

int
nogti (s)
char	*s;
{
	int	t = 0;

	t = sctemp (s, &Pnoden->temp);
	return (t);
}

int
nogto (s)
char	*s;
{
	int	t = 0;

	s += prtemp (s, Pnoden->temp);
	return (t);
}

int
nogci (s)
char	*s;
{
	int	t = 0;

	t = sctime (s, &Pnoden->time);
	return (t);
}

int
nogco (s)
char	*s;
{
	int	t = 0;

	s += prtime (s, Pnoden->time);
	return (t);
}

int
nodti (s)
char	*s;
{
	int	t = 0;
	TEMP	dtemp;

	t = sctemp (s, &dtemp);
	if (!t && Gnode)
	    Pnoden->temp = Pnodem->temp + dtemp;
	return (t);
}

int
nodto (s)
char	*s;
{
	int	t = 0;

	s += prtemp (s, Pnoden->temp - Pnodem->temp);
	return (t);
}

int
nodci (s)
char	*s;
{
	int	t = 0;
	int	n;
	TIME	dtime;
	TIME	ddtime = 0;

	t = sctime (s, &dtime);
	if (!t && Gnode)
	    ddtime = dtime - (Pnoden->time - Pnodem->time);
	if (ddtime)
	    for (n = Gnode; n < Pzone->n_node; n++)
		Pnodex(n)->time += ddtime;
	return (t);
}

int
nodco (s)
char	*s;
{
	int	t = 0;

	s += prtime (s, Pnoden->time - Pnodem->time);
	return (t);
}

int
nowtt ()
{
	int	t = 0;

	t = toggleb (&Pnoden->wflag, WFLAG_T);
	return (t);
}

int
nowto (s)
char	*s;
{
	int	t = 0;

	prstatusb (s, Pnoden->wflag, WFLAG_T);
	return (t);
}

int
nowdt ()
{
	int	t = 0;

	t = toggleb (&Pnoden->wflag, WFLAG_D);
	return (t);
}

int
nowdo (s)
char	*s;
{
	int	t = 0;

	prstatusb (s, Pnoden->wflag, WFLAG_D);
	return (t);
}

int
nowwt ()
{
	int	t = 0;

	t = toggleb (&Pnoden->wflag, WFLAG_W);
	return (t);
}

int
nowwo (s)
char	*s;
{
	int	t = 0;

	prstatusb (s, Pnoden->wflag, WFLAG_W);
	return (t);
}

int
nowzi (s)
char	*s;
{
	int	t = 0;

	t = sczonea (s, &Pnoden->wzone);
	return (t);
}

int
nowzo (s)
char	*s;
{
	int	t = 0;

	if (Pnoden->wflag & WFLAG_W)
	    przone (s, Pnoden->wzone);
	return (t);
}

int
nowni (s)
char	*s;
{
	int	t = 0;

	t = scnodel (s, &Pnoden->wlabel);
	return (t);
}

int
nowno (s)
char	*s;
{
	int	t = 0;

	if (Pnoden->wflag & WFLAG_W)
	    prnodel (s, Pnoden->wlabel);
	return (t);
}

int
nonpi (s)
char	*s;
{
	int	t = 0;

	t = sccoef (s, &Pnoden->p_coef);
	return (t);
}

int
nonpo (s)
char	*s;
{
	int	t = 0;

	s += prcoef (s, Pnoden->p_coef);
	return (t);
}

int
nonii (s)
char	*s;
{
	int	t = 0;

	t = sccoef (s, &Pnoden->i_coef);
	return (t);
}

int
nonio (s)
char	*s;
{
	int	t = 0;

	s += prcoef (s, Pnoden->i_coef);
	return (t);
}

int
nondi (s)
char	*s;
{
	int	t = 0;

	t = sccoef (s, &Pnoden->d_coef);
	return (t);
}

int
nondo (s)
char	*s;
{
	int	t = 0;

	s += prcoef (s, Pnoden->d_coef);
	return (t);
}

int
nonhi (s)
char	*s;
{
	int	t = 0;

	t = scidepth (s, &Pnoden->i_depth);
	return (t);
}

int
nonho (s)
char	*s;
{
	int	t = 0;

	s += pridepth (s, Pnoden->i_depth);
	return (t);
}

int
nonqi (s)
char	*s;
{
	int	t = 0;

	t = scpower (s, &Pnoden->limit);
	return (t);
}

int
nonqo (s)
char	*s;
{
	int	t = 0;

	s += prpower (s, Pnoden->limit);
	return (t);
}
