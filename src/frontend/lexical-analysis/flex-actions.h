#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Bloque de comentarios multilínea.
void BeginCommentPatternAction();
void EndCommentPatternAction();

// Patrones terminales del lenguaje diseñado.
token ComponentOperatorPatternAction(const char* lexeme, const int length);
token CommaPatternAction();
token OpenBracketsPatternAction();
token CloseBracketsPatternAction();
token OpenSquareBracketsPatternAction();
token CloseSquareBracketsPatternAction();
token OpenParenthesisPatternAction();
token CloseParenthesisPatternAction();
token IntegerPatternAction(const char * lexeme, const int length);
token RealNumberPatternAction(const char * lexeme, const int length);
token AssignOperatorPatternAction();
token IdentifierPatterAction(const char * lexeme, const int length);
token StartPatternAction(const char * lexeme, const int length);
token MeshPatternAction(const char * lexeme, const int length);
token EndPatternAction(const char * lexeme, const int length);
token FunctionPatternAction(const char * lexeme, const int length);
token EndLinePatternAction();
token StringPatternAction(const char * lexeme, const int length);
token ColorComponentOperatorPatternAction(const char* lexeme, const int length);

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char * lexeme, const int length);

// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char * lexeme, const int length);

#endif
