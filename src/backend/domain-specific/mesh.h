#ifndef MESH_HEADER
#define MESH_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"

typedef struct {
    unsigned int x;
    unsigned int y;
} Point;

int EvaluateProgram(Program * program, char ** output);
int EvaluateMeshes(MeshItemNode * meshes, char ** output);

#endif

