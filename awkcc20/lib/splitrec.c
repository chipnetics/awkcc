/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

#include <stdio.h>
#include "ear.h"
	
extern char	ctest[MAX_CHARS];
extern int	rsdflt;
	
/* -2 means that's all we need. 
 * -1 means get everything else
 * 0 means count NF, but don't insert
 */
	
extern int	upf[];
extern int	dflt;
	
extern int	ND;
extern int	NF;
extern int	NC;
extern int	pristine;
	
extern VARP	*D;

extern STRP	m_str();

/* PSF/EXPTOOLS changed bufp, c, and vstr to unsigned - don't index
 * into ctest with negative values 
 */

void
splitrec(bufp, upf)
register unsigned char 	*bufp;
int	*upf;
{
	int	forlen;
	register int	lft;
	register unsigned char	c;
	register unsigned char	*vstr;
	VARP		tmp, m_var();

	NF=0;
	NC=(int)bufp;
	c=(*bufp);
	if (dflt==2) {
		strsplitrec(bufp, upf);
		return;
	} else if (dflt==3) {
		resplitrec(bufp, upf);
		return;
	}
	if (dflt && ctest[*bufp]) {
		while (ctest[c=(*bufp++)])
			;
		--bufp;
		c=(*(bufp-1));
	}
	while (c!='\n') {
	NOTREALLYDONE:;
		if (++NF >= ND) {
			SP(D=(VARP *)realloc((char *)D, (++ND)*sizeof(VARP)), "splitrec");
			D[NF]=m_var();
		}
		if (*upf==NF || *upf==-1) {
			upf+=(*upf>0)?1:0;
		} else if (*upf==-2) {
			forlen=strlen(bufp);
			NC=(int)bufp-NC+forlen-1;
			bufp[forlen-1]='\0';
			return;
		} else {
			while (!ctest[c=(*bufp++)])
				;
			if (dflt && ctest[*bufp]) {
				while (ctest[c=(*bufp++)])
					;
				bufp--;
				c=(*(bufp-1));
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
		while (!ctest[c=(*vstr++)=(*bufp++)]) {
			if (!lft--) {
				SP(S(tmp)=realloc(S(tmp),L(tmp)+=8), "splitrec");
				vstr=S(tmp)+L(tmp)-8;
				lft=7;
			}
		}
		if (dflt && ctest[*bufp]) {
			while (ctest[c=(*bufp++)])
				;
			bufp--;
			c=(*(bufp-1));
		}
		*--vstr='\0';
	}
	if (rsdflt && *bufp)
		goto NOTREALLYDONE;
	pristine=1;
	if (NC==(int)bufp) {
		*bufp='\0';
		NC=0;
	} else {
		*--bufp='\0';
		NC=(int)bufp-NC;
	}
}


osplitrec(varp, upf)
register VARP	varp;
int	*upf;
{
	register int	j;

	j=strlen(S(varp));
	if (R(varp)) {
		R(varp)--;
		varp->string=m_str(j+2, S(varp));
	} else if (j+2>L(varp))
		SP(S(varp)=realloc(S(varp), L(varp)=j+2), "osplit");
	S(varp)[j]='\n';
	S(varp)[j+1]='\0';
	splitrec(S(varp), upf);
}
