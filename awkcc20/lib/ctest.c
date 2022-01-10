/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

#include <stdio.h>
#include "ear.h"
	
extern char	ctest[MAX_CHARS];
extern int	dflt;
	
clearctest(c)
char	c;
{
	register int	i;
	
	for (i=0; i<MAX_CHARS; i++)
		ctest[i]=0;
	/* newline is always a field separator
	 */
	ctest['\n']=1;
	ctest[c]=1;
	dflt=0;
}

setctest(c, tab)
char	c;
char	tab[];
{
	tab[c]=1;
}
