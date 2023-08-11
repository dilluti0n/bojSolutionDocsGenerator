#pragma once

#include <curl/curl.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>

extern char temp[255];

size_t bufferCallback(char * buffer, size_t size, size_t nmemb, TidyBuffer * tidyBuffer);

void getTitle(TidyDoc doc, TidyNode tnod, char* title);

int titleWriter (char* title, char* prblmNumber);