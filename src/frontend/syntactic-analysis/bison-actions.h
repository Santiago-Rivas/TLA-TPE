#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
int ProgramGrammarAction(const int value);

//Function
int FunctionGrammarAction(const int value);

// Component.
int AssignGrammarAction(const int value, const int rValue);
int ComponentGrammarAction(const int value);

// Params.
int PairParamsGrammarAction(const int value);
int ExpressionParamsGrammarAction(const int value);
int ConstantParamsGrammarAction(const int value);
int FullSizeParamsGrammarAction(const int value1, const char* value2);

// Pair
int PairIntegerGrammarAction(const int value1, const char* value2);
int PairStringGrammarAction(const char* value1, const char* value2);

// Constantes.
int IntegerConstantGrammarAction(const int value);
int IdentifierConstantGrammarAction(const int value);

int FunctionParamsGrammarAction(const int value); 

int MeshGrammarAction(const int value); 

int MeshesGrammarAction(const int value);

int ComponentsGrammarAction(const int value); 

int PairsGrammarAction(const int value); 

int VariablesGrammarAction(const int value);

int IdentifierVariableGrammarAction(const int value);
    
int FunctionsGrammarAction(const int value);

#endif
