#include "oven.h"
#include "menus.h"
#include "context.h"
#include "global.h"

#include <stdlib.h>

/* push_context - push the menu context
 */
int
push_context ( void )
{
	CONTEXT	*ncp, *ocp = globalp->contextp;
	// char	*calloc ();

	ncp = (CONTEXT *)calloc (1, sizeof(CONTEXT));
	if ( ! ncp )
	    return (-1);

	ncp->prev = ocp;
	ocp->next = ncp;
	globalp->contextp = ncp;

	ncp->dntx = ocp->dntx;
	ncp->dsb  = ocp->dsb;
	ncp->pfe  = ocp->pfe;
	ncp->zone = ocp->zone;
	ncp->tctype = ocp->tctype;
	ncp->node = ocp->node;

	return (0);
}

/* pop_context - pop the menu context
 */
int
pop_context ( void )
{
	CONTEXT	*ncp, *ocp = globalp->contextp;

	ncp = ocp->prev;
	if ( ! ncp )
	    return (-1);

	free ((char *)ocp);

	ncp->next = 0; 
	globalp->contextp = ncp;

	return (0);
}
