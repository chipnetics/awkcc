/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

#include <stdio.h>
#include "awkcc.h"

extern int	dbg;
extern int	OFSbug;
extern int	numifelsecommas;
extern int	numstrtemps;
extern int	numfuntemps;
extern int	numpartemps;
extern int	numvartemps;
extern int	awkdbg;
extern char	*npvar();

makeprog(e, symtab)
Nodep		e;
VARINFOP	*symtab;
{
	header();
	declarevars(e, symtab);
	initvars(symtab);
	freevars(symtab);
	PFO("extern int	upf[];\n");
	special();
	emit(e->narg[3]);
	PFO("\nmain(argc, argv, envp)\n");
	PFO("int\targc;\n");
	PFO("char\t*argv[],*envp[];\n{\n");
	PFO("#ifdef KMALLOC\n");
	PFO("extern int Mt_trace;\n");
	PFO("if (argc>2 && !strncmp(argv[1], \"-m\", 2)) {\nMt_trace=2;\n");
	PFO("argc--; argv++;\n}\n");
	PFO("#endif\n");
	PFO("mainloop(argc, argv, envp);\n}\n");
	PFO("\nvoid\nUSR()\n{\n");
	emit(e->narg[1]);
	PFO("}\n\nvoid\nBEGIN()\n{\n");
	emit(e->narg[0]);
	if (!(e->narg[1] || e->narg[2]))
		PFO("EXIT(1, 0);\n");
	PFO("}\n\nvoid\nEND()\n{\nnotseenend=0;\n");
	PFO("ADSTR(0,\"\"); NF=0; assign_NF();\n");
	emit(e->narg[2]);
	PFO("}\n");
	pickdolls(symtab);
}

pickdolls(symtab)
VARINFOP	*symtab;
{
	int	i;
	int	j;
	int	k;
	extern int	nodecmp();

	j=0;
	for (i=0; symtab[i]; i++)
		if (symtab[i]->type==INDIRECT && 
		    !NUMERIC(symtab[i]->exprs[0]->t)) {
			j=1;
			break;
		}
	qsort(symtab, i, sizeof(VARINFOP), nodecmp);
	PFO("int\tupf[]={");
	if (j || OFSbug) {
		PFO("-1};\n");
		return;
	} 
	for (k=0; !j && symtab[k]; k++)
		if (symtab[k]->type==INDIRECT && symtab[k]->exprs[0]->n)
			PFO("%d, ",(int)symtab[k]->exprs[0]->n);
	if (OVMEMQ("NF")	||
	    UMEMQ("NF")		||
	    memq("NF", INTVAR, FLOATVAR, 0))
		PFO("0};\n");
	else PFO("-2};\n");
}

special()
{
	PFO("\n#define GETR\t(rgetrec(D[0])?(");
	PFO("splitrec(S(D[0]), upf),");
	asgn("NF", "NF");
	incvar("NR", "NR");
	incvar("FNR", "FNR");
	PFO("1):(");
	PFO("(*S(D[0])='\\0'),NF=0,");
	asgn("NF", "0");
	PFO("0))\n\n\n#define AD0\t(osplitrec(D[0], upf),");
	asgn("NF", "NF");
	PFO("(pristine=1),(D[0]->cur=INITSTR))\n");
	PFO("ad0() { AD0; }\n");
	PFO("void\nassign_NF()\n{\n");
	PFO("int\ti;\n");
	PFO("int\ttmpnum;\n");
	PFO("VARP\ttmpvar;\n");
	asgn("NF", "NF");
	PFO("0;\n}\n");
	PFO("getfnr()\n{\nreturn (int) ");
	getnum("FNR");
	PFO(";\n}\n");
	PFO("void\nresetfnr()\n{\n");
	asgn("FNR", "0");
	PFO("0;\n}\n");
}

getnum(str)
char	*str;
{
	char	*p;

	switch(ismem(str)) {
	case 0:
		p="(FNR)";
		break;
	case FLOATVAR:
		p="(Af_%s)";
		break;
	case INTVAR:
		p="(Ai_%s)";
		break;
	default:
		p="TN(Au_%s)";
		break;
	}
	PFO(p, str);
}

