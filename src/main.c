#include "backend/code-generation/generator.h"
#include "backend/domain-specific/mesh.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "backend/semantic-analysis/symbol-table.h"
#include "frontend/syntactic-analysis/bison-parser.h"
#include <stdio.h>

// Estado de la aplicación.
CompilerState state;

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char ** arguments) {
	// Inicializar estado de la aplicación.
	state.program = NULL;
	state.result = 0;
	state.succeed = false;
	state.map = kh_init(comp);

	// Mostrar parámetros recibidos por consola.
	for (int i = 0; i < argumentCount; ++i) {
		LogInfo("Argumento %d: '%s'", i, arguments[i]);
	}
	// Compilar el programa de entrada.
	LogInfo("Compilando...\n");
	const int result = yyparse();
	switch (result) {
		case 0:
			// La variable "succeed" es la que setea Bison al identificar el símbolo
			// inicial de la gramática satisfactoriamente.
			if (state.succeed) {
				LogInfo("La compilación fue exitosa.");
                Rectangle * rectangle = EvaluateProgram(state.program, &state.output);
                if (rectangle == NULL) {
                    state.succeed = 0;
				    LogError("Se produjo un error en la aplicación.");
                    return 1;
                }
                LogDebug("p1x = %d, p1y = %d, p2x = %d, p2y = %d", rectangle->p1.x, rectangle->p1.y, rectangle->p2.x, rectangle->p2.y);
				Generator("output.tex", state.output, rectangle->p2.x - rectangle->p1.x + 12, rectangle->p2.y - rectangle->p1.y + 12);
			}
			else {
				LogError("Se produjo un error en la aplicación.");
				return 1;
			}
			break;
		case 1:
			LogError("Bison finalizo debido a un error de sintaxis.");
			break;
		case 2:
			LogError("Bison finalizo abruptamente debido a que ya no hay memoria disponible.");
			break;
		case 3:
			LogError("Error de redefinición de variable.");
			break;
		default:
			LogError("Error desconocido mientras se ejecutaba el analizador Bison (código %d).", result);
	}
	LogInfo("Fin.");
	cleanup_variables(state.map);
	return result;
}
