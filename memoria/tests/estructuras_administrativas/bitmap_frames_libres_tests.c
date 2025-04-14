#include "estructuras_administrativas/bitmap_frames_libres.h"
#include <cspecs/cspec.h>
#include "t_config/t_config.h"

context(bitmap_frames_libres) {

   describe("Crear bitmap de frames libres"){

        before {
            initialize_cfg("../memoria/tests/testing.config");
            crear_bitmap_frames_libres();
        }end

        after {
            destroy_bitmap_frames_libres();
            destroy_config_memoria();
        }end
        
        it("debería ser creado con el tamanio correcto"){
            t_bitarray *bitmap_frames_libres = get_bitmap();
            int tam_mem = get_config()->TAM_MEMORIA;
            int tam_pag = get_config()->TAM_PAGINA;

            should_int( bitarray_get_max_bit(bitmap_frames_libres)) be equal to (tam_mem/tam_pag * 8);
        }end

    }end

    describe("Modificar bitmap de frames libres"){

        before {
            initialize_cfg("../memoria/tests/testing.config");
            crear_bitmap_frames_libres();
        }end

        after {
            destroy_bitmap_frames_libres();
            destroy_config_memoria();
        }end
        
        it("debería estar ocupado"){
            int indice = 2;
            bool esperado = false;
            asignarFrame(indice);
            should_bool( esFrameLibre(indice)) be equal to (esperado);
        }end

    }end

    describe("Liberar frames"){

        before {
            initialize_cfg("../memoria/tests/testing.config");
            crear_bitmap_frames_libres();
        }end

        after {
            destroy_bitmap_frames_libres();
            destroy_config_memoria();
        }end
        
        it("debería estar libre"){
            int indice = 2;
            bool esperado = true;
            asignarFrame(indice);
            liberar_frame(indice);
            should_bool( esFrameLibre(indice)) be equal to (esperado);
        }end

    }end
}