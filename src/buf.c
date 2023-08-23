#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "buf.h"

/* init bojbuffer */
void BojBufInit (BojBuffer* buf) {
    buf->next = (BojBuffer*) NULL;
}

/* write string to bojbuffer */
char* BojBufWrite (const char* input, BojBuffer* target) {
    target->size = strlen (input) + 1;
    target->allocated = target->size * sizeof (char);
    target->allocater = (char*) malloc (target->allocated);
	target->bp = target->allocater;
    strcpy (target->bp, input);

    return target->allocater;
}

/* append string to bojbuffer */
BojBuffer* BojBufAppend (const char* input, BojBuffer* target) {
    while ( target->next )
        target = target->next;
    target->next = (BojBuffer*) malloc (sizeof (BojBuffer));

    if ( target->next )
        target->next->next = (BojBuffer*) NULL;
    BojBufWrite (input, target->next);

    return target->next;
}

/* free all memory used by bojbuffer */
BojBuffer* BojBufFree (BojBuffer* buf) {
    while ( (buf = buf->next) ) {
        free (buf->allocater);
        free (buf->next);
    }
    return buf;
}