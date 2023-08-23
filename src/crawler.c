#include <string.h>
#include <stdio.h>

#include "crawler.h"

/* curl write callback, to fill tidy's input buffer...  */
size_t write_cb(char *in, size_t size, size_t nmemb, TidyBuffer *out) {
	size_t r;
	r = size * nmemb;
	tidyBufAppend(out, in, r);
	return r;
}

/* Traverse the document tree */
void htmlWalker(TidyDoc doc, TidyNode tnod, char* title) {

	for(TidyNode child = tidyGetChild(tnod); child; child = tidyGetNext(child) ) {
		if ( tidyNodeGetName(child) ) {
			for(TidyAttr attr = tidyAttrFirst(child); attr; attr = tidyAttrNext(attr) ) {
				ctmbstr attrVal = tidyAttrValue(attr);
				if ( attrVal ) {
					if ( !strcmp(attrVal, "problem_title") ) {
						TidyBuffer buf;
						tidyBufInit(&buf);
						tidyNodeGetText(doc, tidyGetChild(child), &buf);
						strcpy(title, (char*) buf.bp);
						tidyBufFree(&buf);
					}
<<<<<<< HEAD
					/*if the html tag's ID is docPointer->id, load elements under that tag.*/
					else for (DOCS* docPointer = macro; docPointer->id; docPointer++)
=======
					/*if the html tag's ID is docPointer->id, Scrapes all the text and image elements under that tag.*/
					else for (DOCS* docPointer = macro; docPointer->id; docPointer++) 
>>>>>>> crawler
						if ( !strcmp(attrVal, docPointer->id) ) {
							BojBufInit (&docPointer->io);
							htmlLoader(doc, child, docPointer);
							return;
						}
				}
			}
		}
		htmlWalker(doc, child, title); /* recursive */
	}
}

<<<<<<< HEAD
/*copy all the text and images under tnod*/
void htmlLoader(TidyDoc doc, TidyNode tnod, FILE* target) {
=======
/*Traverse and copy all the text and images*/
void htmlLoader(TidyDoc doc, TidyNode tnod, DOCS* docPointer) {
>>>>>>> crawler
	for (TidyNode child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
		ctmbstr childName = tidyNodeGetName (child);
		/*text*/
		if ( !childName ) {
			TidyBuffer buf;
			tidyBufInit(&buf);
			tidyNodeGetText(doc, child, &buf);
			BojBufAppend(buf.bp?(char *)buf.bp:"", &docPointer->io);
			BojBufAppend("\n", &docPointer->io);
			tidyBufFree(&buf);
		}
		/*image*/
		else if ( !strcmp(childName, "img")){
			BojBufAppend ("<center>", &docPointer->io);
			BojBufAppend ("<", &docPointer->io);
			BojBufAppend (childName, &docPointer->io);
			BojBufAppend (" ", &docPointer->io);
			for (TidyAttr imgAttr = tidyAttrFirst(child); imgAttr; imgAttr = tidyAttrNext(imgAttr)) {
				BojBufAppend (tidyAttrName(imgAttr), &docPointer->io);
				if (!strcmp(tidyAttrName(imgAttr), "src")) {
					BojBufAppend("=\"https://www.acmicpc.net", &docPointer->io);
					BojBufAppend(tidyAttrValue(imgAttr), &docPointer->io);
					BojBufAppend("\" ", &docPointer->io);
				}
				else if ( tidyAttrValue(imgAttr) ) {
					BojBufAppend ("=\"", &docPointer->io);
					BojBufAppend (tidyAttrValue(imgAttr), &docPointer->io);
					BojBufAppend ("\"", &docPointer->io);
				}
				else
					BojBufAppend(" ",&docPointer->io);
			}
			BojBufAppend ("></center>\n\n", &docPointer->io);
		}
		htmlLoader(doc, child, docPointer);
	}
}

int titleWriter (char* title, char* prblmNumber) {
	CURL *curl;
	char curl_errbuf[CURL_ERROR_SIZE];
	TidyDoc tdoc;
	TidyBuffer docbuf = {0};
	TidyBuffer tidy_errbuf = {0};
	int err = -1;

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
	tidyOptSetInt(tdoc, TidyWrapLen, 4096);

	tidyBufInit(&docbuf);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);

	if ( curl_easy_perform(curl) == CURLE_OK ) {
		err = tidySetErrorBuffer(tdoc, &tidy_errbuf);
		if (err >= 0)
			err = tidyParseBuffer(tdoc, &docbuf); 
		if (err >= 0) 
			err = tidyCleanAndRepair(tdoc); 
		if (err >= 0) 
			err = tidyRunDiagnostics(tdoc); 
		if (err > 1)
			err = ( tidyOptSetBool(tdoc, TidyForceOutput, yes)?err:-1 );
		if (err >= 0) {
			htmlWalker(tdoc, tidyGetRoot(tdoc), title);
		}
		tidyBufFree(&tidy_errbuf); 	
	}

	curl_easy_cleanup(curl);
	tidyBufFree(&docbuf);
	tidyRelease(tdoc);
	
	return err;
}