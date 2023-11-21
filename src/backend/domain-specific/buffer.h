#ifndef BUFFER_HEADER
#define BUFFER_HEADER

#define BUFFER_BLOCK 100

typedef struct Buffer {
    char * str;
    int currentLen;
    int maxLen;
} Buffer;

typedef struct {
    unsigned int x;
    unsigned int y;
} Point;

typedef struct {
    Point currentPoint;
    unsigned int level;
    Buffer * buf;
} Pencil;


Buffer * BufferInit();
void BufferFree(Buffer * buf);
int ConcatString(Buffer * buf, char * string);
int ConcatStringWithLength(Buffer * buf, char * string, int length);

#endif
