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
                        \\usepackage[paperwidth=20in, paperheight=20in, margin=1in]{geometry}\
                        \\begin{document}\
                        \\begin{circuitikz}[line width=1.5pt]""";

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

