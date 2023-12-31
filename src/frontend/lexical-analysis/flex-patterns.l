%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

/* Patrones reutilizables. */
digits          (-)?[0-9]+
number          (-)?[0-9]+(\.[0-9]+)?
identifier      [A-Za-z][A-Za-z0-9_-]*[A-Za-z0-9]*
string          \"[^\"]*\"
endline         [ \f\r\t\v]*[\n]+
whitespace      [ \f\r\t\v]+
component       Resistor|Battery|Cable|Inductor|Ammeter|Voltmeter|Switch|Capacitor|Led
color           red|green|blue

%%

"/*"                                        { BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.                                  { IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+                         { IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"                               { EndCommentPatternAction(); BEGIN(INITIAL); }

"("                                         { return OpenParenthesisPatternAction(yytext); }
")"                                         { return CloseParenthesisPatternAction(yytext); }

"{"                                         { return OpenBracketsPatternAction(yytext); }
"}"                                         { return CloseBracketsPatternAction(yytext); }

"["                                         { return OpenSquareBracketsPatternAction(yytext); }
"]"                                         { return CloseSquareBracketsPatternAction(yytext); }

","                                         { return CommaPatternAction(yytext); }

start                                       { return StartPatternAction(yytext, yyleng); }
mesh                                        { return MeshPatternAction(yytext, yyleng); }
end                                         { return EndPatternAction(yytext, yyleng); }

parallel                                    { return FunctionPatternAction(yytext, yyleng); }

"="                                         { return AssignOperatorPatternAction(yytext); }

{color}                                    { return ColorComponentOperatorPatternAction(yytext, yyleng); }

{component}                                 { return ComponentOperatorPatternAction(yytext, yyleng); }

{endline}+                                  { return EndLinePatternAction(); }

{digits}                                    { return IntegerPatternAction(yytext, yyleng); }

{number}                                    { return RealNumberPatternAction(yytext, yyleng); }

{string}                                    { return StringPatternAction(yytext, yyleng); }

{whitespace}*                               { IgnoredPatternAction(yytext, yyleng); }

{identifier}+                               { return IdentifierPatterAction(yytext, yyleng); }

.                                           { return UnknownPatternAction(yytext, yyleng); }

%%
