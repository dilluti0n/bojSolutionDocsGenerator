
#include "macro.h"

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

void gitMacro (void) {
	system ("git add -A");
	system ("git commit -m \'auto update by bojSolGen\'");
	system ("git push -u");
}