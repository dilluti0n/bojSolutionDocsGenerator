#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#ifdef __linux__
	#define BOJDIRPATH "/home/hskim/my-site/baekjun"
#endif
#ifdef __APPLE__
	#define BOJDIRPATH "/Users/hskimse/Desktop/백준"
#endif

#define NUMOFPROBLEMS 100
#define DOC_LENGTH 10000
#define URL "http://dilluti0n.github.io"

DIR 			*dir_info;
struct dirent   *dir_entry;
FILE 			*sol, *c, *py, *inDex, *page;
	
char nameOfFiles[16][NUMOFPROBLEMS];
char temp[256], indpth[256];
char buffer;
char (*strPointer)[NUMOFPROBLEMS] = nameOfFiles;
char *charPointer;

int main ( int argc, char* argv[] ) {
	#ifdef URL
		char url[30] = URL;
	#else
		char url[20];
		printf ("Enter your site's URL: ");
		scanf ("%s", url);
	#endif

	#ifdef BOJDIRPATH
	#else
		char BOJDIRPATH[20];
		printf ("Enter your directory's path: ");
		scanf ("%s", BOJDIRPATH);
	#endif

	printf("------------------------\nbojSolutionDocsGenerator\n------------------------\n\n");

	strcpy (indpth, BOJDIRPATH);
	strcat (indpth, "/docs/index.markdown");
	inDex = fopen ( indpth, "w"); // open BOJDIRPATH/docs/index.markdown as inDex

	fprintf(inDex, "---\nlayout: home\ntitle: BOJ Solution Documentation\n---\n> This site provides solution for several problems in [Baekjun Online Judge, BOJ](https://www.acmicpc.net/), one of the coding challenge sites in Korea. Each matirial in Section [Solutions](##-solutions), named by the problem number of [BOJ](https://www.acmicpc.net/), contains the algorithm of each problem with consistent notation and the source code respectively. In section [Posts](##-posts), I will post the personal thoughts or motives I felt when solving the problem.\n## Solutions\n");
	
	strcpy (temp, BOJDIRPATH);
	strcat (temp, "/sol");
	dir_info = opendir ( temp ); // open BOJDIRPATH/sol as dir_info

	if ( dir_info != NULL ) {
		while ( (dir_entry = readdir ( dir_info ))) 
			if ( dir_entry->d_name[0] != '.') {
				strcpy(*strPointer, dir_entry->d_name);

				charPointer = *strPointer;
				while (*charPointer != '.')
					charPointer++;
				*charPointer = '\0'; // xxxx.md -> xxxx
				fprintf ( inDex,"[");
				fprintf ( inDex, "%s](Solutions/%s.html) ",*strPointer, *strPointer );
				strPointer++;
			}
		strcpy (*strPointer, "END"); 
		closedir (dir_info);
	}
	fclose (inDex);
	printf ("generated indexfile: %s\n",indpth);
	printf ("target problems are:");
	
	for (strPointer = nameOfFiles; strcmp( *strPointer, "END") != 0 ; strPointer++)
		printf(" %s", *strPointer);
	printf("\n");
	
	for (strPointer = nameOfFiles; strcmp( *strPointer, "END") != 0 ; strPointer++) {
		printf("\n");
		strcpy (temp, BOJDIRPATH);
		strcat (temp, "/docs/Pages/");
		strcat (temp, *strPointer);
		strcat (temp, ".md"); //temp = BOJDIRPATH/Pages/xxxx.md

		printf ("target: %s\n", temp);

		page = fopen ( temp, "w"); //open BOJDIRPATH/Pages/xxxx.md as page

		strcpy(temp, BOJDIRPATH);
		strcat (temp, "/sol/");
		strcat (temp, *strPointer);
		strcat (temp, ".md"); //temp = BOJDIRPATH/sol/xxxx.md

		printf ("assign from %s\n", temp);

		sol = fopen ( temp ,"r"); //open BOJDIRPATH/sol/xxxx.md as sol

		while ( (buffer = fgetc(sol)) != EOF ) //page <- sol
			fputc (buffer, page);
		fclose (sol);

		fputs ("\n## Source code\n", page); 

		strcpy (temp, BOJDIRPATH);
		strcat (temp, "/py/");
		strcat (temp, *strPointer);
		strcat (temp, ".py"); //temp = BOJDIRPATH/py/xxxx.py
		printf ("assign from %s\n", temp);

		if ( fopen ( temp, "r") != NULL ) {
			py = fopen ( temp, "r"); // open BOJDIRPATH/py/xxxx.py as py
			fputs ("### Python\n", page);
			fputs ("```python\n", page);
			while ( (buffer = fgetc(py)) != EOF ) //page <- py
				fputc (buffer, page);
			fputs ("\n```\n", page);
			fclose (py);
		}

		strcpy (temp, BOJDIRPATH);
		strcat (temp, "/c/");
		strcat (temp, *strPointer);
		strcat (temp, ".c");
		printf ("assign from %s\n", temp);

		if ( fopen ( temp, "r") != NULL ) {
			c = fopen ( temp, "r"); // open BOJDIRPATH/c/xxxx.c as c
			fputs ("\n### C\n", page);
			fputs ("```c\n", page);
			while ( (buffer = fgetc(c)) != EOF ) //page <- c
				fputc (buffer, page);
			fputs ("\n```",page);
			fclose (c);
		}
		fputc ('\n',page);

		fclose (page);
		printf("target %s.md generated!\n",*strPointer);
	}
	printf ("\ngenerated files in: ");
	printf (BOJDIRPATH);
	printf ("/docs/Pages\n");

	return 0;
}