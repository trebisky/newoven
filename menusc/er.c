#include "../oven.h"
#include "../menus.h"
#include "../context.h"
#include "../global.h"
#include "../protos.h"

void elogline ( int, char * );
int eloggoto ( char * );
int nelogline ( void );

int
erelg (n)
int	n;
{
	int	t = 0;
	char	s[80];

	elogline (n, s);
	t = eloggoto (s);
	return (t);
}

int
erelo (n, s)
int	n;
char	*s;
{
	int	t = 0;

	elogline (n, s);
	return (t);
}

int
ernl ()
{
	int	n = 0;

	n = nelogline () + nmline ();
	return (n);
}
