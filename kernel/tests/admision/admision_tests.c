#include <stdio.h>
#include <stdbool.h>
// #include "handlers/colas_estados/colas_handler.h"
#include "admision/admision.h"
#include "semaforos_planificadores/semaforos_planificadores.h"
#include <cspecs/cspec.h>

context (admision) {

    describe("admit() realiza la transision de NEW a READY cuando corresponda") {
        before {
            gh_lpm();
            initialize_lists();
            init_mutex_multiprogramacion();
            inicializar_semaforos_pausa_planificador_corto();
        } end

        after {
            gh_lpm();
            destroy_lists();
            destroy_mutex_multiprogramacion();
            destruir_semaforos_pausa_planificador_corto();
        } end

        it("Con multiprogramacion en 0 no admite ningun pcb") {
            set_multiprogramacion(0);
            t_pcb* pcb1 = create_pcb();
            add_pcb("NEW", pcb1);
            admit();

            int new_lenght = len_list("NEW");
            int rdy_lenght = len_list("READY");
            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(0);
        } end

        it("Con multiprogramacion en 1 admite un pcb y solo uno") {
            set_multiprogramacion(1);
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            add_pcb("NEW", pcb1);
            add_pcb("NEW", pcb2);
            int new_lenght = len_list("NEW");
            int rdy_lenght = len_list("READY");
            should_int(new_lenght) be equal to(2);
            should_int(rdy_lenght) be equal to(0);
            
            admit();
            
            new_lenght = len_list("NEW");
            rdy_lenght = len_list("READY");

            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(1);
            admit();
            new_lenght = len_list("NEW");
            rdy_lenght = len_list("READY");
            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(1);
        } end

        it("Idem anterior pero ahora el pcb esta en blocked") {
            set_multiprogramacion(1);
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            add_pcb("NEW", pcb1);
            add_pcb("NEW", pcb2);
            int new_lenght = len_list("NEW");
            int rdy_lenght = len_list("READY");
            should_int(new_lenght) be equal to(2);
            should_int(rdy_lenght) be equal to(0);
    
            admit();
            t_pcb* temp_pcb = get_next_pcb("READY");
            add_pcb("BLOCKED", temp_pcb);

            new_lenght = len_list("NEW");
            rdy_lenght = len_list("READY");
            int blk_length = len_list("BLOCKED");
            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(0);
            should_int(blk_length) be equal to(1);

            admit();
            new_lenght = len_list("NEW");
            rdy_lenght = len_list("READY");
            blk_length = len_list("BLOCKED");
            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(0);
            should_int(blk_length) be equal to(1);
        } end

        it("Idem anterior pero ahora el pcb esta en executing") {
            set_multiprogramacion(1);
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            add_pcb("NEW", pcb1);
            add_pcb("NEW", pcb2);
            int new_lenght = len_list("NEW");
            int rdy_lenght = len_list("READY");
            should_int(new_lenght) be equal to(2);
            should_int(rdy_lenght) be equal to(0);
    
            admit();
            new_lenght = len_list("NEW");
            rdy_lenght = len_list("READY");
            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(1);

            t_pcb* temp_pcb = get_next_pcb("READY");
            pthread_mutex_lock(&mutex_exec);
            t_pcb* exec_pcb = get_executing();
            should_ptr(exec_pcb) be equal to(NULL);

            exec_pcb = temp_pcb;
            pthread_mutex_unlock(&mutex_exec);
            
            new_lenght = len_list("NEW");
            rdy_lenght = len_list("READY");
            
            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(0);
            should_ptr(exec_pcb) not be equal to(NULL);

            admit();
            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(0);
            should_ptr(exec_pcb) not be equal to(NULL);
        } end

        it("Con multiprogramacion en 2 admite dos pcb's y solo dos") {
            set_multiprogramacion(2);
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3= create_pcb();
            add_pcb("NEW", pcb1);
            add_pcb("NEW", pcb2);
            add_pcb("NEW", pcb3);
            int new_lenght = len_list("NEW");
            int rdy_lenght = len_list("READY");
            should_int(new_lenght) be equal to(3);
            should_int(rdy_lenght) be equal to(0);
            
            admit();
            new_lenght = len_list("NEW");
            rdy_lenght = len_list("READY");
            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(2);
            
            admit();
            new_lenght = len_list("NEW");
            rdy_lenght = len_list("READY");
            should_int(new_lenght) be equal to(1);
            should_int(rdy_lenght) be equal to(2);
        } end

    } end

}
