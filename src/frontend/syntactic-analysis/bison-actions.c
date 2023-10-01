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
int ProgramGrammarAction(const int value) {
	LogDebug("[Bison] ProgramGrammarAction(%d)", value);
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	state.result = value;
	return value;
}

// Component

int AssignGrammarAction(const int value, const int rValue) {
	LogDebug("[Bison] AssignExpressionGrammarAction(%d = %d)", value, rValue);
	return value;
}

int ComponentGrammarAction(const int value) {
	LogDebug("[Bison] ComponentExpressionGrammarAction(%d)", value);
	return value;
}

// Params

int PairParamsGrammarAction(const int value) {
	LogDebug("[Bison] PairParamsGrammarAction(%d)", value);
	return value;
}

int ExpressionParamsGrammarAction(const int value) {
	LogDebug("[Bison] ExpressionFactorGrammarAction(%d)", value);
	return value;
}

int ConstantParamsGrammarAction(const int value) {
	LogDebug("[Bison] ConstantFactorGrammarAction(%d)", value);
	return value;
}


int FullSizeParamsGrammarAction(const int value1, const int value2) {
	LogDebug("[Bison] FullSizeParamsGrammarAction({%d} %d)", value1, value2);
	return value1;
}

// Pair

int PairGrammarAction(const int value1, const int value2) {
	LogDebug("[Bison] PairFactorGrammarAction({%d, %d})", value1, value2);
	return value1;
}

// Constantes

int IntegerConstantGrammarAction(const int value) {
	LogDebug("[Bison] IntegerConstantGrammarAction(%d)", value);
	return value;
}

int IdentifierConstantGrammarAction(const int value) {
	LogDebug("[Bison] IdentifierConstantGrammarAction(%d)", value);
	return value;
}
