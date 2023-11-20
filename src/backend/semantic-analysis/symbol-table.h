#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "khash.h"
#include "abstract-syntax-tree.h"
#include <stdio.h>

// Define var type
typedef struct {
    char * identifier;
    Component * component;
} Comp;

// Hash table for variables
KHASH_MAP_INIT_STR(comp, Comp);

void add_variable(khash_t(comp) *map, const char *name, const Component * component);
Comp* get_variable(khash_t(comp) *map, const char *name);
void remove_variable(khash_t(comp) *map, const char *name);
void cleanup_variables(khash_t(comp) *map);

#endif