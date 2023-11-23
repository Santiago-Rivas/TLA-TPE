#include "mesh.h"
#include "buffer.h"
#include "../support/logger.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * DrawComponentType(Component * component);
char * DrawComponent(char * componentName, char * message, Color color);
Point CreatePoint(unsigned int x, unsigned int y);

Rectangle * EvaluateMeshes(Pencil * pencil, MeshItemNode * meshes);
Rectangle * EvaluateComponent(Pencil * pencil, Component * component);
Rectangle * EvaluateFunction(Pencil * pencil, FunctionNode * functionNode);
int PointToPointCable(Buffer * buffer, Point p1, Point p2);
int PointToPointConnection(Buffer * buffer, Point p1, Point p2, Component * component);
char * PointToString(Point * point);
char * GetComponentMessage(Component * component);

int EvaluateProgram(Program * program, char ** output){
    //LogDebug("Entered EvaluateProgram");
    Buffer * buffer = BufferInit();
    if (buffer == NULL) {
        LogError("Evaluate Program Memory Allocation Failed");
        return 0;
    }

    Pencil pencil;
    pencil.currentPoint = (Point) {0,0};
    pencil.level = 0;
    pencil.outerSeries = 0;
    pencil.buf = buffer;
    //LogDebug("Buffer Creation Success");

    Rectangle * rectangle = EvaluateMeshes(&pencil, program->meshes);
    if (rectangle == NULL) {
        return 1;
    }

    if (pencil.outerSeries != 0) {
        Point upperRight = CreatePoint(rectangle->p2.x, rectangle->p1.y);
        Point lowerRight = CreatePoint(upperRight.x, -4);
        PointToPointCable(pencil.buf, upperRight, lowerRight);
        PointToPointCable(pencil.buf, CreatePoint(0,-4), lowerRight);
        PointToPointCable(pencil.buf, CreatePoint(0,-4), CreatePoint(0, 0));
    }

    *output = buffer->str;
    return 0;
}

Rectangle * EvaluateMeshes(Pencil * pencil, MeshItemNode * meshes) {
    Rectangle * rect = NULL;
    int lastWasParallel = 0;
    while (meshes != NULL) {
        if (meshes->itemType == MESH_COMPONENT) {
            if (pencil->level == 0) {
                LogDebug("Pencil Leve = %d", pencil->level);
                pencil->outerSeries++;
            }
            rect = EvaluateComponent(pencil, meshes->item.c);
            if (rect == NULL) {
                LogDebug("EvaluateComponent returned NULL");
            }
            lastWasParallel = 0;
        } else if (meshes->itemType == MESH_FUNCTION) {
            if (lastWasParallel == 1) {
                Point point = CreatePoint(pencil->currentPoint.x + 4, pencil->currentPoint.y);
                PointToPointCable(pencil->buf, pencil->currentPoint, point);
                pencil->currentPoint = point;
                if (pencil->level == 0) {
                    pencil->outerSeries++;
                }
            }
            rect = EvaluateFunction(pencil, meshes->item.f);
            if (rect == NULL) {
                LogDebug("EvaluateFunction returned NULL");
            }
            lastWasParallel = 1;
        }
        meshes = meshes->next;
    }
    return rect;
}

Rectangle * EvaluateComponent(Pencil * pencil, Component * component) {
    Rectangle * rect = malloc(sizeof(Rectangle));
    if (rect == NULL) {
        return NULL;
    }
    rect->p1 = pencil->currentPoint;
    Point point2 = (Point) {pencil->currentPoint.x + 4, pencil->currentPoint.y};
    PointToPointConnection(pencil->buf, pencil->currentPoint, point2, component);
    pencil->currentPoint = point2;
    rect->p2 = point2;
    return rect;
}

