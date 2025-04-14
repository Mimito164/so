#include "estructuras_administrativas/tabla_paginas.h"
#include <cspecs/cspec.h>
#include <commons/collections/list.h>

context(cargar_tabla_de_paginas) {

   describe("Crear tabla de paginas del proceso"){

        uint32_t pid = 0;
        t_tabla_paginas_proceso *tabla;
        int tamanio_esperado = 0;
        int tamanio_esperado_tabla_global = 1; 

        before {
            crear_tabla_paginas_global();
        }end

        after {
            destroy_tabla_paginas_global();
        }end
        
        it("Su cantidad de frames deberia ser 0"){
            agregar_tabla_paginas_proceso_en_tabla_global(pid);
            tabla = buscar_tabla_paginas_proceso(pid);

            should_int(list_size(tabla->frames)) be equal to (tamanio_esperado);
        }end

        it("La tabla de paginas global deberia aumentar la cantidad de tablas por proceso en 1"){
            agregar_tabla_paginas_proceso_en_tabla_global(pid);

            should_int(obtener_tamanio_tabla_pags_global()) be equal to (tamanio_esperado_tabla_global);
        }end

    }end

    describe("Eliminar tabla de paginas del proceso"){

        uint32_t pid = 0;
        int tamanio_esperado = 0;

        before {
            crear_tabla_paginas_global();
        }end

        after {
            destroy_tabla_paginas_global();
        }end
        
        it("La tabla de paginas global deberia disminuir la cantidad de tablas por proceso en 1"){
            agregar_tabla_paginas_proceso_en_tabla_global(pid);
            t_tabla_paginas_proceso * tabla = buscar_tabla_paginas_proceso(pid);
            quitar_tabla_paginas_proceso(pid);
            destroy_tabla_paginas_proceso(tabla);
             
            should_int(obtener_tamanio_tabla_pags_global()) be equal to (tamanio_esperado);
        }end

    }end

    describe("Obtener marco"){

        uint32_t pid = 0;

        before {
            crear_tabla_paginas_global();
            initialize_cfg("../memoria/tests/testing.config");
            crear_bitmap_frames_libres();
            set_tam_pag(4);
            agregar_tabla_paginas_proceso_en_tabla_global(pid);
        }end

        after {
            destroy_bitmap_frames_libres();
            destroy_config_memoria();
            destroy_tabla_paginas_global();
        }end
        
        it("Deberia obtener el marco de la pagina solicitada"){
            int pagina = 1;
            int frame_esperado = 1;

            resize(pid,12);
            
            should_int(obtener_frame(pid,pagina)) be equal to (frame_esperado);
        }end

    }end
}