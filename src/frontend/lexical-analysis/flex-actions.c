#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

char * copyLexeme(const char * lexeme, const int length) {
	char * lexemeCopy = (char *) calloc(length + 1, sizeof(char));
	strncpy(lexemeCopy, lexeme, length);
	return lexemeCopy;
}

void BeginCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] BeginCommentPatternAction............................");
}

void EndCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] EndCommentPatternAction..............................");
}

token ComponentOperatorPatternAction(const char* lexeme, const int lenght) {
	char* lexemeCopy = copyLexeme(lexeme, lenght);
	LogDebug("[Flex] ComponentOperatorPatternAction: %s", lexemeCopy);
	free(lexemeCopy);
	yylval.component = COMPONENT;
	return COMPONENT;
}

token CommaPatternaction() {
	LogDebug("[Flex] CommaPatternaction: ','.");
	yylval.token = COMMA;
	return COMMA;
}

token OpenBracketsPatternAction() {
	LogDebug("[Flex] OpenBracketsPatternAction: '{'.");
	yylval.token = OPEN_BRACKET;
	return OPEN_BRACKET;
}

token CloseBracketsPatternAction() {
	LogDebug("[Flex] CloseBracketsPatternAction: '}'.");
	yylval.token = CLOSE_BRACKET;
	return CLOSE_BRACKET;
}

token OpenSquareBracketsPatternAction() {
	LogDebug("[Flex] OpenSquareBracketsPatternAction: '['.");
	yylval.token = OPEN_SQUAREDBRACKET;
	return OPEN_SQUAREDBRACKET;
}

token CloseSquareBracketsPatternAction() {
	LogDebug("[Flex] CloseSquareBracketsPatternAction: ']'.");
	yylval.token = CLOSE_SQUAREDBRACKET;
	return CLOSE_SQUAREDBRACKET;
}

token OpenParenthesisPatternAction() {
	LogDebug("[Flex] OpenParenthesisPatternAction: '('.");
	yylval.token = OPEN_PARENTHESIS;
	return OPEN_PARENTHESIS;
}

token CloseParenthesisPatternAction() {
	LogDebug("[Flex] CloseParenthesisPatternAction: ')'.");
	yylval.token = CLOSE_PARENTHESIS;
	return CLOSE_PARENTHESIS;
}

token IntegerPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] IntegerPatternAction: '%s' (length = %d).", lexemeCopy, length);
	free(lexemeCopy);
	yylval.integer = INTEGER;
	return INTEGER;
}

token AssignOperatorPatternAction() {
	LogDebug("[Flex] AssignOpperatorPatternAction: '='.");
	yylval.constant = ASSIGN;
	return ASSIGN;
}

token IdentifierPatterAction(const char * lexeme, const int length) {
	char* lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] IdentifierPatternAction: '%s' (lenght = %d).", lexemeCopy, length);
	free(lexemeCopy);
	yylval.identifier = (char*) IDENTIFIER;
	return IDENTIFIER;
}

token StartPatternAction(const char * lexeme, const int length) {
	char* lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] StartPatternAction: 'start'.");
	free(lexemeCopy);
	yylval.beggin = BEGGIN;
	return BEGGIN;
}

token MeshPatternAction(const char * lexeme, const int length) {
	char* lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] MeshPatternAction: 'mesh'.");
	free(lexemeCopy);
	yylval.mesh = MESH;
	return MESH;
}
token EndPatternAction(const char * lexeme, const int length) {
	char* lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] EndPatternAction: 'end'.");
	free(lexemeCopy);
	yylval.end = END;
	return END;
}

token EndLinePatternAction() {
	LogDebug("[Flex] EndLinePatternAction: '\\n'.");
	yylval.newLine = NEWLINE;
	return NEWLINE;
}

token FunctionPatternAction(const char * lexeme, const int length) {
	char* lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] FunctionPatternAction: '%s'.", lexemeCopy);
	free(lexemeCopy);
	yylval.function = FUNCTION;
	return FUNCTION;

}


token StringPatternAction(const char * lexeme, const int length) {
	char* lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] StringPatternAction: '%s'.", lexemeCopy);
	free(lexemeCopy);
	yylval.string = (char*) STRING;
	return STRING;
}


token ColorComponentOperatorPatternAction(const char* lexeme, const int length) {
	char* lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] ColorComponentOperatorPatternAction: '%s'.", lexemeCopy);
	free(lexemeCopy);
	yylval.color = COLOR;
	return COLOR;
}

token UnknownPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
	free(lexemeCopy);
	yylval.token = ERROR;
	// Al emitir este token, el compilador aborta la ejecución.
	return ERROR;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
	LogText(lexemeCopy, length);
	LogRaw("' (length = %d).\n", length);
	free(lexemeCopy);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
	// No se emite ningún token.
}
