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
	int params;
	int color;
	int pair;
	int pairs;
	int beggin;
	int end;
	int mesh;
	int meshes;
	int function;
	int function_params;
	int function_param;
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
%type <pair> pair
%type <pairs> pairs
%type <function> function
%type <function_params> function_params
%type <function_param> function_param
%type <variable> variable
%type <variables> variables
%type <mesh> mesh
%type <meshes> meshes

// Reglas de asociatividad y precedencia (de menor a mayor).
// %left ADD SUB
// %left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%

program: BEGGIN MESH NEWLINE meshes END MESH                                                    { $$ = ProgramGrammarAction($1); }
    | variables BEGGIN MESH NEWLINE meshes END MESH                                             { $$ = ProgramGrammarAction($1); }
    | program NEWLINE                                                                           { $$ = ProgramGrammarAction($1); }
    | NEWLINE BEGGIN MESH NEWLINE meshes END MESH                                               { $$ = ProgramGrammarAction($1); }
    ;

function: FUNCTION OPEN_PARENTHESIS NEWLINE  function_params  CLOSE_PARENTHESIS                 { $$ = FunctionGrammarAction($1); }
    ;

function_param: OPEN_SQUAREDBRACKET NEWLINE meshes CLOSE_SQUAREDBRACKET                         { $$ = FunctionParamGrammarAction($1); }

function_params: function_param COMMA NEWLINE function_param NEWLINE                            { $$ = FunctionParamsGrammarAction($1); }
    | function_param COMMA NEWLINE function_params                                              { $$ = FunctionParamsGrammarAction($1); }
    ;

mesh: component                                                                                 { $$ = MeshGrammarAction($1); }
    | function                                                                                  { $$ = MeshGrammarAction($1); }
    | IDENTIFIER                                                                                { $$ = MeshGrammarAction($1); }
    | COLOR IDENTIFIER                                                                          { $$ = MeshGrammarAction($1); }
    ;

meshes: mesh NEWLINE                                                                            { $$ = MeshesGrammarAction($1); }
    | mesh NEWLINE meshes                                                                       { $$ = MeshesGrammarAction($1); }
    ;

component: COMPONENT params                                                                     { $$ = ComponentGrammarAction($1); }
    | COMPONENT                                                                                 { $$ = ComponentGrammarAction($1); }
    | COLOR COMPONENT                                                                           { $$ = ComponentGrammarAction($2); }
    | COLOR COMPONENT params                                                                    { $$ = ComponentGrammarAction($2); }
    ;

params: OPEN_PARENTHESIS constant CLOSE_PARENTHESIS                                             { $$ = ExpressionParamsGrammarAction($2); }
    |     OPEN_PARENTHESIS pairs CLOSE_PARENTHESIS                                              { $$ = PairParamsGrammarAction($2); }
    ;

pairs: pair                                                                                     { $$ = PairsGrammarAction($1); }
    | pair COMMA pairs                                                                          { $$ = PairsGrammarAction($1); }
    ;

pair: OPEN_BRACKET INTEGER COMMA STRING CLOSE_BRACKET                                           { $$ = PairIntegerGrammarAction($2, $4); }
    | OPEN_BRACKET STRING COMMA STRING CLOSE_BRACKET                                            { $$ = PairStringGrammarAction($2, $4); }
    ;

constant: INTEGER                                                                               { $$ = IntegerConstantGrammarAction($1); }
    ;

variables: variable NEWLINE                                                                     { $$ = VariablesGrammarAction($1); }
    | variable NEWLINE variables                                                                { $$ = VariablesGrammarAction($1); }
    ;

variable: IDENTIFIER ASSIGN component                                                           { $$ = IdentifierVariableGrammarAction($1); }
    ;

%%
