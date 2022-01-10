/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

#include <stdio.h>
#include "awk.h"
#include "ear.h"

#define MAXREPS	10

char	*reploc[MAXREPS+2];

extern char	*patbeg;
extern int	patlen;

extern VARP	gsubvar;
extern VARP	*D;
extern int	upf[];
extern int	pristine;
static int	numreps;

Agsub(v, prog, s, tv, prist, sub)
register VARP	v;
fa	*prog;
char	*s;
register VARP	tv;
int	prist;
int	sub;
{
	int	tempstat;
	int	mflag=0;
	char	*gopoint;
	int	ret=0;
	int	slen;
	int	tvlen;
	int	i;
	extern STRP	m_str();

	FS(tv);
	if (R(v)) {
#ifdef DEBUG
		fprintf(stderr, "[internal warning: tmpv in gsub in use]\n");
#endif
		R(v)--;
		v->string=m_str(L(tv), "");
	}
	*(S(v))='\0';
	gopoint=(S(tv));
	tvlen=strlen(gopoint);
	slen=Areplen(s);
	tempstat=prog->initstat;
	while (pmatch(prog,gopoint)) {
		prog->initstat=2;
		tvlen+=(patlen*numreps+slen);
		if (L(v)<tvlen+2)
			SP(S(v)=realloc(S(v),L(v)=tvlen+2),"gsub");
		if (patlen) {
			Arepsub(S(v), gopoint, patbeg, patlen);
		} else {
			if (mflag) {
				ret--;
				mflag=0;
			} else {
				Arepsub(S(v), gopoint, patbeg, patlen);
				mflag=1;
			}
			if (!*patbeg) {
				ret++;
				gopoint=patbeg+patlen;
				break;
			}
			i=strlen(S(v));
			S(v)[i]=(*gopoint);
			S(v)[i+1]='\0';
			patbeg++;
		}
		gopoint=patbeg+patlen;
		ret++;
		if (!*gopoint || !*(gopoint-1)) {
			gopoint="";
			break;
		}
		if (sub) break;
			
	}
	tv->cur=INITSTR|MUSTFLD;
	if (ret) {
		if (prist) pristine=0;
		strcat(S(v), gopoint);
		R(v)++;
		free_str(tv->string);
		tv->string=v->string;
		if (tv==D[0]) {
			osplitrec(D[0], upf);
			pristine=1;
			assign_NF();
		}
	}
	prog->initstat=tempstat;
	return ret;
}

Arepsub(dest, gopoint, beg, plen)
register char	*dest;
char	*gopoint;
register char	*beg;
int 	plen;
{
	int	n;
	char	*tmp;

	dest=dest+strlen(dest);
	while (gopoint<beg)
		(*dest++)=(*gopoint++);
	*dest='\0';
	for (n=0; n<=numreps; n++) {
		tmp=reploc[n];
		while((*dest++)=(*tmp++))
			;
		dest--;
		if (n<numreps) {
			strncat(dest, beg, plen);
			dest=dest+plen;
			*dest='\0';
		}
	}
}

Areplen(s)
register char	*s;
{
	register int	ans=0;
	register char	*c;
	int	sublength;

	sublength=1+strlen(s);
	if (L(gsubvar) < sublength) {
		free(S(gsubvar));
		SP(S(gsubvar)=malloc(L(gsubvar)=sublength),"Areplen");
	}
	c=S(gsubvar);
	numreps=0;
	*c=(*s);
	reploc[numreps++]=c;
	for (;*s;s++, c++) {
		switch(*s) {
		case '\\':
			if (*(s+1)=='&')
				s++;
			*c=(*s);
			ans++;
			break;
		case '&':
			if (numreps>=MAXREPS)
				ER "8+ '&'s in sub/gsub" SX;
			*c='\0';
			reploc[numreps++]=c+1;
			break;
		default:
			*c=(*s);
			ans++;
		}
	}
	*c='\0';
	numreps--;
	return(ans);
}
