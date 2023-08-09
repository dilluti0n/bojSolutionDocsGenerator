#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#ifndef BOJDIRPATH
	char BOJDIRPATH[64];
#endif

#define NUMOFPROBLEMS 100
#define NAMEOFSOURCES { {.name = "c", .ext = "c"}, {.name = "python", .ext = "py"}, {.name = NULL} }

typedef struct _Langs {
	const char* name;
	const char* ext;
} LANG;

DIR 			*dir_info;
struct dirent   *dir_entry;
FILE 			*read, *write; 
	
char nameOfFiles[NUMOFPROBLEMS][16]; //array of name of files in BOJDIRPATH/sol
char temp[64], indPth[64];
char buffer;
char *charPointer;

void mergeSource (LANG nameOfSource, const char* const prblmNumber);
int  strcmpForqsort (const void*, const void*);