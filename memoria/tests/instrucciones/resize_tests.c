#include "estructuras_administrativas/bitmap_frames_libres.h"
#include "estructuras_administrativas/tabla_paginas.h"
#include "t_config/t_config.h"
#include <cspecs/cspec.h>
#include <commons/collections/list.h>
#include "instrucciones/resize.h"

context(resize) {

   describe("Aumentar tamanio"){

        uint32_t pid = 0;

        before {
            initialize_cfg("../memoria/tests/testing.config");
            set_tam_pag(4);
            crear_bitmap_frames_libres();
            crear_tabla_paginas_global();
            agregar_tabla_paginas_proceso_en_tabla_global(pid);
        }end

        after {
            destroy_bitmap_frames_libres();
            destroy_config_memoria();
            destroy_tabla_paginas_global();
        }end
        
        it("deberia ser aumentado correctamente"){
            t_tabla_paginas_proceso *tabla;
            int tamanio_esperado = 3;
            resize(pid,12);
            tabla = buscar_tabla_paginas_proceso(pid);

            should_int(list_size(tabla->frames)) be equal to (tamanio_esperado);
        }end

    }end

    describe("Disminuir tamanio"){

        uint32_t pid = 0;

        before {
            initialize_cfg("../memoria/tests/testing.config");
            set_tam_pag(4);
            crear_bitmap_frames_libres();
            crear_tabla_paginas_global();
            agregar_tabla_paginas_proceso_en_tabla_global(pid);
        }end

        after {
            destroy_bitmap_frames_libres();
            destroy_config_memoria();
            destroy_tabla_paginas_global();
        }end
        
        it("deberia ser disminuido correctamente"){
            t_tabla_paginas_proceso *tabla;
            int tamanio_esperado = 1;
            int indice = 0;
            resize(pid,12);
            resize(pid,4);
            tabla = buscar_tabla_paginas_proceso(pid);

            should_int(list_size(tabla->frames)) be equal to (tamanio_esperado);
            should_bool( esFrameLibre(indice)) be equal to (false);
            should_bool( esFrameLibre(1)) be equal to (true);
        }end

    }end

    describe("Aumentar tamanio, cuando ya tiene frames asignados"){

        uint32_t pid = 0;

        before {
            initialize_cfg("../memoria/tests/testing.config");
            set_tam_pag(4);
            crear_bitmap_frames_libres();
            crear_tabla_paginas_global();
            agregar_tabla_paginas_proceso_en_tabla_global(pid);
        }end

        after {
            destroy_bitmap_frames_libres();
            destroy_config_memoria();
            destroy_tabla_paginas_global();
        }end
        
        it("deberia ser aumentado correctamente"){
            t_tabla_paginas_proceso *tabla;
            int tamanio_esperado = 10;
            int indice = 0;
            resize(pid,12);
            resize(pid,38);
            tabla = buscar_tabla_paginas_proceso(pid);

            should_int(list_size(tabla->frames)) be equal to (tamanio_esperado);
        }end

    }end
}