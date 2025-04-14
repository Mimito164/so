#include "instrucciones/cargar_instrucciones.h"
#include "estructuras_administrativas/tabla_instrucciones.h"
#include <cspecs/cspec.h>

context(cargar_instrucciones) {

   describe("Obtener instrucciones de la tabla de instrucciones"){

        char *path = malloc(24); 
        // *string = malloc(str_length);
        
        char * _path = "codigos_prueba/PRUEBA_2";
        memcpy(path, _path, 24);


        int program_counter = 2;
        char *instruccion = "MOV AX DX";
        uint32_t pid = 0;

        before {
            crear_tabla_instrucciones();
        }end

        after {
            destroy_tabla_de_instrucciones();
        }end
        
        it("debería ser cargado y accedido correctamente"){
            agregar_instrucciones_en_tabla(pid, cargar_instrucciones(path));
            char *instruccion_obtenida = obtener_instruccion(pid, program_counter);

            should_string(instruccion_obtenida) be equal to (instruccion);
        }end

    }end
}