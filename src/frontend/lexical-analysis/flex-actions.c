#include "flex-actions.h"
#include "../../backend/support/logger.h"
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

char *copyLexeme(const char *lexeme, const int length) {
    char *lexemeCopy = (char *)calloc(length + 1, sizeof(char));
    if (lexemeCopy == NULL) {
        return NULL;
    }
    strncpy(lexemeCopy, lexeme, length);
    return lexemeCopy;
}

void BeginCommentPatternAction() {
    LogDebug(
        "[Flex] [COMMENT] BeginCommentPatternAction............................");
}

void EndCommentPatternAction() {
    LogDebug(
        "[Flex] [COMMENT] EndCommentPatternAction..............................");
}

token ComponentOperatorPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] ComponentOperatorPatternAction: %s", lexemeCopy);

    ComponentType type;

    if (strcmp(lexemeCopy, "Resistor") == 0) {
        type = RESISTOR;
    } else if (strcmp(lexemeCopy, "Battery") == 0) {
        type = BATTERY;
    } else if (strcmp(lexemeCopy, "Cable") == 0) {
        type = CABLE;
    } else if (strcmp(lexemeCopy, "Inductor") == 0) {
        type = INDUCTOR;
    } else if (strcmp(lexemeCopy, "Ammeter") == 0) {
        type = AMMETER;
    } else if (strcmp(lexemeCopy, "Voltmeter") == 0) {
        type = VOLTMETER;
    } else if (strcmp(lexemeCopy, "Switch") == 0) {
        type = SWITCH;
    } else if (strcmp(lexemeCopy, "Capacitor") == 0) {
        type = CAPACITOR;
    } else if (strcmp(lexemeCopy, "Led") == 0) {
        type = LED;
    } else {
        type = CABLE;
    }
    yylval.componentType = type;
    return COMPONENT;
}

token CommaPatternAction() {
    LogDebug("[Flex] CommaPatternAction: ','.");
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
    yylval.token = OPEN_SQUAREBRACKET;
    return OPEN_SQUAREBRACKET;
}

token CloseSquareBracketsPatternAction() {
    LogDebug("[Flex] CloseSquareBracketsPatternAction: ']'.");
    yylval.token = CLOSE_SQUAREBRACKET;
    return CLOSE_SQUAREBRACKET;
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

token IntegerPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] IntegerPatternAction: '%s' (length = %d).", lexemeCopy,
             length);
    yylval.integer.i = atoi(lexemeCopy);
    free(lexemeCopy);
    return INTEGER;
}

token RealNumberPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] RealNumberPatternAction: '%s' (length = %d).", lexemeCopy,
             length);
    yylval.real.f = atof(lexemeCopy);
    free(lexemeCopy);
    return REAL;
}

token AssignOperatorPatternAction() {
    LogDebug("[Flex] AssignOperatorPatternAction: '='.");
    return ASSIGN;
}

token IdentifierPatterAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] IdentifierPatternAction: '%s' (length = %d).", lexemeCopy,
             length);
    yylval.identifier = calloc(1, sizeof(MeshItem));
    yylval.identifier->s = lexemeCopy;
    return IDENTIFIER;
}

token StartPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] StartPatternAction: 'start'.");
    yylval.initiate = INITIATE;
    return INITIATE;
}

token MeshPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] MeshPatternAction: 'mesh'.");
    return MESH;
}
token EndPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
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

token FunctionPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] FunctionPatternAction: '%s'.", lexemeCopy);
    return FUNCTION;
}

token StringPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] StringPatternAction: '%s'.", lexemeCopy);
    yylval.string.s = lexemeCopy;
    return STRING;
}

token ColorComponentOperatorPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] ColorComponentOperatorPatternAction: '%s'.", lexemeCopy);

    Color color;

    if (strcmp(lexemeCopy, "red") == 0) {
        color = RED;
    } else if (strcmp(lexemeCopy, "green") == 0) {
        color = GREEN;
    } else if (strcmp(lexemeCopy, "blue") == 0) {
        color = BLUE;
    } else {
        color = BLUE;
    }

    yylval.color = color;
    return COLOR;
}

token UnknownPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy,
             length);
    free(lexemeCopy);
    yylval.token = ERROR;
    // Al emitir este token, el compilador aborta la ejecución.
    return ERROR;
}

void IgnoredPatternAction(const char *lexeme, const int length) {
    char *lexemeCopy = copyLexeme(lexeme, length);
    LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
    LogText(lexemeCopy, length);
    LogRaw("' (length = %d).\n", length);
    free(lexemeCopy);
    // Como no debe hacer nada con el patrón, solo se loguea en consola.
    // No se emite ningún token.
}