Rectangle * EvaluateFunction(Pencil * pencil, FunctionNode * functionNode){
    Point point2;
    Point point3;

    point2 = (Point) {pencil->currentPoint.x, pencil->currentPoint.y};
    pencil->currentPoint = point2;

    point3 = (Point) {point2.x, point2.y};
    pencil->currentPoint = point3;

    int initialLevel = pencil->level;
    // pencil->level = functionNode->level;
    LogDebug("Pencil Level Eval = %d", pencil->level);

    Point initial = pencil->currentPoint;


    PointList * pointList = NULL;
    Rectangle * totalRectangle = malloc(sizeof(Rectangle));

    if (totalRectangle == NULL) {
        return NULL;
    }
    totalRectangle->p1 = pencil->currentPoint;
    totalRectangle->p2 = pencil->currentPoint;
    while (functionNode != NULL) {
        Point aux = CreatePoint(pencil->currentPoint.x, pencil->currentPoint.y + 4);

        pencil->level += 1;
        LogDebug("EvaluateFunction level: %d", pencil->level);
        Rectangle * newRectangle = EvaluateMeshes(pencil, functionNode->mesh);
        pencil->level -= 1;
        pencil->currentPoint.x = initial.x;

        PointList * pointNode = malloc(sizeof(PointList));
        if (pointNode == NULL) {
            return NULL;
        }
        pointNode->nextPoint = pointList;
        pointNode->point = newRectangle->p2;
        pointList = pointNode;

        functionNode = functionNode->next;


        if (totalRectangle->p2.x < newRectangle->p2.x) {
            LogDebug("increased totalRectangle");
            totalRectangle->p2.x = newRectangle->p2.x;
        }
        if (totalRectangle->p2.y < newRectangle->p2.y) {
            LogDebug("increased totalRectangle");
            totalRectangle->p2.y = newRectangle->p2.y;
        }

        if (functionNode != NULL) {
            if (newRectangle->p1.y == newRectangle->p2.y) {
                newRectangle->p2.y += 4;
            }
            LogDebug("destination");
            Point destination = CreatePoint(pencil->currentPoint.x, newRectangle->p2.y);
            PointToPointCable(pencil->buf, pencil->currentPoint, destination);
            pencil->currentPoint = destination;
        }
    }

    PointList * pointNode = pointList;
    while (pointNode != NULL) {
        if (pointNode->point.x < totalRectangle->p2.x) {
            Point newPoint = CreatePoint(totalRectangle->p2.x, pointNode->point.y);
            PointToPointCable(pencil->buf, pointNode->point, newPoint);
        }
        pointNode = pointNode->nextPoint;
    }

    Point point4 = CreatePoint(totalRectangle->p2.x, totalRectangle->p1.y);
    Point point5 = CreatePoint(point4.x, pencil->currentPoint.y);
    PointToPointCable(pencil->buf, point5, point4);
    pencil->currentPoint = point4;


    Point point6;
    Point point7;

    point6 = (Point) {point4.x, pencil->currentPoint.y};
    pencil->currentPoint = point6;


    point7 = (Point) {point6.x, point2.y};
    pencil->currentPoint = point7;

    totalRectangle->p2.x = point7.x;

    return totalRectangle;
}

int EvaluateComponents() {
    return 0;
}

char * DrawComponent(char * componentName, char * message, Color color) {
    LogDebug("DrawComponent %s", componentName);
    char * str = malloc(strlen(componentName) + strlen(message) + 30);
    if (str == NULL) {
        LogDebug("Memory allocation failed in DrawComponent");
        return NULL; // or handle the error in an appropriate way
    }

    char * colorString;
    if (color == RED) {
        colorString = "red";
    } else if (color == BLUE) {
        colorString = "blue";
    } else if (color == GREEN) {
        colorString = "green";
    } else {
        colorString = "black";
    }

    sprintf(str, " to [%s, l_=$%s$, color=%s] ", componentName, message, colorString);
    return str;
}

