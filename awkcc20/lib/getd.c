/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

#include <stdio.h>
#include "ear.h"

extern int	ND;
extern int	NF;
extern VARP	*D;
extern VARP	mD0();
extern int	pristine;
extern void	assign_NF();
extern VARP	nullfld;
extern VARP	m_var();
extern STRP	m_str();

VARP	
getd(j, setnf)
register int	j;
int	setnf;
{
	register int	i;

extern VARP	tmpvar;
	if (j>=1 && j<=NF) return(D[j]);
	if (!j) return (pristine?D[0]:mD0());
	if (j<0) ER "Illegal field $(%d)", j SX;
	if (!setnf) {
#ifdef DEBUG
		if (S(nullfld)[0]	||
		    nullfld->num	||
		    (nullfld->cur != (STRFLD|DIDFLD|NUMFLD))) {
			print_var("NLFD", nullfld);
			ER "Corrupt nullfld" SX
		}
#endif
		return (nullfld);
	}
	if (j>NF) {
		for (i=NF+1; i < ND; i++) {
			D[i]->cur=STRFLD|DIDFLD;
			if (R(D[i])) {
				R(D[i])--;
				D[i]->string=m_str(1, "");
			} else *(S(D[i]))='\0';
		}
	}
	if (j>=ND) {
		D=(VARP *)realloc((char *)D, (j+1)*sizeof(VARP));
		SP(D, "getd");
		for (i=ND; i <= j; i++) {
			D[i]=m_var();
			D[i]->cur=STRFLD|DIDFLD;
		}
		ND=1+j;
	}
	pristine=0;
	NF=j;
	assign_NF();
	return(D[j]);
}


VARP	
riggetd(v, setnf, exi)
register VARP	v;
int	setnf;
int	*exi;
{
	register int	i;

	if (!DIDCHECK(v->cur))
		v->cur|=isitanum(S(v));
	if (ISNUM(v->cur)) {
		if (!NUMAVAIL(v->cur)) {
			v->num=atof(S(v));
			v->cur|=NUMFLD;
		}
		(*exi)=i=(int) v->num;
	} else ER "Illegal field $(%s)", S(v) SX;
	return(getd(i, setnf));
}
