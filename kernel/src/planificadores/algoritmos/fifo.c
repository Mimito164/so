#include "planificadores/algoritmos/round_robin/round_robin.h"

bool send_Kernel_cpu_execute_call_fifo = false;

void enviar_proceso_fifo()
{
    log_trace(get_logger(), "FIFO");
    t_pcb *pcb = get_next_pcb("READY");

    bool dead = false;

#ifndef TEST
    do
    {
        if (pcb == NULL)
        {
            log_info(get_logger(), "La lista de ready está vacía");
            return;
        }

        dead = check_blacklist(READY_A_EXEC, pcb->pid, pcb);

        if (dead)
        {
            pcb = get_next_pcb("READY");
        }
    } while (dead);
#else
    if (pcb == NULL)
    {
        log_info(get_logger(), "La lista de ready está vacía");
        return;
    }
#endif

    sem_wait(&executing_mutex);
    set_executing(pcb);
    sem_post(&executing_mutex);

    log_cambio_de_estado((int)pcb->pid, "READY", "EXEC");

#ifdef TEST
    send_Kernel_cpu_execute_call_fifo = true;
    t_log *logger = get_logger();
    log_info(logger, "KERNEL_CPU_EXECUTE: Ponele que se ejecuto bien");
#else
    send_dispatch(create_contexto(get_executing()));
#endif
}