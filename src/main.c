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

#ifdef BOJDIRPATH
#else
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
LANG			srcs[] = NAMEOFSOURCES;
	
char nameOfFiles[NUMOFPROBLEMS][16]; //array of name of files in BOJDIRPATH/sol
char temp[64], indPth[64];
char buffer;
char (*strPointer)[16] = nameOfFiles;
char *charPointer;

void mergeSource (LANG nameOfSource, const char* const prblmNumber);
int  strcmpForqsort (const void*, const void*);

int main ( int argc, char* argv[] ) {
	printf("------------------------\nbojSolutionDocsGenerator\n------------------------\n\n");

	#ifdef BOJDIRPATH
	#else
		printf ("Enter your directory's path: ");
		scanf ("%s", BOJDIRPATH);
	#endif

	strcpy (temp, BOJDIRPATH);
	strcat (temp, "/assets/origin.md");
	read = fopen ( temp, "r");// open BOJDIRPATH/assets/origin.md as read

	strcpy (indPth, BOJDIRPATH);
	strcat (indPth, "/docs/index.markdown");
	write = fopen ( indPth, "w"); // open BOJDIRPATH/docs/index.markdown as write

	while ( (buffer = fgetc(read)) != EOF ) //index.md <- origin
			fputc (buffer, write);
	fclose (read); //close read(origin.md)
	
	strcpy (temp, BOJDIRPATH);
	strcat (temp, "/assets/sol");
	dir_info = opendir ( temp ); // open BOJDIRPATH/assets/sol as dir_info

	if ( !dir_info )  {
		printf("There is no file to merge!!\n");
		return -1;
	}
	int cnt = 0;
	while ( (dir_entry = readdir ( dir_info ))) 
		if ( dir_entry->d_name[0] != '.') {
			strcpy(*strPointer, dir_entry->d_name);

			charPointer = *strPointer;
			while (*charPointer != '.')
				charPointer++;
			*charPointer = '\0'; // xxxx.md -> xxxx
			strPointer++;
			cnt++;
		}
	qsort(nameOfFiles, cnt, 16, strcmpForqsort); //sort nameOfFiles to ascending order
	closedir (dir_info);

	printf ("target problems are:");
	for (strPointer = nameOfFiles; **strPointer ; strPointer++) {
		printf(" %s", *strPointer);
		fprintf ( write,"[");
		fprintf ( write, "%s](Solutions/%s.html) ",*strPointer, *strPointer );
	}
	**strPointer = '\0';
	fclose (write); //close write(index.markdown)

	printf ("\ngenerated indexfile: %s\n",indPth);
	
	for (strPointer = nameOfFiles; **strPointer ; strPointer++) {
		printf("\n");

		//open target(Solutions/xxxx.md)
		strcpy (temp, BOJDIRPATH);
		strcat (temp, "/docs/Solutions/");
		strcat (temp, *strPointer);
		strcat (temp, ".md"); //temp = BOJDIRPATH/Solutions/xxxx.md
		write = fopen ( temp, "w"); //open BOJDIRPATH/Solutions/xxxx.md as write
		printf ("target %s\n", temp);

		//open origin(sol/xxxx.md)
		strcpy(temp, BOJDIRPATH);
		strcat (temp, "/assets/sol/");
		strcat (temp, *strPointer);
		strcat (temp, ".md"); //temp = BOJDIRPATH/assets/sol/xxxx.md
		read = fopen ( temp ,"r"); //open BOJDIRPATH/sol/xxxx.md as read
		printf ("origin %s\n", temp);

		//page <- sol
		while ( (buffer = fgetc(read)) != EOF )
			fputc (buffer, write);
		fclose (read); // close read(sol/xxxx.md)

		//merge sources of srcs
		fputs ("\n## Codes\n", write); 
		for (const LANG* ptr = srcs; ptr->name; ptr++) 
		mergeSource (*ptr, *strPointer);
		fputc ('\n',write);

		fclose (write); //close write(Solutions/xxxx.md)
		printf("target 'Solutions/%s.md' generated!\n",*strPointer);
	}
	printf ("\ncomplete!!\ngenerated files are in ");
	printf ("%s",BOJDIRPATH);
	printf ("/docs/Solutions\n");

	return 0;
}

void mergeSource (LANG source, const char* const prblmNumber) {
	FILE *src;

	strcpy (temp, BOJDIRPATH);
	strcat (temp, "/");
	strcat (temp, source.ext);
	strcat (temp, "/");
	strcat (temp, prblmNumber);
	strcat (temp, ".");
	strcat (temp, source.ext); //temp = BOJDIRPATH/(nameOfSource)/xxxx.(nameOfSource)

		if ( (src = fopen ( temp, "r"))) {// open BOJDIRPATH/(nameOfSource)/xxxx.(nameOfSource) as src
			printf ("merge from %s\n", temp); 
			fputs ("### ", write);
			fputs (source.name, write);
			fputs ("\n```", write);
			fputs (source.name, write);
			fputs ("\n", write);
			while ( (buffer = fgetc(src)) != EOF ) //page <- py
				fputc (buffer, write);
			fputs ("\n```\n", write);
			fclose (src);
		}
		else
			printf("!Does not exists %s\n",temp);
}

int strcmpForqsort (const void* a, const void* b) {
	return strcmp ( (char*)a, (char*)b );
}