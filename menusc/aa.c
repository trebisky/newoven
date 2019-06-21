#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

int
aaexg ( void )
{
	int	t = 0;

	Gmenu = -1;
	return (t);
}

int
aacsg ( void )
{
	int	t = 0;

	t = menugoto ("cs");
	return (t);
}

int
aaerg ( void )
{
	int	t = 0;

	t = menugoto ("er");
	return (t);
}

int
aadbg ( void )
{
	int	t = 0;

	t = menugoto ("db");
	return (t);
}
