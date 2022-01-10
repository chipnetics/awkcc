/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

#include <stdio.h>
#include "ear.h"
#include "awk.h"

extern char	*patbeg;
extern int	patlen;
	
extern char	ctest[MAX_CHARS];
	
/* -2 means that's all we need. 
 * -1 means get everything else
 * 0 means count NF, but don't insert
 */
	
extern int	upf[];

extern int	rsdflt;	
extern int	ND;
extern int	NF;
extern int	NC;
extern int	pristine;
extern char	*STRFS;
	
extern VARP	*D;
extern fa	*REFS;

extern STRP	m_str();

void
resplitrec(bufp, upf)
register char 	*bufp;
int	*upf;
{
	register int	lft;
	register char	*vstr;
	register VARP		tmp;
	VARP	m_var();
	int	tempstat;
	fa	*resep;

	resep=REFS;
	NF=0;
	NC=(int)bufp;
	tempstat=resep->initstat;
	for (;;) {
		if (++NF >= ND) {
			SP(D=(VARP *)realloc((char *)D, (++ND)*sizeof(VARP)), "resplitrec");
			D[NF]=m_var();
		}
		if (*upf==NF || *upf==-1) {
			upf+=(*upf>0)?1:0;
		} else if (*upf==-2) {
			bufp[(NC=strlen(bufp)-1)]='\0';
			return;
		} else {
			if (nematch(resep, bufp)) {
				resep->initstat=2;
				bufp=patbeg+patlen;
			} else {
				resep->initstat=tempstat;
				while (*bufp++)
					;
				break;
			}
			continue;
		}
		(tmp=D[NF])->cur=INITSTR;
		if (R(tmp)) {
			R(tmp)--;
			tmp->string=m_str(L(tmp), "");
		}
		vstr=S(tmp);
		lft=L(tmp)-1;
		if (nematch(resep, bufp)) {
			resep->initstat=2;
			while (bufp<patbeg) {
				(*vstr++)=(*bufp++);
				if (!lft--) {
					SP(S(tmp)=realloc(S(tmp), L(tmp)+=8), "resplitrec");
					vstr=S(tmp)+L(tmp)-8;
					lft=7;
				}
			}
			bufp=patbeg+patlen;
			*vstr='\0';
		} else {
			resep->initstat=tempstat;
			while ((*vstr++)=(*bufp++)) {
				if (!lft--) {
					SP(S(tmp)=realloc(S(tmp),L(tmp)+=8), "resplitrec");
					vstr=S(tmp)+L(tmp)-8;
					lft=7;
				}
			}
			*(vstr-2)='\0';
			break;
		}
	}
	*(bufp-2)='\0';
	pristine=1;
	NC=(int)bufp-NC-2;
}
