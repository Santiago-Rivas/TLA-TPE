#include "generator.h"
#include "../support/logger.h"

#include <stdio.h>

/**
 * Implementación de "generator.h".
 */

int Generator(char * fileName, char * output, int width, int height) {
    FILE *file = fopen(fileName, "w");
    char * docSetup = """\\documentclass{article}\n\
\\usepackage[utf8]{inputenc}\n\
\\usepackage{circuitikz}\n""";

    char * docOpen = """\\begin{document}\n\
\\begin{circuitikz}\n""";

    char * docClose = """\\end{circuitikz}\n\
\\end{document}\n""";

    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }
    LogDebug("width = %d, height = %d", width, height);

    fprintf(file, "%s\n\\usepackage[paperwidth=%dcm, paperheight=%dcm, margin=1in]{geometry}\n%s\n%s\n%s", docSetup, width, height, docOpen, output, docClose);

    fclose(file);

    return 0;
}

