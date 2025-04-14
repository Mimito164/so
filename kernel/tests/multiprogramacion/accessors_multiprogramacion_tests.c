#include <stdio.h>
#include "multiprogramacion/accessors_multiprogramacion.h"
#include "globals/globals.h"

#include <cspecs/cspec.h>

context (multiprogramacion_accessors_tests) {

    before {
        init_mutex_multiprogramacion();
    } end

    after {
        destroy_mutex_multiprogramacion();
        mem_clean_exit();
    } end

    describe("Multiprogramacion Accessors") {

        it("Deberian funcionar todos los accessors correctamente") {
            set_multiprogramacion(0);
            should_int(get_multiprogramacion()) be equal to(0);

            set_multiprogramacion(12);
            should_int(get_multiprogramacion()) be equal to(12);
            
        } end

    } end

}