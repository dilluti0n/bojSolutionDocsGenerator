#ifndef __CRAWLERHH__
#define __CRAWLERHH__ 1

#include <curl/curl.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>

typedef struct {
    char* id;
    char* file;
    char* header;
} DOCS;

extern char temp[255];
extern DOCS macro[4];

size_t  write_cb(char *in, size_t size, size_t nmemb, TidyBuffer *out);
void    htmlLoader(TidyDoc doc, TidyNode tnod, FILE* target);
void    htmlWalker(TidyDoc doc, TidyNode tnod, char* title);
int     titleWriter (char* title, char* prblmNumber);

#endif