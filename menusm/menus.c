#include <stdlib.h>
#include "../menus.h"
#include "menus.ext"

/* menus - initialize the menus database
 * called from menus_init() in init.c
 */
MENUS *
menus ( void )
{
	MENUS	*msp;
	MENU	*mp;
	ITEM	*ip;
	// char	*calloc();

	if ((msp = (MENUS *)calloc (1, sizeof(MENUS))) == (MENUS *)0)
	    return (msp);

#include "menus.vc"

	return (msp);
}