incvar(str)
char	*str;
{
	char	*p;

	switch(ismem(str)) {
	case 0:
		return;
	case FLOATVAR:
		p="(Af_%s=Af_%s+1),";
		break;
	case INTVAR:
		p="(Ai_%s++),";
		break;
	default:
		p="PREID(++, Au_%s),";
		break;
	}
	PFO(p, str, str);
}

asgn(str, str1)
char	*str;
char	*str1;
{
	char	*p;

	switch (ismem(str)) {
	case 0:
		return;
	case FLOATVAR:
		p="(Af_%s=%s),";
		break;
	case INTVAR:
		p="(Ai_%s=%s),";
		break;
	default:
		p="ANUM(Au_%s, (double) %s),";
		break;
	}
	PFO(p, str, str1);
}

header()
{
	PFO("#include <stdio.h>\n");
#ifdef PC7300
	PFO("#define PC7300\t1\n");
#endif
	PFO("#include <math.h>\n");
	PFO("#include \"ear.h\"\n");
	PFO("#include \"hash.h\"\n");
	PFO("#include \"awk.h\"\n");
	PFO("#include \"y.tab.h\"\n");
	PFO("#include \"header.h\"\n");
	PFO("VARP\ttmpvar;\n");
	PFO("void\tEND();\n");
	PFO("int\ti;\n\n");
}

declarevars(e, symtab)
Nodep		e;
VARINFOP	*symtab;
{
	Nodep	tmp;
	int	i;
	char	*p, *s;

	tmpva(e);
	for (tmp=e->FOUR; tmp; tmp=tmp->nnext) {
		if (hasret(tmp->s))
			PFO("VARP\t");
		else PFO("void\t");
		PFO("AF_%s();\n", tmp->s);
	}
	for (i=0; i<numstrtemps; i++)
		PFO("char\ttmpstr%d[CONVSIZ];\n", i);
	for (i=0; i<numifelsecommas; i++)
		PFO("int\tifelse%d;\n", i);
	for (i=0; i<numvartemps; i++) 
		PFO("VARP\ttmpvar%d;\n", i);
	for (i=0; i<numfuntemps; i++) 
		PFO("VARP\tfuntmp%d=(VARP) NULL;\n", i);
	for(i=0; symtab[i]; i++) {
		s=(char *)symtab[i]->string;
		switch(symtab[i]->type) {
		case VAR: case SYSVAR: case FLOATVAR: 
		case INTVAR: case REGEXP: case ARRAY:
			break;
		default:
			continue;
		}
		switch(symtab[i]->type) {
		case REGEXP:
			p=(symtab[i]->flags&FREGEXP)?"1_":"";
			PFO("extern fa\t*reg%s%d;\n", p, i);
			PFO("fa\t*reg%s%d;\n", p, i);
			break;
		case FLOATVAR:
			PFO("double\tAf_%s;\n", s);
			break;
		case INTVAR:
			PFO("int\tAi_%s;\n", s);
			break;
		case ARRAY:
			if (strcmp(s, "ARGV"))
				PFO("HTBP\tAa_%s;\n", symtab[i]->string);
			break;
		case SYSVAR:
			if (strcmp(s, "NR") && strcmp(s, "FNR") &&
			    strcmp(s, "NF")) 
				break;
		case VAR:
		default:
			PFO("VARP\tAu_%s;\n", npvar(i));
			break;
		}
	}
	if (dbg) {
		fprintf(stderr, "FUNTMPS: %d\n", numfuntemps);
		fprintf(stderr, "PARTMPS: %d\n", numpartemps);
	}
	/* This is okay for partemps, since they don't have to
	 * be initialized, really.
	 */
	for (i=0; i<numpartemps*numfuntemps; i++) 
		PFO("VAR\tpartmp%d;\n", i);
}

uinstall(s, ts, tn)
uchar	*s;
char	*ts;
int	tn;
{
	if (awkdbg || UMEMQ(s)) {
		PFO("tmpv=s_insert(\"%s\"", s);
		PFO(", USRVARS);\n");
		PFO("tmpv->string=m_str(1, \"\");\n");
		PFO("free(S(tmpv));\n");
		PFO("S(tmpv)=(char *)(&A%s_%s);\n", ts, s);
		PFO("tmpv->num=%d.0;\n", tn);
	}
}

udelete(s, ts, tn)
uchar	*s;
char	*ts;
int	tn;
{
	if (awkdbg || UMEMQ(s)) {
		PFO("S(s_insert(\"%s\"", s);
		PFO(", USRVARS))=malloc(1);\n");
	}
}

