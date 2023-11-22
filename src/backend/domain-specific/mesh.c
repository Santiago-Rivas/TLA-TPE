#include "mesh.h"
#include "buffer.h"
#include "../support/logger.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * DrawComponentType(ComponentType componentType, char * message);
char * DrawComponent(char * componentName, char * message);
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
    pencil.buf = buffer;
    //LogDebug("Buffer Creation Success");

    Rectangle * rectangle = EvaluateMeshes(&pencil, program->meshes);
    if (rectangle == NULL) {
        return 1;
    }

    Point upperRight = CreatePoint(rectangle->p2.x, rectangle->p1.y);
    Point lowerRight = CreatePoint(upperRight.x, -4);
    PointToPointCable(pencil.buf, upperRight, lowerRight);
    PointToPointCable(pencil.buf, CreatePoint(0,-4), lowerRight);
    PointToPointCable(pencil.buf, CreatePoint(0,-4), CreatePoint(0, 0));

    *output = buffer->str;
    return 0;
}

Rectangle * EvaluateMeshes(Pencil * pencil, MeshItemNode * meshes) {
    Rectangle * rect;
    while (meshes != NULL) {
        switch (meshes->itemType) {
            case MESH_COMPONENT:
                rect = EvaluateComponent(pencil, meshes->item.c);
                if (rect == NULL) {
                    LogDebug("EvaluateComponent returned NULL");
                }
                break;
            case MESH_FUNCTION:
                pencil->level = meshes->item.f->level;
                rect = EvaluateFunction(pencil, meshes->item.f);
                if (rect == NULL) {
                    LogDebug("EvaluateFunction returned NULL");
                }
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
    return rect;
}

Rectangle * EvaluateComponent(Pencil * pencil, Component * component) {
    Rectangle * rect = malloc(sizeof(Rectangle));
    rect->p1 = pencil->currentPoint;
    Point point2 = (Point) {pencil->currentPoint.x + 4, pencil->currentPoint.y};
    PointToPointConnection(pencil->buf, pencil->currentPoint, point2, component);
    pencil->currentPoint = point2;
    rect->p2 = point2;
    return rect;
}

Rectangle * EvaluateFunction(Pencil * pencil, FunctionNode * functionNode){
    Point initial = pencil->currentPoint;
    Point point2 = (Point) {pencil->currentPoint.x, (pencil->currentPoint.y + 4) * pencil->level};
    PointToPointCable(pencil->buf, pencil->currentPoint, point2);
    pencil->currentPoint = point2;

    Point point3 = (Point) {point2.x + 4, point2.y};
    PointToPointCable(pencil->buf, pencil->currentPoint, point3);
    pencil->currentPoint = point3;


    PointList * pointList = NULL;
    Rectangle * totalRectangle = malloc(sizeof(Rectangle));

    totalRectangle->p1 = pencil->currentPoint;
    totalRectangle->p2 = pencil->currentPoint;
    while (functionNode != NULL) {
        Point aux = CreatePoint(pencil->currentPoint.x, (pencil->currentPoint.y + 4) * pencil->level);

        Rectangle * newRectangle = EvaluateMeshes(pencil, functionNode->mesh);
        pencil->currentPoint.x = initial.x;
        pencil->currentPoint.y = newRectangle->p2.y + 4;

        PointList * pointNode = malloc(sizeof(PointList));
        pointNode->nextPoint = pointList;
        pointNode->point = newRectangle->p2;
        pointList = pointNode;

        functionNode = functionNode->next;

        if (functionNode != NULL) {

            if (newRectangle->p1.y == newRectangle->p2.y) {
                newRectangle->p2.y += 4;
            }
            if (totalRectangle->p2.x < newRectangle->p2.x) {
                totalRectangle->p2.x = newRectangle->p2.x;
            }
            if (totalRectangle->p2.y < newRectangle->p2.y) {
                totalRectangle->p2.y = newRectangle->p2.y;
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

    Point newPoint = CreatePoint(totalRectangle->p2.x, totalRectangle->p1.y);
    PointToPointCable(pencil->buf, totalRectangle->p2, newPoint);
    pencil->currentPoint = newPoint;

    return totalRectangle;
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
            str = DrawComponent("short", message); // TODO: Remove
            break;
        default:
            str = DrawComponent("short", message);
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
    ConcatStringWithLength(buffer, "\n\\draw \n", 8);

    char * point1Str = PointToString(&p1);
    ConcatString(buffer, point1Str);

    char * message = GetComponentMessage(component);

    char * battery1 = DrawComponentType(component->type, message);
    ConcatString(buffer, battery1);

    char * point2Str = PointToString(&p2);
    ConcatString(buffer, point2Str);

    ConcatStringWithLength(buffer, ";\n", 2);

    free(battery1);
    free(point1Str);
    free(point2Str);
    // TODO: free(message) when implemented correctly

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
char * PointToString(Point * point) {
    char *str = malloc(20);
    if (str == NULL) {
        return NULL;
    }
    sprintf(str, " (%d,%d) ", point->x, point->y); 
    return str;
}

char * GetComponentMessage(Component * component){
    // TODO: Parse Param List
    return "";
}

Point CreatePoint(unsigned int x, unsigned int y) {
    return (Point) {x, y};
}
