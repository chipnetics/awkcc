/***
 * Copyright 1987 by AT&T Bell Laboratories.
 * All rights reserved.
 * Note: Lucent Technologies is now Alcatel-Lucent; some copyrights
 * may have been assigned by AT&T to Alcatel-Lucent.
 ***/

#define MAX_LEVELS	128
#define MAX_FORMAT	124

typedef struct varinfo {
	short	cur[MAX_LEVELS];	/* currently avail 		*/
	short	rt[MAX_LEVELS];		/* "real" type 			*/
	short	type;			/* what kind of symbol? 	*/
	uchar	*string;		/* symbol's name? 		*/
	uchar	*func;			/* if param, which func? 	*/
	short	flags;			/* REGEXP type in bit one	*/
	Nodep	*exprs;			/* $(x+1): expr<-x+1		*/
	short	ne;			/* # of exprs			*/
	short	sympos;			/* position in symbol table	*/
	short	argnum;			/* argument number, if arg	*/
	short	called;			/* times called, if param 	*/
	short	atyped;			/* save work in array typing	*/
} VARINFO, *VARINFOP;
	
#define MAX_ARRAYS	16
#define MAX_VARIABLES	124

#define LEFT	narg[0]
#define RIGHT	narg[1]
#define THREE	narg[2]
#define FOUR	narg[3]
