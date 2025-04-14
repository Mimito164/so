#include "admision.h"

static bool hay_procesos_en_new()
{
    int length = len_list("NEW");
    return length > 0;
}

static bool hay_vacantes_multiprogramacion()
{
    int multiprog = get_multiprogramacion();
    int length_ready = len_list("READY");
    int length_blocked = len_list("BLOCKED");

    pthread_mutex_lock(&mutex_exec);
    int length_exec = get_executing() == NULL ? 0 : 1;
    pthread_mutex_unlock(&mutex_exec);

    return (multiprog - (length_ready + length_blocked + length_exec)) > 0;
}

void admit()
{
    sem_wait(&semaforo_transicion_new_ready);

    if (!hay_procesos_en_new() || !hay_vacantes_multiprogramacion())
    {
        if (!hay_procesos_en_new())
        {
            log_trace(get_logger(), "Admit(): No hay procesos en new");
        }

        if (!hay_vacantes_multiprogramacion())
        {
            log_trace(get_logger(), "Admit(): No hay vacantes en multiprogramación");
        }
        sem_post(&semaforo_transicion_new_ready);

        return;
    }
    while (hay_procesos_en_new() && hay_vacantes_multiprogramacion())
    {
        t_pcb *admited_pcb = get_next_pcb("NEW");

#ifndef TEST
        bool is_dead = false;
        do
        {
            if (admited_pcb == NULL)
            {
                log_trace(get_logger(), "Admit(): La lista de NEW está vacía");
                sem_post(&semaforo_transicion_new_ready);
                return;
            }

            is_dead = check_blacklist(NEW_A_READY, admited_pcb->pid, admited_pcb);

            if (is_dead)
            {
                admited_pcb = get_next_pcb("NEW");
            }
        } while (is_dead);
#else
        if (admited_pcb == NULL)
        {
            sem_post(&semaforo_transicion_new_ready);
            log_trace(get_logger(), "Admit(): La lista de NEW está vacía");
            return;
        }
#endif

        add_pcb("READY", admited_pcb);
        log_cambio_de_estado((int)admited_pcb->pid, "NEW", "READY");
    }

#ifndef TEST
    s_planificador_corto_args args = {.transicion = READY_A_EXEC};
    planificador_corto(args);
#endif
    sem_post(&semaforo_transicion_new_ready);
}