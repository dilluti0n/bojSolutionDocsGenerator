#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define BOJDIRPATH "/Users/hskimse/Desktop/백준"
#define NUMOFPROBLEMS 100


int main ( int argc, char* argv[] ) {
	DIR 			*dir_info;
	struct dirent   *dir_entry;
	FILE 			*sol, *c, *py, *inDex, *page;
	
	char nameOfFiles [NUMOFPROBLEMS][256];
	char Buffer[250], temp[256];
	char (*charPointer)[256] = nameOfFiles;

	void assignString ( char* a, const char* b );
	
	strcpy(temp, BOJDIRPATH);
	assignString (temp, "/docs/index.markdown");
	inDex = fopen ( temp, "w");
	fprintf(inDex, "---\nlayout: home\n---\n# Solutions\n");
	
	dir_info = opendir ( BOJDIRPATH"/sol" );
	if ( dir_info != NULL ) {
		while ( (dir_entry = readdir ( dir_info ))) {
			if ( dir_entry->d_name[0] != '.') {
				strcpy(*charPointer++, dir_entry->d_name);
				fprintf ( inDex,"[%s](http://dilluti0n.github.io/baekjun/%s) ", dir_entry->d_name,dir_entry->d_name);
			}
			else 
				--charPointer;
		strcpy(*charPointer,"\0") ;
		}
		closedir(dir_info);
	}
	fclose (inDex);
	
	charPointer = nameOfFiles;
	while ( !*charPointer ) {
		strcpy(temp, BOJDIRPATH);
		assignString (temp, "sol/");
		assignString (temp, *charPointer);
		sol = fopen ( temp ,"r");
		fgets (Buffer, 250, sol);
		fclose (sol);
	}
		
}

void assignString ( char* a, const char* b ) {
	for (; *a != '\0'; ++a);
	for (; *b != '\0'; ++b , ++a )
		*a = *b ;
	*a = '\0';
}