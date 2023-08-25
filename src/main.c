#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include "crawler.h"
#include "macro.h"
#include "buf.h"

#define NUMOFPROBLEMS 100

DIR 			*dir_info;
struct dirent   *dir_entry;
FILE 			*read, *write;

int error;
	
char nameOfFiles[NUMOFPROBLEMS][16]; /*array of name of files in BOJDIRPATH/sol*/
char (*strPointer)[16] = nameOfFiles;
char temp[255], indPth[255], problemTitle[255] = "error !!!";
char *charPointer;
char buffer;

LANG	srcs[] = { 
		{.name = "c", .ext = "c"},
		{.name = "python", .ext = "py"},
		{.name = NULL}
		};
DOCS	macro[] = {
		{.id = "problem_description", .header = NULL},
		{.id = "problem_input", .header = "## input"},
		{.id = "problem_output",.header = "## output"},
		{.id = NULL}
		};

int main ( int argc, char* argv[] ) {
	printf("---------------------------\n| bojSolutionDocsGenerator |\n---------------------------\n\n");

	#ifndef BOJDIRPATH
		printf ("Enter your directory's path: ");
		scanf ("%s", BOJDIRPATH);
	#endif

	strcpy (temp, BOJDIRPATH);
	strcat (temp, "/assets/origin.md");
	if ( !(read = fopen ( temp, "r")) ) { /* open BOJDIRPATH/assets/origin.md as read */
		printf ("\n[error] Wrong Path or assets/origin.md does not exists !!!\n");
		return -1 ;
	}
	strcpy (indPth, BOJDIRPATH);
	strcat (indPth, "/docs/index.markdown");
	write = fopen ( indPth, "w"); /* open BOJDIRPATH/docs/index.markdown as write */

	while ( (buffer = fgetc(read)) != EOF ) /* index.md <- origin */
			fputc (buffer, write);
	fclose (read); /* close read(origin.md) */
	
	strcpy (temp, BOJDIRPATH);
	strcat (temp, "/assets/sol");

	if ( !(dir_info = opendir ( temp )))  {
		printf("There is no file to merge!!\n"); /* open BOJDIRPATH/assets/sol as dir_info */
		return -1;
	}
	int cnt = 0;
	while ( (dir_entry = readdir ( dir_info ))) 
		if ( dir_entry->d_name[0] != '.') {
			strcpy(*strPointer, dir_entry->d_name);

			charPointer = *strPointer;
			while (*charPointer != '.')
				charPointer++;
			*charPointer = '\0'; /* xxxx.md -> xxxx */
			strPointer++;
			cnt++;
		}
	qsort(nameOfFiles, cnt, 16, strcmpForqsort); /*sort nameOfFiles to ascending order*/
	closedir (dir_info);
	**strPointer = '\0';

	printf ("target problems are:");
	for (strPointer = nameOfFiles; **strPointer ; strPointer++) {
		printf(" %s", *strPointer);
		fprintf ( write,"[");
		fprintf ( write, "%s](Solutions/%s.html) ",*strPointer, *strPointer );
	}

	fclose (write); /*close write(index.markdown)*/

	printf ("\ngenerated indexfile: %s\n",indPth);

	for (strPointer = nameOfFiles, cnt = 1; **strPointer ; strPointer++,cnt++) {
		printf("\n");

		/*open write(Solutions/xxxx.md)*/
		strcpy (temp, BOJDIRPATH);
		strcat (temp, "/docs/Solutions/");
		strcat (temp, *strPointer);
		strcat (temp, ".md"); /*temp = BOJDIRPATH/Solutions/xxxx.md*/
		write = fopen ( temp, "w"); /*open BOJDIRPATH/Solutions/xxxx.md as write*/
		printf ("target %s\n", temp);

		/*crawl title & problem*/
		if ( ( error = titleWriter(problemTitle, *strPointer) ) >= 0) {
			printf("\n***********************************************************************\n");
			printf("*** completely crawled from %s!!! ***\n", temp);
			printf("***********************************************************************\n\n");
		}
		else
			printf("crawl failed, errcode: [%d]\n", error);
		/*put yaml front to Solutions/xxxx.md*/
		fprintf(write, "---\nlayout: page\ntitle: %s %s\nparent: Solutions\nnav_order: %i\n---\n", *strPointer, problemTitle, cnt);

		/*make page title*/
		fprintf(write, "# [BOJ] [%s](https://www.acmicpc.net/problem/%s) %s\n",*strPointer, *strPointer, problemTitle);

		/* open & copy crawled elements */
		for ( DOCS* docPointer = macro; docPointer->id; docPointer++) {
			if ( docPointer->header ) {
				fputc ('\n', write);
				fputs (docPointer->header, write);
				fputc ('\n', write);
			}
			
			BojBuffer* listPtr = docPointer->io.next;
			charPointer = listPtr->bp;
			while ( listPtr ) {
				fputc (*charPointer, write);
				if (*charPointer == '$')
					fputc ('$', write);
				charPointer++ ;
				if (*charPointer == '\0') {
					if ( (listPtr = listPtr->next) )
						charPointer = listPtr->bp;
				}
			}

			BojBufFree (&docPointer->io);
		}
		/*open read(sol/xxxx.md)*/
		strcpy (temp, BOJDIRPATH);
		strcat (temp, "/assets/sol/");
		strcat (temp, *strPointer);
		strcat (temp, ".md"); /*temp = BOJDIRPATH/assets/sol/xxxx.md*/
		read = fopen ( temp ,"r"); /*open BOJDIRPATH/sol/xxxx.md as read*/
		printf ("origin %s\n", temp);

		/*page <- sol*/
		while ( (buffer = fgetc(read)) != EOF )
			fputc (buffer, write);
		fclose (read); /* close read(sol/xxxx.md) */

		/*merge sources of srcs*/
		fputs ("\n## Codes\n", write); 
		for (const LANG* ptr = srcs; ptr->name; ptr++) 
		mergeSource (*ptr, *strPointer);
		fputc ('\n',write);

		fclose (write); /*close write(Solutions/xxxx.md)*/
		printf("target 'Solutions/%s.md' generated!\n",*strPointer);
	}
	printf ("\ncomplete!!\ngenerated files are in ");
	printf ("%s",BOJDIRPATH);
	printf ("/docs/Solutions\n");

	system ("rm -rf ./*.bojSolGen");

	#ifdef __GITCOMMENDLINEMACRO__
	gitMacro();
	#endif

	return 0;
}
