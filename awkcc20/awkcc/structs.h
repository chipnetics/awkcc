/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

typedef unsigned char	uchar;

typedef struct node {

	/* TYPE INFORMATION */
	short	y;		/* the current type 			*/
	short	w;		/* what we want it to yield 		*/
	short 	t;		/* availabiliby indicator 		*/
	short	rt;		/* real type 				*/

	/* COUNTERS */
	short	sympos;		/* position in symbol table 		*/
	short	called;		/* number of times function called	*/
	short	tempvar;	/* number of the temporary variable	*/
	short	tempstr;	/* number of the temporary string	*/
	short	tempfun;	/* number of funtemp 			*/
	short	tempif;		/* for ifelse's 			*/
	short	argnum;		/* for arguments			*/

	/* FLAGS */
	short	flags;

	/* USEFUL */
	double	n;		/* The numeric value (leaves only?)	*/
	uchar	*s;		/* The string value (leaves only)	*/

	/* MISCELLANEOUS */
	uchar	subs;		/* to keep track of narg's size 	*/
	short	ntype;		/* to make the yacc version happy 	*/
	short	lineno;		/* for error messages			*/

	/* OTHER NodeS */
	struct node	*nnext;
	struct node	**narg;
} Node, *Nodep;

#define TOKLEN	400

typedef struct {
	int 	type;
	double 	num;
	uchar 	string[TOKLEN];
	int	lineno;
} TOKEN, *TOKENP;
