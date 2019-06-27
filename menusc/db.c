#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

void databascw ( void );
void databascr ( void );

int
dbrfg ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_bpread_disk ();
	t = GOER_EXECUTED;
	return (t);
}

int
dbwfg ()
{
	int	t = 0;

	db_bpwrite_disk ();
	t = GOER_EXECUTED;
	return (t);
}

int
dbrog ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_bpread_oven ();
	t = GOER_EXECUTED;
	return (t);
}

int
dbwog ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_bpwrite_oven ();
	t = GOER_EXECUTED;
	return (t);
}

int
dbrdg ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_dread_oven ();
	t = GOER_EXECUTED;
	return (t);
}

int
dbreg ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	db_eread_oven ();
	t = GOER_EXECUTED;
	return (t);
}

int
dbwtg ()
{
	int	t = 0;

	databascw ();
	t = GOER_EXECUTED;
	return (t);
}

int
dbrtg ()
{
	int	t = 0;

	if (globalp->readonly)
	    return (GOER_NOEDITP);
	databascr ();
	t = GOER_EXECUTED;
	return (t);
}
