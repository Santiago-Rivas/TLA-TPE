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

// Component.
int AssignGrammarAction(const int value, const int rValue);
int ComponentGrammarAction(const int value);

// Params.
int PairParamsGrammarAction(const int value);
int ExpressionParamsGrammarAction(const int value);
int ConstantParamsGrammarAction(const int value);
int FullSizeParamsGrammarAction(const int value1, const int value2);

// Pair
int PairGrammarAction(const int value1, const int value2);

// Constantes.
int IntegerConstantGrammarAction(const int value);
int IdentifierConstantGrammarAction(const int value);

#endif
