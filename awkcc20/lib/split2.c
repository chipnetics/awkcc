/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

#include <stdio.h>
#include "ear.h"
#include "hash.h"

extern char	ctest[MAX_CHARS];
extern char	*numbers[];
extern VARP	REALSPLIT;
extern STRP	m_str();

split2(bufp, array, sep)
register char 	*bufp;
HTBP	array;
char	*sep;
{
	static int	arrayupf[]={-1};	/* for now, get everything */
	register int	lft;
	register char	c, *vstr;
	VARP		tmp, m_var(), s_insert();
	char	subifs, tmpstr[CONVSIZ];
	int	i, *upf;

	c=(*bufp);
	upf=arrayupf;
	vassign_str(REALSPLIT,sep);
	sep=S(REALSPLIT);
	fixreg(sep);
	subifs=(*sep);
	for (i=1; c; i++) {
		if (*upf==i || *upf==-1) {
			upf+=(*upf>0)?1:0;
		} else if (*upf==-2) {
			bufp[strlen(bufp)-1]='\0';
			return(0);
		} else {
			for (;;) {
				if ((c=(*bufp++))=='\n' || !c)
					goto DONE1;
				if (c==subifs) {
					register char	*s1=bufp;
					register char	*s2=sep+1;
					
					for (;*s2 && *s1++==*s2; s2++)
						;
					if (!*s2) {
						bufp=s1;
						goto DONE1;
					}
				}
			}
			DONE1:;
			continue;
		}
		if (i<MAX_NUMBERS) {
			tmp=s_insert(numbers[i], array);
			tmp->string=m_str(1, "");
		} else {
			sprintf(tmpstr, "%d", i);
			tmp=s_insert(tmpstr, array);
			tmp->string=m_str(1, "");
		}
		tmp->cur=INITSTR;
		vstr=S(tmp);
		lft=L(tmp)-1;
		for (;;) {
			if ((c=(*vstr++)=(*bufp++)) == '\n' || !c)
				goto DONE2;
			if (c==subifs) {
				register char	*s1=bufp;
				register char	*s2=sep+1;
				
				for (;*s2 && *s1++==*s2; s2++)
					;
				if (!*s2) {
					bufp=s1;
					goto DONE2;
				}
			}
			if (!lft--) {
				SP(S(tmp)=realloc(S(tmp),L(tmp)+=8), "split2");
				vstr=S(tmp)+L(tmp)-8;
				lft=7;
			}
		}
		DONE2:;
		*--vstr='\0';
	}
	return(i-1);
}
