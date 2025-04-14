#include <stdio.h>
#include <stdbool.h>
#include "comunicacion_kernel/interruptHandlers/interrupt_handlers.h"

#include <cspecs/cspec.h>

context (interrupt_handlers_tests) {

    before {
        init_mutex_interrupt();
    } end

    after {
        destroy_mutex_interrupt();
    } end

    describe("Interrupt Handlers") {

        it("should set, get and clear an interrupt") {
            interrupt_type tipo_esperado = KILL;
            int pid_esperado = 7;
            set_interrupt(pid_esperado, tipo_esperado);

            should_bool(get_interrupt_state()) be equal to(true);
            should_int(get_interrupt_pid()) be equal to(pid_esperado);
            should_int(get_interrupt_type()) be equal to(tipo_esperado);

            clear_interrupt();
            pid_esperado = -1;
            tipo_esperado = NONE;

            should_bool(get_interrupt_state()) be equal to(false);
            should_int(get_interrupt_pid()) be equal to(pid_esperado);
            should_int(get_interrupt_type()) be equal to(tipo_esperado);
        } end
    } end

}