#ifndef __BUFHH__
#define __BUFHH__ 1

typedef unsigned int uint;

/* string buffer */
typedef struct _bojBuffer {
    uint                allocated;
    char*               allocater;
    char*               bp;
    size_t              size;
    struct _bojBuffer*  next;
} BojBuffer;

void BojBufInit (BojBuffer* buf);
char* BojBufWrite (const char* input, BojBuffer* target);
BojBuffer* BojBufAppend (const char* input, BojBuffer* target);
BojBuffer* BojBufFree (BojBuffer* buf);

#endif