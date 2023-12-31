#include "symbol-table.h"

// Function to add a variable to the map
int add_variable(khash_t(comp) *map, const char *name, const Component * component) {
    int ret;
    khiter_t iter = kh_put(comp, map, name, &ret);
    if (ret != 0) {
        Comp var;
        var.identifier = strdup(name);
        var.component = malloc(sizeof(Component));
        var.component->type = component->type;
        var.component->color = component->color;
        var.component->paramList = component->paramList;
        kh_value(map, iter) = var;
        LogDebug("Variable '%s' added to the map. Color: '%d', Type: '%d'\n", var.identifier, var.component->color, var.component->type);
    }
    return ret;
}

// Function to retrieve a variable from the map
Comp* get_variable(khash_t(comp) *map, const char *name) {
    khiter_t iter = kh_get(comp, map, name);
    if (iter != kh_end(map)) {
        return &kh_value(map, iter);
    } else {
        LogDebug("Variable '%s' not found.\n", name);
        return NULL;
    }
}

// Function to remove a variable from the map
void remove_variable(khash_t(comp) *map, const char *name) {
    khiter_t iter = kh_get(comp, map, name);
    if (iter != kh_end(map)) {
        free(kh_value(map, iter).identifier); 
        free(kh_value(map, iter).component); 
        kh_del(comp, map, iter);
        LogDebug("Variable '%s' removed from the map.\n", name);
    } else {
        LogDebug("Variable '%s' not found.\n", name);
    }
}

// Function to clean up the map and free memory
void cleanup_variables(khash_t(comp) *map) {
    khiter_t iter;
    for (iter = kh_begin(map); iter != kh_end(map); ++iter) {
        if (kh_exist(map, iter)) {
            free(kh_value(map, iter).identifier);  
            free(kh_value(map, iter).component); 
        }
    }
    kh_destroy(comp, map);
}
