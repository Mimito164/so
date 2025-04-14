#include <stdio.h>
#include <stdbool.h>
#include "planificadores/algoritmos/fifo.h"
#include "planificadores/algoritmos/round_robin/round_robin.h"
#include "planificador_corto/planificador_corto.h"
#include "globals/globals.h"
#include <cspecs/cspec.h>


context (algoritmos_planificacion) {

    describe("Algoritmos de Planificacion") {

        before {
            initialize_cfg("tests/testing.config");
            initialize_lists();
            t_pcb* pcb = create_pcb();
            add_pcb("READY", pcb);
            sem_init(&executing_mutex, 0, 1);
        } end

        after {
            gh_lpm();
            destroy_lists();
            sem_destroy(&executing_mutex);
        } end

        it("Deberia mandar un mensaje a cpu mediante algoritmo FIFO") {
            enviar_proceso_fifo();
            // should_bool(send_Kernel_cpu_execute_call_fifo) be equal to(true);
        } end

        // it("Deberia mandar un mensaje a cpu mediante algoritmo RR y hacer el sleep") {
        //   enviar_proceso_round_robin();
        //   sleep(3); // esto es porque send_Kernel_cpu_int_desalojo_call_rr cambia de valor luego del sleep
        //   should_bool(send_Kernel_cpu_int_desalojo_call_rr) be equal to(true);
        //   should_bool(send_Kernel_cpu_execute_call_rr) be equal to(true);
        // } end
    } end

}
