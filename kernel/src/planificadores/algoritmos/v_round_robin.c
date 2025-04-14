#include "v_round_robin.h"

#define ONE_SEC_TO_MILI 1000
#define ONE_MILI_TO_NANO 1000000

bool send_Kernel_cpu_int_desalojo_call_vrr = false;
bool send_Kernel_cpu_execute_call_vrr = false;

t_temporal *elapsedQ = NULL;

// 1 millisecond to nanoseconds = 1,000,000.00 ns

static void quantumVRR(t_pcb *pcb)
{
    // TODO(fed): acá tenemos un memory leak pero me está rompiendo
    // if (elapsedQ != NULL)
    // {
    // temporal_stop(elapsedQ);
    // }
    elapsedQ = temporal_create(); // timestamp cuando arranca

    int milisecs = pcb->quantum;
    int secs = milisecs / ONE_SEC_TO_MILI;
    int r_milisecs = milisecs % ONE_SEC_TO_MILI;
    int r_nanosecs = r_milisecs * ONE_MILI_TO_NANO;

    struct timespec request = {secs, r_nanosecs};
    struct timespec remaining = {secs, r_nanosecs};

    int response = nanosleep(&request, &remaining);

    if (response == 0)
    {
        log_trace(get_logger(), "quantumVRR completed ok\n");
    }
    else
    {
        log_error(get_logger(), "Something went wrong with quantumVRR\n");
    }

#ifdef TEST
    send_Kernel_cpu_int_desalojo_call_vrr = true;
    t_log *logger = get_logger();
    log_info(logger, "KERNEL_CPU_INT_DESALOJO: Ponele que se ejecuto bien");
#else
    send_interrupt(pcb->pid, (op_code)KERNEL_CPU_INT_DESALOJO);
#endif
}

void exec_a_ready_vrr()
{
    temporal_stop(elapsedQ);

    t_pcb *pcb_executing = get_executing();

#ifndef TEST
    bool dead = check_blacklist(EXEC_A_READY, pcb_executing->pid, NULL);
    if (dead)
    {
        temporal_destroy(elapsedQ);
        return;
    }
#endif
    set_executing(NULL);

    int time_elapsed = (int)temporal_gettime(elapsedQ);
    int time_remaining = pcb_executing->quantum - time_elapsed;
    log_trace(get_logger(), "PCB_Q (%i) - TIME_ELAPSED (%i) = time_remaining %i", pcb_executing->quantum, time_elapsed,
              time_remaining);

    temporal_destroy(elapsedQ);

    if (time_remaining > 0)
    {
        // esto no debería ocurrir nunca
        pcb_executing->quantum = time_remaining;
        add_pcb("PREADY", pcb_executing);
    }
    else
    {
        pcb_executing->quantum = (get_config())->QUANTUM;
        add_pcb("READY", pcb_executing);
    }
}

void update_pcb_q(t_pcb *pcb)
{
    t_config_kernel *cfg = get_config();
    if (!!strcmp(cfg->ALGORITMO_PLANIFICACION, "VRR"))
    {
        return;
    }

    temporal_stop(elapsedQ);
    int time_elapsed = (int)temporal_gettime(elapsedQ);
    int time_remaining = pcb->quantum - time_elapsed;
    temporal_destroy(elapsedQ);

    log_trace(get_logger(), "PCB_Q (%i) - TIME_ELAPSED (%i) = time_remaining %i", pcb->quantum, time_elapsed,
              time_remaining);

    if (time_remaining > 0)
    {
        pcb->quantum = time_remaining;
    }
    else
    {
        pcb->quantum = cfg->QUANTUM;
    }
}

void enviar_proceso_v_round_robin()
{
    log_trace(get_logger(), "VRR");

    t_pcb *pcb = get_next_pcb("PREADY");
    if (pcb == NULL)
    {
        pcb = get_next_pcb("READY");
    }

    bool dead = false;

#ifndef TEST
    do
    {
        if (pcb == NULL)
        {
            log_trace(get_logger(), "Las listas de ready y pready están vacías");
            return;
        }

        dead = check_blacklist(READY_A_EXEC, pcb->pid, pcb);

        if (dead)
        {
            pcb = get_next_pcb("PREADY");
            if (pcb == NULL)
            {
                pcb = get_next_pcb("READY");
            }
        }
    } while (dead);
#else
    if (pcb == NULL)
    {
        log_trace(get_logger(), "Las listas de ready y pready están vacía");
        return;
    }
#endif

    sem_wait(&executing_mutex);
    set_executing(pcb);
    sem_post(&executing_mutex);
    log_cambio_de_estado((int)pcb->pid, "READY", "EXEC");

#ifdef TEST
    send_Kernel_cpu_execute_call_vrr = true;
    t_log *logger = get_logger();
    log_info(logger, "KERNEL_CPU_EXECUTE: Ponele que se ejecuto bien");
#else
    send_dispatch(create_contexto(pcb));
#endif
    pthread_t thread_quantumVRR;
    pthread_create(&thread_quantumVRR, NULL, (void *)quantumVRR, pcb);
    pthread_detach(thread_quantumVRR);
}