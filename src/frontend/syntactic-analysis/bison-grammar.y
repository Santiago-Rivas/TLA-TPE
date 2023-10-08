%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
	int constant;
	char * identifier;
	int params;
	int component;
	int pair;
	int beggin;
	int mesh;
	int end;
	char * string;
	int color;

	// Terminales.
	token token;
	int integer;
	int newLine;

}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> COMPONENT

%token <token> ASSIGN
%token <token> COMMA
%token <token> NEWLINE

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET

%token <token> OPEN_SQUAREDBRACKET
%token <token> CLOSE_SQUAREDBRACKET

%token <integer> INTEGER
%token <token> IDENTIFIER // token??? (identifier type char*)
%token <mesh> MESH
%token <beggin> BEGGIN
%token <end> END
%token <color> COLOR
%token <string> STRING

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <constant> constant
%type <params> params
%type <component> component
%type <pair> pair

// Reglas de asociatividad y precedencia (de menor a mayor).
// %left ADD SUB
// %left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%

program: program BEGGIN MESH NEWLINE END MESH											 				{ $$ = ProgramGrammarAction($1); }
	| program BEGGIN MESH NEWLINE component END MESH												{ $$ = ProgramGrammarAction($3); }
	| component BEGGIN MESH NEWLINE component END MESH										{ $$ = ProgramGrammarAction($4); }
	| component BEGGIN MESH NEWLINE END MESH												{ $$ = ProgramGrammarAction($1); }
	| program NEWLINE
	;

component: COMPONENT params NEWLINE component  												{ $$ = ComponentGrammarAction($1); } 
	| COMPONENT NEWLINE component  															{ $$ = ComponentGrammarAction($1); }
	| COMPONENT params NEWLINE																{ $$ = ComponentGrammarAction($1); }
	| COMPONENT NEWLINE 																	{ $$ = ComponentGrammarAction($1); }
	| COLOR COMPONENT params NEWLINE component 												{ $$ = ComponentGrammarAction($2); } 
	| COLOR COMPONENT NEWLINE component 													{ $$ = ComponentGrammarAction($2); }
	| COLOR COMPONENT params NEWLINE  														{ $$ = ComponentGrammarAction($2); }
	| COLOR COMPONENT NEWLINE   															{ $$ = ComponentGrammarAction($2); }
	;

params: OPEN_PARENTHESIS constant CLOSE_PARENTHESIS									{ $$ = ExpressionParamsGrammarAction($2); }
	| 	OPEN_PARENTHESIS pair CLOSE_PARENTHESIS										{ $$ = PairParamsGrammarAction($2); }
	| 	OPEN_PARENTHESIS pair COMMA STRING CLOSE_PARENTHESIS						{ $$ = FullSizeParamsGrammarAction($2, $4); }
	| 	constant																	{ $$ = ConstantParamsGrammarAction($1); }
	;

pair: OPEN_BRACKET INTEGER COMMA STRING CLOSE_BRACKET								{ $$ = PairGrammarAction($2, $4); }
	;

constant: INTEGER 																	{ $$ = IntegerConstantGrammarAction($1); }
	| IDENTIFIER																	{ $$ = IdentifierConstantGrammarAction($1); }
	;

%%
