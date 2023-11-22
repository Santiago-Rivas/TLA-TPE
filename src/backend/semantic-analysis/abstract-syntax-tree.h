#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/

/**
* En caso de que un no-terminal ("Factor" en este caso), posea más de una
* regla de producción asociada, se crea además de la estructura un enumerado
* que identifique cada reducción posible (cada regla de producción). Luego,
* se agrega una instancia de este enumerado dentro del nodo del no-terminal.
*
* De este modo, al recorrer el AST, es posible determinar qué nodos hijos
* posee según el valor de este enumerado.
*/
typedef enum {
    EXPRESSION
} FactorType;

typedef struct {
    FactorType type;
    Expression * expression;
} Factor;

typedef enum {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    FACTOR
} ExpressionType;

struct Expression {
    ExpressionType type;
    Expression * leftExpression;
    Expression * rightExpression;
};

typedef enum Color {
    RED,
    GREEN,
    BLUE,
    BLACK
} Color;

typedef enum {
    RESISTOR,
    BATTERY,
    CABLE,
    INDUCTOR,
    AMMETER,
    VOLTMETER,
    SWITCH,
    CAPACITOR,
    LED
} ComponentType;

typedef enum {
    VALUE_INTEGER,
    VALUE_FLOAT,
    VALUE_STRING
} ValueType;

typedef union {
    int i;
    char * s;
    double f;
} Value;

typedef struct {
    Value left;
    ValueType leftType;
    char * right;
} Pair;

typedef struct PairNode {
    Pair * pair;
    struct PairNode * next;
} PairNode;

typedef struct {
    Value value;
    ValueType type;
} Constant;

typedef union {
    Constant * c;
    PairNode * p;
} ComponentParams;

typedef enum {
    PARAM_CONSTANT,
    PARAM_PAIR_NODE 
} ParamType;

typedef struct {
    ComponentParams params;
    ParamType type;
} ComponentParamsList;


typedef struct {
    ComponentType type;
    ComponentParamsList * paramList;
    Color color;
} Component;

typedef struct Variable {
    char * identifier;
    Component * component;
} Variable;

typedef enum {
    MESH_COMPONENT,
    MESH_FUNCTION,
    MESH_IDENTIFIER
} MeshItemType;

typedef struct FunctionNode {
    int level;
    struct MeshItemNode * mesh;
    struct FunctionNode * next;
} FunctionNode;

typedef union {
    Component * c;
    FunctionNode * f;
    char * s;
} MeshItem;

typedef struct MeshItemNode{
    MeshItem item;
    MeshItemType itemType;
    Color color;
    struct MeshItemNode * next;
} MeshItemNode;

typedef struct {
    MeshItemNode * meshes;
} Program;

#endif
