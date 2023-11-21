#include "generator.h"
#include "../support/logger.h"

#include <stdio.h>

/**
 * Implementación de "generator.h".
 */

int Generator(char * fileName, char * output) {
    FILE *file = fopen(fileName, "w");
    char * docSetup = """\
                        \\documentclass{article}\
                        \\usepackage[utf8]{inputenc}\
                        \\usepackage{circuitikz}\
                        \\begin{document}\
                        \\begin{circuitikz} \\draw""";

    char * docClose = """\
                        \\end{circuitikz}\
                        \\end{document}""";

    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    fprintf(file, "%s\n%s\n%s", docSetup, output, docClose);

    fclose(file);

    return 0;
}

