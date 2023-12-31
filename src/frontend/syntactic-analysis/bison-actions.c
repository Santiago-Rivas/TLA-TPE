#include "bison-actions.h"
#include "../../backend/domain-specific/mesh.h"
#include "../../backend/support/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "bison-actions.h".
 */

/**
 * Esta función se ejecuta cada vez que se emite un error de sintaxis.
 */
void yyerror(const char *string) {
    LogErrorRaw("[ERROR] Message: '%s', because of '", string);
    for (int i = 0; i < yyleng; ++i) {
        switch (yytext[i]) {
            case '\n':
                LogErrorRaw("\\n");
            default:
                LogErrorRaw("%c", yytext[i]);
        }
    }
    LogErrorRaw("' (length = %d, line %d).\n\n", yyleng, yylineno);
}

/**
 * Esta acción se corresponde con el no-terminal que representa el símbolo
 * inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
 * indica que efectivamente el programa de entrada se pudo generar con esta
 * gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
 */
Program *ProgramGrammarAction(MeshItemNode *meshes) {
    Program *program = calloc(1, sizeof(Program));
    if (program == NULL) {
        state.succeed = false;
        return NULL;
    }
    program->meshes = meshes;

    state.program = program;

    state.result = 0;
    state.succeed = true;

    return program;
}

Program * ProgramVariableGrammarAction(int variableCreationReturn, MeshItemNode * meshes) {
    if (variableCreationReturn == 0) {
        LogDebug("Variable Exists");
        state.succeed = false;
        state.result = 3;
        return NULL;
    }
    return ProgramGrammarAction(meshes);
}

// Function
FunctionNode *FunctionParamsGrammarAction(FunctionNode *first,
                                          FunctionNode *second) {
    first->next = second;
    return first;
}

FunctionNode *FunctionGrammarAction(MeshItemNode *meshItem) {
    FunctionNode *functionNode = calloc(1, sizeof(FunctionNode));
    if (functionNode == NULL) {
        return NULL;
    }
    functionNode->mesh = meshItem;
    if (meshItem->itemType == MESH_FUNCTION) {
        functionNode->level = meshItem->item.f->level + 1;
    } else {
        functionNode->level = 0;
    }
    return functionNode;
}

MeshItem * FunctionNodeToMeshItem(FunctionNode *functionNode) {
    MeshItem * mi = calloc(1, sizeof(MeshItem));
    if (mi == NULL) {
        return NULL;
    }
    mi->f = functionNode;
    return mi;
}

// Component

int AssignGrammarAction(const int value, const int rValue) { return value; }

MeshItem * ComponentGrammarAction(const ComponentType value, const Color color, ComponentParamsList *params) {
    MeshItem * meshItem = calloc(1, sizeof(MeshItem));
    if (meshItem == NULL) {
        return NULL;
    }
    meshItem->c = calloc(1, sizeof(Component));
    if (meshItem->c == NULL) {
        return NULL;
    }
    meshItem->c->type = value;
    meshItem->c->color = color;
    meshItem->c->paramList = params;
    return meshItem;
}

// Params

ComponentParamsList *ComponentParamsGrammarAction(ComponentParams * params, const ParamType paramType) {
    ComponentParamsList *componentParamsList = calloc(1, sizeof(ComponentParamsList));
    if (componentParamsList == NULL) {
        return NULL;
    }
    componentParamsList->params = *params;
    componentParamsList->type = paramType;
    return componentParamsList;
}

Pair *PairGrammarAction(Value leftValue, ValueType type, Value rightValue) {
    Pair *pair = calloc(1, sizeof(Pair));
    if (pair == NULL) {
        return NULL;
    }
    pair->left = leftValue;
    pair->leftType = type;
    pair->right = rightValue.s;
    return pair;
}

// Constantes

ComponentParams * ConstantGrammarAction(Value value, ValueType type) {
    ComponentParams * componentParams = calloc(1, sizeof(ComponentParams));
    if (componentParams == NULL) {
        return NULL;
    }
    componentParams->c = calloc(1, sizeof(Constant));
    if (componentParams->c == NULL) {
        return NULL;
    }
    componentParams->c->value = value;
    componentParams->c->type = type;
    return componentParams;
}

MeshItemNode *MeshGrammarAction(MeshItem * meshItem, MeshItemType type, Color color) {
    MeshItemNode *meshItemNode = calloc(1, sizeof(MeshItemNode));
    if (meshItemNode == NULL) {
        return NULL;
    }
    if (type == MESH_IDENTIFIER) {
        Comp * comp = get_variable(state.map, meshItem->s);
        if (comp == NULL) {
            return NULL;
        }
        Component * component = malloc(sizeof(Component));
        component->type = comp->component->type;
        component->paramList = comp->component->paramList;
        if (color == BLACK) {
            component->color = comp->component->color;
        } else {
            component->color = color;
        }

        meshItem->c = component;
        meshItemNode->item = *meshItem;
        meshItemNode->itemType = MESH_COMPONENT;
    } else {
        meshItemNode->item = *meshItem;
        meshItemNode->itemType = type;
    }
    if (color == BLACK) {
        meshItemNode->color = meshItem->c->color;
    } else {
        meshItemNode->color = color;
    }
    return meshItemNode;
}

MeshItemNode *MeshesGrammarAction(MeshItemNode *first, MeshItemNode *second) {
    if (first == NULL) {
        return NULL;
    }
    first->next = second;
    return first;
}

ComponentParams * PairsGrammarAction(Pair *first, ComponentParams * second) {
    ComponentParams * componentParams = calloc(1, sizeof(ComponentParams));
    if (componentParams == NULL) {
        return NULL;
    }
    componentParams->p = calloc(1, sizeof(PairNode));
    if (componentParams->p == NULL) {
        return NULL;
    }
    componentParams->p->pair = first;
    if (second == NULL) {
        componentParams->p->next = NULL;
    } else {
        componentParams->p->next = second->p;
    }
    return componentParams;
}

int VariablesGrammarAction(Variable * variable, int pastRet) {
    if (variable == NULL || pastRet == 0) {
        LogDebug("Variable Already Exists");
        return 0;
    }
    return 1;
}

Variable * IdentifierVariableGrammarAction(MeshItem * identifier, MeshItem * meshItem) {
    Variable * variable = calloc(1, sizeof(Variable));
    if (variable == NULL) {
        return NULL;
    }
    variable->identifier = strdup(identifier->s);
    variable->component = meshItem->c;
    int res = add_variable(state.map, variable->identifier, variable->component);
    if(res == 0) {
        LogError("Variable '%s' already exists.", variable->identifier);
        state.succeed = false;
        state.result = 3;
        return NULL;
    } else{
        LogDebug("Variable '%s' added to the map. Color: '%d', Type: '%d'\n", variable->identifier, variable->component->color, variable->component->type);
    }
    return variable;
}

