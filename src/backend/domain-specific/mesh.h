#ifndef MESH_HEADER
#define MESH_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"
#include "buffer.h"

#define MAX_STR 256


Rectangle * EvaluateProgram(Program * program, char ** output);

#endif