freevars(symtab)
VARINFOP	*symtab;
{
	int	i;
	char	*p, *s;

	PFO("\nvoid\nfree_vars()\n{\n");
	for(i=0; symtab[i]; i++) {
		s=(char *)symtab[i]->string;
		/* SYSVARS MUST NOT BE INITIALIZED HERE! */
		switch(symtab[i]->type) {
		case UVAR:
			break;
		case FLOATVAR:
			if (!wassysvar(s))
				udelete(s, "f", 1);
			break;
		case INTVAR:
			if (!wassysvar(s))
				udelete(s, "i", 2);
			break;
		case SYSVAR:
			if (strcmp(s, "NR") && strcmp(s, "FNR") &&
			    strcmp(s, "NF"))
				break;
		case VAR:
			PFO("free_var(Au_%s);\n", npvar(i));
			udelete(s, "u", 3);
			break;
		case ARRAY:
			if (strcmp((char *)s, "ARGV"))
				PFO("free_array(Aa_%s);\n", s);
			break;
		case REGEXP:
			p=(symtab[i]->flags&FREGEXP)?"1_":"";	
			PFO("freefa(reg%s%d);\n", p, i);
			break;
		}
	}
	for (i=0; i<numpartemps*numfuntemps; i++) 
		PFO("free_str(partmp%d.string);\n", i);
	for (i=0; i<numvartemps; i++) 
		PFO("free_var(tmpvar%d);\n", i);
	PFO("free_array(INPIPES);\n");
	PFO("free_array(OUTPIPES);\n");
	PFO("free_array(INFILES);\n");
	PFO("free_array(OUTFILES);\n");
	PFO("}\n");
}

initvars(symtab)
VARINFOP	*symtab;
{
	int	i;
	char	*p, *s;

	PFO("\nvoid\ninit_vars()\n{\n");
	PFO("VARP\tm_var();\nHTBP\tm_array();\n\n");
	PFO("VARP\ttmpv;\n");
	for(i=0; symtab[i]; i++) {
		s=(char *)symtab[i]->string;
		/* SYSVARS MUST NOT BE INITIALIZED HERE! */
		switch(symtab[i]->type) {
		case UVAR:
			if (!memq(s, FLOATVAR, INTVAR, 0) &&
			    !memq(s, SYSVAR, VAR, 0)) {
				fprintf(stderr, "WARNING: `-u %s' ignored, since you never use variable %s.\n", s, s);
				PFO("/* would have put a -u %s here */\n", s);
			} else if (memq(s, ARRAY, SYSARRAY, 0))
				fprintf(stderr, "WARNING: `-u %s' ignored, since %s is an array.\n", s, s);
			break;
		case FLOATVAR:
			if (wassysvar(s))
				continue;
			PFO("Af_%s=0.0;\n", s);
			uinstall(s, "f", 1);
			break;
		case INTVAR:
			if (wassysvar(s))
				continue;
			PFO("Ai_%s=0;\n", s);
			uinstall(s, "i", 2);
			break;
		case SYSVAR:
			if (strcmp(s, "NR") && strcmp(s, "FNR") &&
			    strcmp(s, "NF")) 
				break;
		case VAR:
			PFO("Au_%s=m_var();\n", npvar(i));
			uinstall(s, "u", 3);
			break;
		case ARRAY:
			if (strcmp((char *)s, "ARGV")) {
				PFO("Aa_"); 
				ma((char *)s);
			}
			break;
		case REGEXP:
			p=(symtab[i]->flags&FREGEXP)?"1_":"";
			PFO("reg%s%d=mdfa(reparse(", p, i);
			printstring(s, 1, 0);
			PFO("), %d);\n", symtab[i]->flags&FREGEXP);
			break;
		}
	}
	for (i=0; i<numpartemps*numfuntemps; i++) 
		PFO("partmp%d.string=m_str(1, \"\");\n", i);
	for (i=0; i<numvartemps; i++) 
		PFO("tmpvar%d=m_var();\n", i);
	ma("INPIPES");
	ma("OUTPIPES");
	ma("INFILES");
	ma("OUTFILES");
	PFO("}\n");
}

ma(s)
char	*s;
{
	PFO("%s=m_array();\n", s);
}
