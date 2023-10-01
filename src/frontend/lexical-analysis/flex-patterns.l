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
digit			[0-9]
string          [A-Za-z]|[A-Za-z][A-Za-z0-9]
endline			[\n]
whitespace		[ \f\n\r\t\v]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"("                                         { return OpenParenthesisPatternAction(yytext); }
")"                                         { return CloseParenthesisPatternAction(yytext); }

"{"                                         { return OpenBracketsPatternAction(yytext); }
"}"                                         { return CloseBracketsPatternAction(yytext); }

"["                                         { return OpenSquareBracketsPatternAction(yytext); }
"]"                                         { return CloseSquareBracketsPatternAction(yytext); }

","                                         { return CommaPatternaction(yytext); }

"start"                                     { return StartPatternAction(yytext, yyleng); }
"mesh"                                      { return MeshPatternAction(yytext, yyleng); }
"end"                                       { return EndPatternAction(yytext, yyleng); }

"resistance"                                { return ComponentOperatorPatternAction(yytext, yyleng); }
"battery"                                   { return ComponentOperatorPatternAction(yytext, yyleng); }
"cable"                                     { return ComponentOperatorPatternAction(yytext, yyleng); }
"inductor"                                  { return ComponentOperatorPatternAction(yytext, yyleng); }
"ammeter"                                   { return ComponentOperatorPatternAction(yytext, yyleng); }
"voltmeter"                                 { return ComponentOperatorPatternAction(yytext, yyleng); }
"switch"                                    { return ComponentOperatorPatternAction(yytext, yyleng); }
"capacitor"                                 { return ComponentOperatorPatternAction(yytext, yyleng); }
"led"                                       { return ComponentOperatorPatternAction(yytext, yyleng); }
"trasformer"                                { return ComponentOperatorPatternAction(yytext, yyleng); }

"red"                                       { return ColorComponentOperatorPatternAction(yytext, yyleng); }
"green"                                     { return ColorComponentOperatorPatternAction(yytext, yyleng); }
"blue"                                      { return ColorComponentOperatorPatternAction(yytext, yyleng); }

"\""                                        { return StringPatternAction(yytext); }                                    

"="                                         { return AssignOperatorPatternAction(yytext); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

{string}+                                   { return IdentifierPatterAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
