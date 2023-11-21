#include "mesh.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * DrawComponentType(ComponentType componentType, char * message);

int EvaluateProgram(Program * program, char ** output){
    *output = malloc(1000);

    // Empty file
    char * str = ";";

    // TODO: Remove, this is and example of use
    char * battery1 = DrawComponentType(BATTERY, "hola");
    char * battery2 = DrawComponentType(BATTERY, "hola");
    sprintf(*output, "(0,0) %s (0, 1) %s (0,2)%s", battery1, battery2, str);
    free(battery1);
    free(battery2);
    return 0;
}

int EvaluateMeshes(MeshItemNode * meshes, char ** output) {
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
