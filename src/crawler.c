#include <curl/curl.h>

#include "main.h"

void titleWrite ( char* title, char* prblmNumber ) {
    CURL* handle;

    handle = curl_easy_init();
    strcpy (temp, "https://www.acmicpc.net/problem/");
    strcat (temp, prblmNumber);
    if (handle) {
        CURLcode res;
        curl_easy_setopt (handle, CURLOPT_URL, temp);
        res = curl_easy_perform (handle);
        curl_easy_cleanup (handle);
    }

    
} 