#include "mesh.h"
#include "buffer.h"
#include "../support/logger.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * DrawComponentType(ComponentType componentType, char * message);
char * DrawComponent(char * componenetName, char * message);

int EvaluateMeshes(MeshItemNode * meshes, char ** output);
int EvaluateComponent(Component * component, char ** output);
int EvaluateFunction(FunctionNode * functionNode, char ** output);
int PointToPointConnection(Buffer * buffer, Point p1, Point p2, ComponentType component, char * message);
int PointToString(Point * point, char ** str);

int EvaluateProgram(Program * program, char ** output){
    //LogDebug("Entered EvaluateProgram");
    Buffer * buffer = BufferInit();
    if (buffer == NULL) {
        LogError("Evaluate Program Memory Allocation Failed");
        return 0;
    }
    //LogDebug("Buffer Creation Success");

//    PointToPointConnection(buffer, (Point) {0,0}, (Point) {0,1}, BATTERY, "Hola");
//    PointToPointConnection(buffer, (Point) {0,1}, (Point) {0,2}, CABLE, "Hola");
//    PointToPointConnection(buffer, (Point) {0,2}, (Point) {0,4}, RESISTOR, "Hola");
//
//    PointToPointConnection(buffer, (Point) {0,4}, (Point) {2,4}, LED, "Hola");
//    PointToPointConnection(buffer, (Point) {2,4}, (Point) {2,0}, AMMETER, "Hola");
//    PointToPointConnection(buffer, (Point) {2,0}, (Point) {0,0}, CABLE, "Hola");

    // PointToPointConnection(buffer, (Point) {2,4}, (Point) {6,4}, LED, "Hola");
    // PointToPointConnection(buffer, (Point) {6,4}, (Point) {6,0}, AMMETER, "Hola");
    // PointToPointConnection(buffer, (Point) {6,0}, (Point) {0,0}, CABLE, "Hola");

    *output = buffer->str;
    return 0;
}

// int EvaluateMeshes(MeshItemNode * meshes, char ** output) {
//     while (meshes != NULL) {
//         switch (meshes->itemType) {
//             case MESH_COMPONENT:
//                 EvaluateComponent(meshes->item.c, output);
//                 break;
//             case MESH_FUNCTION:
//                 EvaluateFunction(meshes->item.f, output);
//                 break;
//             case MESH_IDENTIFIER:
//                 break;
//             default:
//                 break;
//         }
//         meshes = meshes->next;
//     }
//     return 0;
// }


// int EvaluateComponent(Point * point, Component * component, char ** output) {
//     Point point2 = {point->x + 4, point->y};
//     int PointToPointConnection(buffer, *point, point2, component, message);
//     return point2.x;
// }
// 
// int EvaluateFunction(FunctionNode * functionNode, char ** output){
//     EvaluateComponents(point, functionNode->mesh);
//     return 0;
// }
// 
// int EvaluateComponents() {
//     
// }

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


int PointToPointConnection(Buffer * buffer, Point p1, Point p2, ComponentType component, char * message) {
    ConcatStringWithLength(buffer, "\n\\draw \n", 8);

    char * point1Str;
    int len1 = PointToString(&p1, &point1Str);
    ConcatStringWithLength(buffer, point1Str, len1);

    char * battery1 = DrawComponentType(component, message);
    ConcatString(buffer, battery1);

    char * point2Str;
    int len2 = PointToString(&p2, &point2Str);
    ConcatStringWithLength(buffer, point2Str, len2);

    ConcatStringWithLength(buffer, ";\n", 2);

    free(battery1);
    free(point1Str);
    free(point2Str);

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
