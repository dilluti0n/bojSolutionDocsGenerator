#ifndef __CRAWLERHH__
#define __CRAWLERHH__ 1

#include <curl/curl.h>
#include <tidy/tidy.h>
#include <tidy/tidybuffio.h>

typedef struct {
    char* id;
    char* file;
    char* header;
} DOCS;

extern char temp[255];
extern DOCS macro[4];

size_t bufferCallback(char * buffer, size_t size, size_t nmemb, TidyBuffer * tidyBuffer);

void getTitle(TidyDoc doc, TidyNode tnod, char* title);

int titleWriter (char* title, char* prblmNumber);

#endif