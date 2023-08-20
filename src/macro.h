#ifndef __MACROHH__
#define __MACROHH__ 1

#include <stdio.h>

#ifndef BOJDIRPATH
	char BOJDIRPATH[255];
#endif

typedef struct {
	const char* name;
	const char* ext;
} LANG;

extern char temp[255];
extern FILE *read, *write; 
extern char buffer;

void 	mergeSource (LANG nameOfSource, const char* const prblmNumber);
int  	strcmpForqsort (const void*, const void*);
void 	gitMacro (void);

#endif