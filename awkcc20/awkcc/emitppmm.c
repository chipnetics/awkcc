/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

#include <stdio.h>
#include "awkcc.h"

extern int	changedollar;

#define ARGORDER	(e->flags&FIXORD 		&& 	\
	    		 e->LEFT->t != INTVAR 		&& 	\
	    		 e->LEFT->t != FLOATVAR)

emitppmm(e)
Nodep	e;
{
	/*  This only necessary when used as an arg, and there
	 *  are more than one increments/decrements of the same
	 *  variable.
	 */
	if (ARGORDER)
		PFO("S(cat(tmpvar%d, 1, ", e->tempvar);
	switch(e->LEFT->t) {
	case INTVAR:
	case FLOATVAR:
		switch(e->w) {
		case YSTR:
			PFO("%sTOA(tmpstr%d,", 
				(e->LEFT->t==INTVAR?"I":"F"),e->tempstr);
			break;
		case YNUM:
			PFO("((double) ");
			break;
		case YINT:
			PFO("((int) ");
			e->LEFT->w=YNUM;
			break;
		default:
			break;
		}
		PFO("num_");
		tokname(5, e->t);
		e->LEFT->w=e->LEFT->y;
		femit(e->LEFT, ")");
		switch(e->w) {
		case YSTR: case YNUM: case YINT:
			PFO(")");
			break;
		}
		break;
	case INDIRECT:
		if (CHDOLL(e->LEFT->LEFT)) {
			changedollar=1;
			PFO("((pristine=0), ");
		}
	default:
		switch(e->w) {
		case YINT:
			PFO("((int) ");
			tokname(2, e->t);
			break;
		case YNUM:
			PFO("((double) ");
			tokname(2, e->t);
			break;
		case YSTR:
			tokname(3, e->t);
			break;
		default:
			tokname(2, e->t);
			break;
		}
		e->LEFT->w=YUNK;
		femit(e->LEFT, ")");
		if (e->w==YINT || e->w==YNUM) 
			PFO(")");
		if (e->LEFT->t==INDIRECT) {
			if (CHDOLL(e->LEFT->LEFT)) {
				changedollar=0;
				PFO(")");
			}
		}
		break;
	}
	if (ARGORDER)
		PFO("))");
}