char * DrawComponentType(Component * component){
    char * message = GetComponentMessage(component);
    char * str;
    switch(component->type) {
        case RESISTOR:
            str = DrawComponent("R", message, component->color);
            break;
        case BATTERY:
            str = DrawComponent("battery1", message, component->color);
            break;
        case CABLE:
            str = DrawComponent("short", message, component->color);
            break;
        case INDUCTOR:
            str = DrawComponent("L", message, component->color);
            break;
        case AMMETER:
            str = DrawComponent("ammeter", message, component->color);
            break;
        case VOLTMETER:
            str = DrawComponent("voltmeter", message, component->color);
            break;
        case SWITCH:
            str = DrawComponent("switch", message, component->color);
            break;
        case CAPACITOR:
            str = DrawComponent("C", message, component->color);
            break;
        case LED:
            str = DrawComponent("led", message, component->color);
            break;
        default:
            str = DrawComponent("short", message, component->color);
            break;
    }
    return str;
}

int PointToPointCable(Buffer * buffer, Point p1, Point p2) {
    LogDebug("Creating cable from (%d,%d) to (%d,%d)", p1.x, p1.y, p2.x, p2.y);
    Component comp;
    comp.type = CABLE;
    comp.color = BLACK;
    comp.paramList = NULL;
    return PointToPointConnection(buffer, p1, p2, &comp);
}

int PointToPointConnection(Buffer * buffer, Point p1, Point p2, Component * component) {
    ConcatStringWithLength(buffer, "\n\\draw ", 7);

    char * point1Str = PointToString(&p1);
    ConcatString(buffer, point1Str);


    char * componentString = DrawComponentType(component);
    ConcatString(buffer, componentString);

    char * point2Str = PointToString(&p2);
    ConcatString(buffer, point2Str);

    ConcatStringWithLength(buffer, ";\n", 2);

    free(componentString);
    free(point1Str);
    free(point2Str);
    // TODO: free(message) when implemented correctly

    return 1;
}

int CheckPointRealloc(char ** str, int i) {
    //LogDebug("Check Point Realloc");
    if ((i % 10) == 0) {
        //LogDebug("Entered Point To String Realloc");
        char *temp = realloc(*str, (i + 10) * sizeof(char));
        if (temp == NULL) {
            // Handle realloc failure, e.g., log an error message or return an error code
            return 0;
        }
        *str = temp;
    }
    return 1;
}

// Fix implementation
char * PointToString(Point * point) {
    char *str = malloc(20);
    if (str == NULL) {
        return NULL;
    }
    sprintf(str, " (%d,%d) ", point->x, point->y); 
    return str;
}

void PrintValue(Buffer * buffer, Value * value) {
}

char * GetComponentMessage(Component * component){
    char str[MAX_STR];
    Buffer * buffer = BufferInit();

    if (component->paramList == NULL) {
        return "";
    }
    if (component->paramList->type == PARAM_PAIR_NODE) {
        PairNode * pairNode = component->paramList->params.p;

        while (pairNode != NULL) {
            if (pairNode->pair->leftType == VALUE_INTEGER){
                sprintf(str, "%d", pairNode->pair->left.i);
                ConcatString(buffer, str);
            } else if (pairNode->pair->leftType == VALUE_FLOAT) {
                sprintf(str, "%f", pairNode->pair->left.f);
                ConcatString(buffer, str);
            } else {
                if (pairNode->pair->left.s != NULL) {
                    ConcatString(buffer, pairNode->pair->left.s);
                }
            }
            if (pairNode->pair->right != NULL) {
                ConcatStringWithLength(buffer, pairNode->pair->right + 1, strlen(pairNode->pair->right + 1) - 1);
                ConcatStringWithLength(buffer, "\\ ", 2);
            }
            pairNode = pairNode->next;
        }
    } else {
        Constant * constant = component->paramList->params.c;
        if (constant->type == VALUE_INTEGER){
            sprintf(str, "%d", constant->value.i);
            ConcatString(buffer, str);
        } else if (constant->type == VALUE_FLOAT) {
            LogDebug("FLOAT: %f", constant->value.f);
            sprintf(str, "%.*g",10 ,constant->value.f);
            ConcatString(buffer, str);
        } else {
            if (constant->value.s != NULL) {
                ConcatStringWithLength(buffer, constant->value.s + 1, strlen(constant->value.s + 1) - 1);
            }
        }
    }
    return buffer->str;
}

Point CreatePoint(unsigned int x, unsigned int y) {
    return (Point) {x, y};
}
