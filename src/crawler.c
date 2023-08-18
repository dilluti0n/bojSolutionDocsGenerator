#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "crawler.h"

extern DOCS macro[3];

/* curl write callback, to fill tidy's input buffer...  */
uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out) {
	uint r;
	r = size * nmemb;
	tidyBufAppend(out, in, r);
	return r;
}

/* Traverse the document tree */
void getTitle(TidyDoc doc, TidyNode tnod, char* title) {
	TidyNode child;
	FILE* target;
	_Bool ind = 1;

	for(child = tidyGetChild(tnod); child; child = tidyGetNext(child) ) {
		ctmbstr name = tidyNodeGetName(child);
		if(name) {
			/* if it has a name, then it's an HTML tag ... */
			for(TidyAttr attr = tidyAttrFirst(child); attr; attr = tidyAttrNext(attr) )
				if ( tidyAttrValue(attr)) {
					if ( ind && !strcmp(tidyAttrValue(attr), "problem_title") ) {
						TidyBuffer buf;
						tidyBufInit(&buf);
						tidyNodeGetText(doc, tidyGetChild(child), &buf);
						strcpy(title, (char*) buf.bp);
						tidyBufFree(&buf);
						ind = 0;
					}
					else for (DOCS* docPointer = macro; docPointer->id; docPointer++)
						if ( !strcmp(tidyAttrValue(attr), docPointer->id)) {
							TidyNode node, nodeChild;
							target = fopen (docPointer->file, "w");
							for (node = tidyGetChild(child); node; node = tidyGetNext(node)) {
								nodeChild = tidyGetChild(node);
								ctmbstr nodeName = tidyNodeGetName(nodeChild);
								if ( !nodeName ) {
									TidyBuffer buf;
									tidyBufInit(&buf);
									tidyNodeGetText(doc, nodeChild, &buf);
									fputs (buf.bp?(char *)buf.bp:"", target);
									fputc ('\n', target);
									tidyBufFree(&buf);
								}
								else if ( !strcmp(nodeName, "img")){
									fputs ("<center>",target);
									fputc ('<',target);
									fputs (nodeName, target);
									fputc (' ', target);
									for (TidyAttr imgAttr = tidyAttrFirst(nodeChild); imgAttr; imgAttr = tidyAttrNext(imgAttr)) {
										fputs (tidyAttrName(imgAttr), target);
										if (!strcmp(tidyAttrName(imgAttr), "src")) {
											fputs("=\"https://www.acmicpc.net", target);
											fputs(tidyAttrValue(imgAttr), target);
											fputs("\" ",target);
										}
										else
											tidyAttrValue(imgAttr)?fprintf(target, "=\"%s\" ",tidyAttrValue(imgAttr)):fputs(" ",target);
									}
									fputs ("></center>\n\n", target);
								}
							}
							fclose (target);
							return;
						}
			}	
		}
		getTitle(doc, child, title); /* recursive */
	}
}

int titleWriter (char* title, char* prblmNumber) {
		CURL *curl;
		char curl_errbuf[CURL_ERROR_SIZE];
		TidyDoc tdoc;
		TidyBuffer docbuf = {0};
		TidyBuffer tidy_errbuf = {0};
		int err;

		strcpy (temp, "https://www.acmicpc.net/problem/");
		strcat (temp, prblmNumber);
 
		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, temp);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
 
		tdoc = tidyCreate();
		tidySetCharEncoding(tdoc, "utf8");
		tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */
		tidyOptSetInt(tdoc, TidyWrapLen, 4096);
		tidySetErrorBuffer(tdoc, &tidy_errbuf);

		tidyBufInit(&docbuf);
 
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
		err = curl_easy_perform(curl);
		if(!err) {
			err = tidyParseBuffer(tdoc, &docbuf); /* parse the input */
			if(err >= 0) {
				err = tidyCleanAndRepair(tdoc); /* fix any problems */
				if(err >= 0) {
					err = tidyRunDiagnostics(tdoc); /* load tidy error buffer */
					if(err >= 0) 
						getTitle(tdoc, tidyGetRoot(tdoc), title); /* walk the tree */
				}
			}
		}

		curl_easy_cleanup(curl);
		tidyBufFree(&docbuf);
		tidyBufFree(&tidy_errbuf);
		tidyRelease(tdoc);
		return err;
}