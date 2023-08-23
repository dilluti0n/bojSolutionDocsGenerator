#ifndef __CRAWLERHH__
#define __CRAWLERHH__ 1

#include <curl/curl.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>

#include "buf.h"

typedef struct {
    char*       id;
    char*       file;
    char*       header;
    BojBuffer   io;
} DOCS;

extern char temp[255];
extern DOCS macro[4];
extern 

size_t  write_cb(char *in, size_t size, size_t nmemb, TidyBuffer *out);
void    htmlLoader(TidyDoc doc, TidyNode tnod, DOCS* docPointer);
void    htmlWalker(TidyDoc doc, TidyNode tnod, char* title);
int     titleWriter (char* title, char* prblmNumber);

#endif