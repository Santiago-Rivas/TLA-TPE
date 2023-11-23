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
	Program * program;
	MeshItemNode * mesh;
	MeshItemNode * meshes;
	int initiate;
	int end;
	MeshItem * function;
	FunctionNode * function_params;
	FunctionNode * function_param;
	ComponentParams * constant;
	MeshItem * component;
    ComponentType componentType;
	ComponentParamsList * params;
	Color color;
	Pair * pair;
	ComponentParams * pairs;
	Variable * variable;
	int variables;

	// Terminales.
	MeshItem * identifier;
	token token;
	Value integer;
	Value string;
	Value real;
	int newLine;

}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
%token <componentType> COMPONENT

%token <token> ASSIGN
%token <token> COMMA
%token <token> NEWLINE

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> FUNCTION

%token <token> OPEN_SQUAREBRACKET
%token <token> CLOSE_SQUAREBRACKET

%token <integer> INTEGER
%token <real> REAL 
%token <identifier> IDENTIFIER // token??? (identifier type char*)
%token <mesh> MESH
%token <initiate> INITIATE
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

program: INITIATE MESH NEWLINE meshes END MESH                                        { $$ = ProgramGrammarAction($4); }
    | variables INITIATE MESH NEWLINE meshes END MESH                                 { $$ = ProgramGrammarAction($5); }
    | program NEWLINE                                                                 { $$ = $1; }
    | NEWLINE INITIATE MESH NEWLINE meshes END MESH                                   { $$ = ProgramGrammarAction($5); }
    ;

meshes: mesh NEWLINE                                                                { $$ = MeshesGrammarAction($1, NULL); }
    | mesh NEWLINE meshes                                                           { $$ = MeshesGrammarAction($1, $3); }
    ;

mesh: component                                                                     { $$ = MeshGrammarAction($1, MESH_COMPONENT, BLACK); }
    | function                                                                      { $$ = MeshGrammarAction($1, MESH_FUNCTION, BLACK); }
    | IDENTIFIER                                                                    { $$ = MeshGrammarAction($1, MESH_IDENTIFIER , BLACK); }
    | COLOR IDENTIFIER                                                              { $$ = MeshGrammarAction($2, MESH_IDENTIFIER, $1); }
    ;

function: FUNCTION OPEN_PARENTHESIS NEWLINE  function_params  CLOSE_PARENTHESIS     { $$ = FunctionNodeToMeshItem($4); }
    ;

function_params: function_param COMMA NEWLINE function_param NEWLINE                { $$ = FunctionParamsGrammarAction($1, $4); }
    | function_param COMMA NEWLINE function_params                                  { $$ = FunctionParamsGrammarAction($1, $4); }
    ;

function_param: OPEN_SQUAREBRACKET NEWLINE meshes CLOSE_SQUAREBRACKET             { $$ = FunctionGrammarAction($3); }

component: COMPONENT params                                                         { $$ = ComponentGrammarAction($1, BLACK, $2); }
    | COMPONENT                                                                     { $$ = ComponentGrammarAction($1, BLACK, NULL); }
    | COLOR COMPONENT                                                               { $$ = ComponentGrammarAction($2, $1, NULL); }
    | COLOR COMPONENT params                                                        { $$ = ComponentGrammarAction($2, $1, $3); }
    ;

params: OPEN_PARENTHESIS constant CLOSE_PARENTHESIS                                 { $$ = ComponentParamsGrammarAction($2, PARAM_CONSTANT); }
    |     OPEN_PARENTHESIS pairs CLOSE_PARENTHESIS                                  { $$ = ComponentParamsGrammarAction($2, PARAM_PAIR_NODE); }
    ;

pairs: pair                                                                         { $$ = PairsGrammarAction($1, NULL); }
    | pair COMMA pairs                                                              { $$ = PairsGrammarAction($1, $3); }
    ;

pair: OPEN_BRACKET INTEGER COMMA STRING CLOSE_BRACKET                               { $$ = PairGrammarAction($2, VALUE_INTEGER, $4); }
    | OPEN_BRACKET STRING COMMA STRING CLOSE_BRACKET                                { $$ = PairGrammarAction($2, VALUE_STRING, $4); }
    | OPEN_BRACKET REAL COMMA STRING CLOSE_BRACKET                                  { $$ = PairGrammarAction($2, VALUE_FLOAT, $4); }
    ;

constant: INTEGER                                                                   { $$ = ConstantGrammarAction($1, VALUE_INTEGER); }
    | REAL                                                                          { $$ = ConstantGrammarAction($1, VALUE_FLOAT); }
    | STRING                                                                        { $$ = ConstantGrammarAction($1, VALUE_STRING); }
    ;

variables: variable NEWLINE                                                         { $$ = VariablesGrammarAction($1); }
    | variable NEWLINE variables                                                    { $$ = VariablesGrammarAction($1); }
    ;

variable: IDENTIFIER ASSIGN component                                               { $$ = IdentifierVariableGrammarAction($1, $3); }
    ;

%%
