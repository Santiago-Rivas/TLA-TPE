#ifndef BUFFER_HEADER
#define BUFFER_HEADER

#define BUFFER_BLOCK 1000

typedef struct PointList PointList;

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
    unsigned int level;
    unsigned int outerSeries;
    Point currentPoint;
    Buffer * buf;
} Pencil;

typedef struct {
    Point p1;
    Point p2;
} Rectangle;

typedef struct PointList {
    Point point;
    PointList * nextPoint;
} PointList;

Buffer * BufferInit();
void BufferFree(Buffer * buf);
int ConcatString(Buffer * buf, char * string);
int ConcatStringWithLength(Buffer * buf, char * string, int length);

#endif
