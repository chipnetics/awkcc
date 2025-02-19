/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/


typedef double	Awkfloat;
typedef	unsigned char uchar;

#ifdef MYALLOC
	extern uchar *Malloc(), *Calloc();
#else
#	define	Malloc	malloc
#	define	Calloc	calloc
#	define	Free	free
#endif

#define	xfree(a)	{ if ((a) != NULL) { Free(a); a = NULL; } }

#ifdef	DEBUG
#	define	dprintf	if(dbg)printf
#else
#	define	dprintf(x1, x2, x3, x4)
#endif

/*extern	uchar	*patbeg;*//* beginning of pattern matched */
/*extern	int	patlen;*/	/* length.  set in b.c */

/* Cell:  all information about a variable or constant */

/* Cell.tval values: */
/*
uchar	*tostring(), *malloc(), *calloc();
*/

/* Node:  parse tree is made of nodes, with Cell's at bottom */

typedef struct Node {
	int	ntype;
	struct	Node *nnext;
	int	lineno;
	int	nobj;
	struct Node *narg[1];	/* variable: (must be last in struct) actual size set by calling malloc */
} Node;

/* node types */
#define NEXPR	3

extern	Node	*op2();
extern	Node	*reparse();

#define NCHARS	128+1	/* could be 256+1 */
#define NSTATES	16

typedef struct rrow {
	int ltype;
	int lval;
	int *lfollow;
} rrow;

typedef struct fa {
	/* new */
	uchar	*restr;
	int	anchor;
	int	use;
	/* old */
	uchar gototab[NSTATES][NCHARS];
	int *posns[NSTATES];
	uchar out[NSTATES];
	int initstat;
	int curstat;
	int accept;
	int reset;
	struct rrow re[1];
} fa;

extern	fa	*mdfa();
