#include <stdio.h>
#include "multiprogramacion/handlers_multiprogramacion.h"
#include "handlers/colas_estados/colas_handler.h"
#include "semaforos_planificadores/semaforos_planificadores.h"


#include <cspecs/cspec.h>

context (multiprogramacion_handlers_tests) {

    before {
        init_mutex_multiprogramacion();
    } end

    after {
        destroy_mutex_multiprogramacion();
    } end

    describe("Multiprogramacion Handlers") {

        it("Deberian funcionar todos los handlers correctamente") {
            initialize_lists();
            inicializar_semaforos_pausa_planificador_corto();
            gh_lpm();

            set_multiprogramacion(1);
            increment_multiprogramacion();
            should_int(get_multiprogramacion()) be equal to(2);
        
            increment_n_multiprogramacion(3);
            should_int(get_multiprogramacion()) be equal to(5);
        
            decrement_multiprogramacion();
            should_int(get_multiprogramacion()) be equal to(4);
        
            decrement_n_multiprogramacion(3);
            should_int(get_multiprogramacion()) be equal to(1);
        
            set_multiprogramacion(4);
            update_multiprogramacion(7);
            should_int(get_multiprogramacion()) be equal to(7);
            update_multiprogramacion(2);
            should_int(get_multiprogramacion()) be equal to(2);

            destroy_lists();
            
        } end

    } end

}