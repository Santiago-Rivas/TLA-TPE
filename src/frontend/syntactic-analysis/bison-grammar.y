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
	int component;
	int components;
	int params;
	int color;
	int pair;
	int pairs;
	int beggin;
	int end;
	int mesh;
	int function;
	int functions;
	int function_params;
	char * string;
	char * identifier;
	int variable;
	int variables;


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
%token <token> FUNCTION

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
%type <components> components
%type <pair> pair
%type <pairs> pairs
%type <function> function
%type <functions> functions
%type <function_params> function_params
%type <variable> variable
%type <variables> variables
%type <mesh> mesh

// Reglas de asociatividad y precedencia (de menor a mayor).
// %left ADD SUB
// %left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%

program: BEGGIN MESH NEWLINE mesh END MESH								        			 	{ $$ = ProgramGrammarAction($1); }
	| variables BEGGIN MESH NEWLINE mesh END MESH										{ $$ = ProgramGrammarAction($1); }
	| program NEWLINE 													                { $$ = ProgramGrammarAction($1); }
	| NEWLINE BEGGIN MESH NEWLINE mesh END MESH												            { $$ = ProgramGrammarAction($1); }
	;

function: FUNCTION OPEN_PARENTHESIS NEWLINE  function_params  CLOSE_PARENTHESIS  { $$ = FunctionGrammarAction($1); }
	;

functions: function NEWLINE																{ $$ = FunctionsGrammarAction($1); }
	| function NEWLINE functions														{ $$ = FunctionsGrammarAction($1); }
	;


function_params:OPEN_SQUAREDBRACKET mesh  NEWLINE CLOSE_SQUAREDBRACKET 				   { $$ = FunctionParamsGrammarAction($1); }
	| mesh COMMA NEWLINE function_params											   { $$ = FunctionParamsGrammarAction($1); }
	;



mesh: mesh NEWLINE 																	{ $$ = MeshGrammarAction($1); }
	| components 														                    { $$ = MeshGrammarAction($1); }
	| functions 														                        { $$ = MeshGrammarAction($1); }
	| IDENTIFIER NEWLINE                                                                       { $$ = MeshGrammarAction($1); }
	;

component: COMPONENT params 																 { $$ = ComponentGrammarAction($1); }
	| COMPONENT  																	       { $$ = ComponentGrammarAction($1); }
	| COLOR COMPONENT 													                { $$ = ComponentGrammarAction($2); }
	| COLOR COMPONENT params   														    { $$ = ComponentGrammarAction($2); }
	;

components: component NEWLINE																{ $$ = ComponentsGrammarAction($1); }
	| component NEWLINE components															{ $$ = ComponentsGrammarAction($1); }
	;


params: OPEN_PARENTHESIS constant CLOSE_PARENTHESIS									{ $$ = ExpressionParamsGrammarAction($2); }
	| 	OPEN_PARENTHESIS pairs CLOSE_PARENTHESIS										{ $$ = PairParamsGrammarAction($2); }
	;

pairs: pair 																		{ $$ = PairsGrammarAction($1); }
	| pair COMMA pairs																{ $$ = PairsGrammarAction($1); }
	;

pair: OPEN_BRACKET INTEGER COMMA STRING CLOSE_BRACKET								{ $$ = PairGrammarAction($2, $4); }
	//| OPEN_BRACKET STRING COMMA STRING CLOSE_BRACKET								{ $$ = PairGrammarAction($2, $4); }
	;

constant: INTEGER 																	{ $$ = IntegerConstantGrammarAction($1); }
	| IDENTIFIER																	{ $$ = IdentifierConstantGrammarAction($1); }
	;

variables: variable NEWLINE																{ $$ = VariablesGrammarAction($1); }
	| variable NEWLINE variables															{ $$ = VariablesGrammarAction($1); }
	;

variable: IDENTIFIER ASSIGN component                    							{ $$ = IdentifierVariableGrammarAction($1); }
	;

%%
