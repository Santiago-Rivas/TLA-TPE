#include "mesh.h"
#include "buffer.h"
#include "../support/logger.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * DrawComponentType(ComponentType componentType, char * message);
char * DrawComponent(char * componenetName, char * message);
Point CreatePoint(unsigned int x, unsigned int y);

int EvaluateMeshes(Pencil * pencil, MeshItemNode * meshes);
int EvaluateComponent(Pencil * pencil, Component * component);
int EvaluateFunction(Pencil * pencil, FunctionNode * functionNode);
int PointToPointCable(Buffer * buffer, Point p1, Point p2);
int PointToPointConnection(Buffer * buffer, Point p1, Point p2, Component * component);
int PointToString(Point * point, char ** str);
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
    pencil.buf = buffer;
    //LogDebug("Buffer Creation Success");

    *output = buffer->str;
    return 0;
}

int EvaluateMeshes(Pencil * pencil, MeshItemNode * meshes) {
    while (meshes != NULL) {
        switch (meshes->itemType) {
            case MESH_COMPONENT:
                EvaluateComponent(pencil, meshes->item.c);
                break;
            case MESH_FUNCTION:
                pencil->level = meshes->item.f->level;
                EvaluateFunction(pencil, meshes->item.f);
                if (!(meshes->item.f->level == 0)) {
                    meshes->item.f->level--;
                }
                break;
            case MESH_IDENTIFIER:
                break;
            default:
                break;
        }
        meshes = meshes->next;
    }
    return 0;
}

int EvaluateComponent(Pencil * pencil, Component * component) {
    Point point2 = (Point) {pencil->currentPoint.x + 4, pencil->currentPoint.y};
    PointToPointConnection(pencil->buf, pencil->currentPoint, point2, component);
    pencil->currentPoint = point2;
    return point2.x;
}

int EvaluateFunction(Pencil * pencil, FunctionNode * functionNode){
    Point point2 = (Point) {pencil->currentPoint.x, (pencil->currentPoint.y + 4) * pencil->level};
    PointToPointCable(pencil->buf, pencil->currentPoint, point2);
    pencil->currentPoint = point2;

    Point point3 = (Point) {point2.x + 4, point2.y};
    PointToPointCable(pencil->buf, pencil->currentPoint, point3);
    pencil->currentPoint = point3;

    while (functionNode != NULL) {
        Point aux = CreatePoint(pencil->currentPoint.x, (pencil->currentPoint.y + 4) * pencil->level);

        EvaluateMeshes(pencil, functionNode->mesh);
        functionNode = functionNode->next;
    }

    Point point4 = (Point) {pencil->currentPoint.x + 4, pencil->currentPoint.y};
    PointToPointCable(pencil->buf, pencil->currentPoint, point4);
    pencil->currentPoint = point4;


    Point point5 = (Point) {point4.x, point2.y};
    PointToPointCable(pencil->buf, pencil->currentPoint, point5);
    pencil->currentPoint = point5;

    return 0;
}

int EvaluateComponents() {
    return 0;
}

char * DrawComponent(char * componentName, char * message) {
    char * str = malloc(strlen(componentName) + strlen(message) + 15);
    sprintf(str, " to [%s, l_=$%s$] ", componentName, message);
    return str;
}

char * DrawComponentType(ComponentType componentType, char * message){
    char * str;
    switch(componentType) {
        case RESISTOR:
            str = DrawComponent("R", message);
            break;
        case BATTERY:
            str = DrawComponent("battery1", message);
            break;
        case CABLE:
            str = DrawComponent("short", message);
            break;
        case INDUCTOR:
            str = DrawComponent("L", message);
            break;
        case AMMETER:
            str = DrawComponent("ammeter", message);
            break;
        case VOLTMETER:
            str = DrawComponent("voltmeter", message);
            break;
        case SWITCH:
            str = DrawComponent("switch", message);
            break;
        case CAPACITOR:
            str = DrawComponent("C", message);
            break;
        case LED:
            str = DrawComponent("led", message);
            break;
        case TRANSISTOR:
            str = DrawComponent("short", message); // TODO: Remov
            break;
        default:
            str = DrawComponent("short", message);
            break;
    }
    return str;
}

int PointToPointCable(Buffer * buffer, Point p1, Point p2) {
    Component comp;
    comp.type = CABLE;
    comp.color = BLACK;
    comp.paramList = NULL;
    return PointToPointConnection(buffer, p1, p2, &comp);
}

int PointToPointConnection(Buffer * buffer, Point p1, Point p2, Component * component) {
    ConcatStringWithLength(buffer, "\n\\draw \n", 8);

    char * point1Str;
    int len1 = PointToString(&p1, &point1Str);
    ConcatStringWithLength(buffer, point1Str, len1);

    char * message = GetComponentMessage(component);

    char * battery1 = DrawComponentType(component->type, message);
    ConcatString(buffer, battery1);

    char * point2Str;
    int len2 = PointToString(&p2, &point2Str);
    ConcatStringWithLength(buffer, point2Str, len2);

    ConcatStringWithLength(buffer, ";\n", 2);

    free(battery1);
    free(point1Str);
    free(point2Str);
    // TODO: free(message) when implmemented correctly

    return 1;
}

int CheckPointRealloc(char ** str, int i) {
    //LogDebug("Check Point Realloc");
    if ((i % 10) == 0) {
        //LogDebug("Entered Point To String Realloc");
        *str = realloc(*str, i + (sizeof(char) * 10));
        if (*str == NULL) {
            return 0;
        }
    }
    return 1;
}

// Fix implementation
int PointToString(Point * point, char ** str) {
    //LogDebug("Start of PointToString");
    *str = malloc(sizeof(char) * 10);
    if (*str == NULL) {
        return 0;
    }
    int i = 0;
    unsigned int x = point->x;
    unsigned int y = point->y;

    (*str)[i++] = '(';
    do {
        //LogDebug("x value : %d", x);
        if (!CheckPointRealloc(str, i)){
            return 0;
        }
        (*str)[i++] = '0' + (x % 10);
        x /= 10;
    } while (x > 0);
    if (!CheckPointRealloc(str, i)){
        return 0;
    }
    //LogDebug("PointToString coma");
    (*str)[i++] = ',';
    do {
        //LogDebug("y value : %d", y);
        if (!CheckPointRealloc(str, i)){
            return 0;
        }
        (*str)[i++] = '0' + (y % 10);
        y /= 10;
    } while (y > 0);

    if (!CheckPointRealloc(str, i)){
        return 0;
    }

    (*str)[i++] = ')';
    (*str)[i] = '\0';
    return i;
}

char * GetComponentMessage(Component * component){
    // TODO: Parse Param List
    return "Message";
}

Point CreatePoint(unsigned int x, unsigned int y) {
    return (Point) {x, y};
}
