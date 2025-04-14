#include <stdio.h>
#include <stdbool.h>
#include <finalizacion/finalizacion.h>
#include <finalizacion/blacklist.h>
#include <commons/collections/dictionary.h>
#include "handlers/colas_estados/colas_handler.h"
#include <utils/console.h>
#include <sys/time.h>
#include <cspecs/cspec.h>

#define TIMEOUT_SEC 3

void wait_for_blacklist(statics_t * statics, int size_start, int size_target)
{
            pthread_mutex_t blacklist_mutex = statics->blacklist_mutex;

            int blacklist_size = size_start;
            
            time_t start = time(NULL);
            while (blacklist_size != size_target)
            {

                pthread_mutex_lock(&blacklist_mutex);
                blacklist_size = (*(statics->blacklist))->table_current_size;
                pthread_mutex_unlock(&blacklist_mutex);
                
                long time_elapsed = time(NULL) - start; // in seconds
                if (time_elapsed > TIMEOUT_SEC)
                {
                    printf("1. Timeouted: %li \n", time_elapsed);
                    break;
                }
            }

}


context (Finalizacion) {

    describe("Finalizacion") {

        // consola_con_censura();
        initialize_cfg("tests/testing.config");
        statics_t * statics  = get_statics();

        before {
            initialize_lists();
            gh_lpm();
            statics->init_blacklist();
        } end

        after {
            gh_lpm();
            destroy_lists();
            destroy_blacklist();
        } end

        it("debería poder blacklistear pids") {
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3 = create_pcb();
            
            blackilst_process(pcb1->pid, (motivo_de_muerte)INTERRUPTED_BY_USER);
            blackilst_process(pcb2->pid, (motivo_de_muerte)INTERRUPTED_BY_USER);
            blackilst_process(pcb3->pid, (motivo_de_muerte)INTERRUPTED_BY_USER);

            wait_for_blacklist(statics, 0, 3);

            should_bool(statics->find_in_blacklist(pcb1->pid)) be equal to(true);
            should_bool(statics->find_in_blacklist(pcb2->pid)) be equal to(true);
            should_bool(statics->find_in_blacklist(pcb3->pid)) be equal to(true);
            
            destroy_pcb(pcb1);
            destroy_pcb(pcb2);
            destroy_pcb(pcb3);

            pcb1 = NULL;
            pcb2 = NULL;
            pcb3 = NULL;

        } end

        it("debería eliminar procesos si están en la blacklist y se produce una transición") {
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3 = create_pcb();
            t_pcb* pcb4 = create_pcb();
            uint32_t pcb1_pid = pcb1->pid;
            uint32_t pcb2_pid = pcb2->pid;
            uint32_t pcb3_pid = pcb3->pid;
            uint32_t pcb4_pid = pcb4->pid;

            // Agrego a la blacklist
            blackilst_process(pcb1_pid, (motivo_de_muerte)INTERRUPTED_BY_USER);
            blackilst_process(pcb2_pid, (motivo_de_muerte)INTERRUPTED_BY_USER);
            blackilst_process(pcb3_pid, (motivo_de_muerte)INTERRUPTED_BY_USER);

            wait_for_blacklist(statics, 0, 3);

            // Trato de eliminar procesos que no están en las colas de estado
            bool found = check_blacklist(NEW_A_READY, pcb1_pid, NULL);
            should_bool(found) be equal to(false);

            found = check_blacklist(READY_A_EXEC, pcb2_pid, NULL);
            should_bool(found) be equal to(false);

            found = check_blacklist(BLOCKED_A_READY, pcb3_pid, NULL);
            should_bool(found) be equal to(false);

            // Agrego los procesos a las colas de estado
            add_pcb("NEW", pcb1);
            add_pcb("READY", pcb2);
            add_pcb("BLOCKED", pcb3);
            add_pcb("NEW", pcb4);

            found = check_blacklist(NEW_A_READY, pcb1_pid, NULL);
            should_bool(found) be equal to(true);

            found = check_blacklist(READY_A_EXEC, pcb2_pid, NULL);
            should_bool(found) be equal to(true);

            found = check_blacklist(BLOCKED_A_READY, pcb3_pid, NULL);
            should_bool(found) be equal to(true);

            t_pcb * pcb1_post = find_pcb_by_pid("NEW", pcb1_pid);
            t_pcb * pcb2_post = find_pcb_by_pid("READY",pcb2_pid);
            t_pcb * pcb3_post = find_pcb_by_pid("BLOCKED", pcb3_pid);

            should_ptr(pcb1_post) be equal to(NULL); // No existe porque fue eliminado
            should_ptr(pcb2_post) be equal to(NULL); // No existe porque fue eliminado
            should_ptr(pcb3_post) be equal to(NULL); // No existe porque fue eliminado

            t_pcb * pcb4_post = find_pcb_by_pid("NEW", pcb4_pid); // find_pcb_by_pid lo quita de la lista
            should_int(pcb4_post->pid) be equal to(pcb4_pid); // Existe porque NO fue eliminado

            // destroy_pcb(pcb1);
            // destroy_pcb(pcb2);
            // destroy_pcb(pcb3);
            destroy_pcb(pcb4);

            // pcb1 = NULL;
            // pcb2 = NULL;
            // pcb3 = NULL;
            pcb4 = NULL;

        } end

        it("no debería eleiminar procesos si no están en la blacklist") {
            t_pcb* pcb5 = create_pcb();
            uint32_t pcb5_pid = pcb5->pid;
            add_pcb("NEW", pcb5);

            bool found = check_blacklist(NEW_A_READY, pcb5_pid, NULL);
            should_bool(found) be equal to(false);

            t_pcb * pcb5_post = find_pcb_by_pid("NEW", pcb5_pid);
            should_int(pcb5_post->pid) be equal to(pcb5_pid);

            destroy_pcb(pcb5);
            pcb5 = NULL;
        } end

        it("debería poder eliminar procesos aunque no hayan transiciones") {
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3 = create_pcb();
            t_pcb* pcb4 = create_pcb();
            t_pcb* pcb5 = create_pcb();
            uint32_t pcb1_pid = pcb1->pid;
            uint32_t pcb2_pid = pcb2->pid;
            uint32_t pcb3_pid = pcb3->pid;
            uint32_t pcb4_pid = pcb4->pid;
            uint32_t pcb5_pid = pcb5->pid;

            // Agrego procesos a la lista de NEW
            add_pcb("NEW", pcb1);
            add_pcb("NEW", pcb2);
            add_pcb("NEW", pcb3);
            add_pcb("NEW", pcb4);
            add_pcb("NEW", pcb5);

            // Agrego procesos a la blacklist
            blackilst_process(pcb1->pid, (motivo_de_muerte)INTERRUPTED_BY_USER);
            blackilst_process(pcb2->pid, (motivo_de_muerte)INTERRUPTED_BY_USER);
            blackilst_process(pcb3->pid, (motivo_de_muerte)INTERRUPTED_BY_USER);
            blackilst_process(pcb4->pid, (motivo_de_muerte)INTERRUPTED_BY_USER);
            blackilst_process(pcb5->pid, (motivo_de_muerte)INTERRUPTED_BY_USER);

            wait_for_blacklist(statics, 0, 5);

            find_and_kill(pcb1->pid);
            find_and_kill(pcb2->pid);
            find_and_kill(pcb3->pid);
            find_and_kill(pcb4->pid);

            wait_for_blacklist(statics, 5, 1);

            t_pcb * pcb1_post = find_pcb_by_pid("NEW", pcb1_pid);
            t_pcb * pcb2_post = find_pcb_by_pid("NEW", pcb2_pid);
            t_pcb * pcb3_post = find_pcb_by_pid("NEW", pcb3_pid);
            t_pcb * pcb4_post = find_pcb_by_pid("NEW", pcb4_pid);
            t_pcb * pcb5_post = find_pcb_by_pid("NEW", pcb5_pid); // lo saca de la cola -_-
                    
            should_ptr(pcb1_post) be equal to(NULL);
            should_ptr(pcb2_post) be equal to(NULL);
            should_ptr(pcb3_post) be equal to(NULL);
            should_ptr(pcb4_post) be equal to(NULL);

            should_int(pcb5_post->pid) be equal to(pcb5_pid); // no debería haber muerto aún

            add_pcb("NEW", pcb5); // pq find_pcb_by_pid lo saca de la cola
            find_and_kill(pcb5_pid);

            wait_for_blacklist(statics, 1, 0);

            pcb5_post = find_pcb_by_pid("NEW", pcb5_pid);
            should_ptr(pcb5_post) be equal to(NULL);

            // destroy_pcb(pcb1);
            // destroy_pcb(pcb2);
            // destroy_pcb(pcb3);
            // destroy_pcb(pcb4);
            // destroy_pcb(pcb5);

            // pcb1 = NULL;
            // pcb2 = NULL;
            // pcb3 = NULL;
            // pcb4 = NULL;
            // pcb5 = NULL;

        } end

        // consola_con_libre_expresion();
        free(statics);
        destroy_config_kernel();
    } end

}