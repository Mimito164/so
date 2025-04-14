#include <hilo_genesis/hilo_genesis.h>
#include "handlers/colas_estados/colas_handler.h"
#include "globals/globals.h"

#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>
#include <utils/hello.h>

context (hilo_genesis) {

    bool send_Kernel_memoria_creacion_call;
    
    describe("Hilo Genesis") {
        before {
            initialize_cfg("tests/testing.config");
            fclose(fopen("logs/test_hilo_genesis.log", "w")); // limpiar el archivo de logs en cada prueba
            initialize_lists();
        } end

        after {
            gh_lpm();
            destroy_lists();
        } end

        it("Deberia agregar un pcb a la lista de new y mandar un mensaje a memoria") {
            iniciar_proceso("tests/hilo_genesis/Ejemplo_pseudocodigo");
            sleep(1);
            int largo_new = len_list("NEW");
            should_int(largo_new) be equal to(1);

            // esta funcion deberia haber sido llamada: send_Kernel_memoria_creacion
            should_bool(send_Kernel_memoria_creacion_call) be equal to(true);
            should_bool(funcion_hilo_admision_call) be equal to(true);
        } end
    } end

}