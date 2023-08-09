#include "main.h"

LANG	srcs[] = NAMEOFSOURCES;
char 	(*strPointer)[16] = nameOfFiles;

int main ( int argc, char* argv[] ) {
	printf("------------------------\nbojSolutionDocsGenerator\n------------------------\n\n");

	#ifndef BOJDIRPATH
		printf ("Enter your directory's path: ");
		scanf ("%s", BOJDIRPATH);
	#endif

	strcpy (temp, BOJDIRPATH);
	strcat (temp, "/assets/origin.md");
	if ( !(read = fopen ( temp, "r")) ) { // open BOJDIRPATH/assets/origin.md as read
		printf ("\n[error] Wrong Path or assets/origin.md does not exists !!!\n");
		return -1 ;
	}
	strcpy (indPth, BOJDIRPATH);
	strcat (indPth, "/docs/index.markdown");
	write = fopen ( indPth, "w"); // open BOJDIRPATH/docs/index.markdown as write

	while ( (buffer = fgetc(read)) != EOF ) //index.md <- origin
			fputc (buffer, write);
	fclose (read); //close read(origin.md)
	
	strcpy (temp, BOJDIRPATH);
	strcat (temp, "/assets/sol");

	if ( !(dir_info = opendir ( temp )))  {
		printf("There is no file to merge!!\n"); // open BOJDIRPATH/assets/sol as dir_info
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
	**strPointer = '\0';

	printf ("target problems are:");
	for (strPointer = nameOfFiles; **strPointer ; strPointer++) {
		printf(" %s", *strPointer);
		fprintf ( write,"[");
		fprintf ( write, "%s](Solutions/%s.html) ",*strPointer, *strPointer );
	}

	fclose (write); //close write(index.markdown)

	printf ("\ngenerated indexfile: %s\n",indPth);

	cnt = 1;
	for (strPointer = nameOfFiles; **strPointer ; strPointer++,cnt++) {
		printf("\n");

		//open write(Solutions/xxxx.md)
		strcpy (temp, BOJDIRPATH);
		strcat (temp, "/docs/Solutions/");
		strcat (temp, *strPointer);
		strcat (temp, ".md"); //temp = BOJDIRPATH/Solutions/xxxx.md
		write = fopen ( temp, "w"); //open BOJDIRPATH/Solutions/xxxx.md as write
		printf ("target %s\n", temp);

		//open read(sol/xxxx.md)
		strcpy (temp, BOJDIRPATH);
		strcat (temp, "/assets/sol/");
		strcat (temp, *strPointer);
		strcat (temp, ".md"); //temp = BOJDIRPATH/assets/sol/xxxx.md
		read = fopen ( temp ,"r"); //open BOJDIRPATH/sol/xxxx.md as read
		printf ("origin %s\n", temp);

		//put yaml front to Solutions/xxxx.md
		fprintf(write, "---\nlayout: page\ntitle: %s\nparent: Solutions\nnav_order: %i\n---\n", *strPointer, cnt);

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