#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "bison-actions.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogErrorRaw("[ERROR] Mensaje: '%s', debido a '", string);
	for (int i = 0; i < yyleng; ++i) {
		switch (yytext[i]) {
			case '\n':
				LogErrorRaw("\\n");
			default:
				LogErrorRaw("%c", yytext[i]);
		}
	}
	LogErrorRaw("' (length = %d, linea %d).\n\n", yyleng, yylineno);
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
Program * ProgramGrammarAction(MeshItemNode * meshes) {
    Program * program = calloc(1, sizeof(Program));
    program->meshes = meshes;
    state.result = 0;
    state.succeed = true;
    return program;
}

// Function
FunctionNode * FunctionParamsGrammarAction(FunctionNode * first, FunctionNode * second){
    first->next = second;
    return first;
}

FunctionNode * FunctionGrammarAction(MeshItemNode * meshItem) {
    FunctionNode * functionNode = calloc(1, sizeof(FunctionNode));
    functionNode->mesh = meshItem;
    return functionNode;
}

MeshItem FunctionNodeToMeshItem(FunctionNode * functionNode){
    MeshItem mi;
    mi.f = functionNode;
    return mi;
}

// Component

int AssignGrammarAction(const int value, const int rValue) {
	return value;
}

Component * ComponentGrammarAction(const ComponentType value, const Color color, ComponentParamsList * params){
    Component * component = calloc(1, sizeof(Component));
    component->type = value;
    component->color = color;
    component->paramList = params;
	return component;
}


// Params

ComponentParamsList * ComponentParamsGrammarAction(ComponentParams params, const ParamType paramType){
    ComponentParamsList * componentParamsList = calloc(1, sizeof(ComponentParamsList));
    componentParamsList->params = params;
    componentParamsList->type = paramType;
    return componentParamsList;
}

Pair * PairGrammarAction(Value leftValue, ValueType type, Value rightValue) {
    Pair * pair = calloc(1, sizeof(Pair));
    pair->left = leftValue;
    pair->leftType = type;
    pair->right = rightValue.s;
    return pair;
}

// Constantes

Constant * ConstantGrammarAction(Value value, ValueType type){
    Constant * constant = calloc(1, sizeof(Constant));
    constant->value = value;
    constant->type = type;
    return constant;

}

MeshItemNode * MeshGrammarAction(MeshItem meshItem, MeshItemType type, Color color){
    MeshItemNode * meshItemNode = calloc(1, sizeof(MeshItemNode));
    meshItemNode->item = meshItem;
    meshItemNode->itemType = type;
    meshItemNode->color = color;
    return meshItemNode;
}

MeshItemNode * MeshesGrammarAction(MeshItemNode * first, MeshItemNode * second){
    first->next = second;
    return first;
}

PairNode * PairsGrammarAction(Pair * first, PairNode * second) {
    PairNode * pairNode = calloc(1, sizeof(PairNode));
    pairNode->pair = first;
    pairNode->next = second;
	return pairNode;
}

int VariablesGrammarAction(const int value) {
	return value;
}

int IdentifierVariableGrammarAction(const int value) {
	return value;
}

int FunctionsGrammarAction(const int value) {
	return value;
}
