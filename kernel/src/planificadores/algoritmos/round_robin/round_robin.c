#include "planificadores/algoritmos/round_robin/round_robin.h"

#define ONE_SEC_TO_MILI 1000
#define ONE_MILI_TO_NANO 1000000

bool send_Kernel_cpu_int_desalojo_call_rr = false;
bool send_Kernel_cpu_execute_call_rr = false;
bool hay_fin_q = false;

static t_temporal *elapsedQ = NULL;

static void quantumRR(quantum_thread_args *args)
{
    elapsedQ = temporal_create(); // timestamp cuando arranca

    int milisecs = get_config()->QUANTUM;
    int secs = milisecs / ONE_SEC_TO_MILI;
    int r_milisecs = milisecs % ONE_SEC_TO_MILI;
    int r_nanosecs = r_milisecs * ONE_MILI_TO_NANO;

    struct timespec request = {secs, r_nanosecs};
    struct timespec remaining = {secs, r_nanosecs};

    int response = nanosleep(&request, &remaining);

    if (response == 0)
    {
        log_trace(get_logger(), "quantumRR completed ok\n");
    }
    else
    {
        log_error(get_logger(), "Something went wrong with quantumRR\n");
    }

#ifdef TEST
    send_Kernel_cpu_int_desalojo_call_rr = true;
    t_log *logger = get_logger();
    log_info(logger, "KERNEL_CPU_INT_DESALOJO: Ponele que se ejecuto bien");
#else
    if (args->is_active)
    {
        hay_fin_q = true;
        send_interrupt(args->pid, (op_code)KERNEL_CPU_INT_DESALOJO);
    }
    args->ended = true;
#endif
}

void enviar_proceso_round_robin()
{
    log_trace(get_logger(), "RR");
    t_pcb *pcb = get_next_pcb("READY");

    bool dead = false;

#ifndef TEST
    do
    {
        if (pcb == NULL)
        {
            log_trace(get_logger(), "La lista de ready está vacía");
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
        log_trace(get_logger(), "La lista de ready está vacía");
        return;
    }
#endif
    sem_wait(&executing_mutex);
    set_executing(pcb);
    sem_post(&executing_mutex);
    log_cambio_de_estado((int)pcb->pid, "READY", "EXEC");

#ifdef TEST
    send_Kernel_cpu_execute_call_rr = true;
    t_log *logger = get_logger();
    log_info(logger, "KERNEL_CPU_EXECUTE: Ponele que se ejecuto bien");
#else
    send_dispatch(create_contexto(get_executing()));
#endif
    quantum_thread_args *new_quantum_thread_args = malloc(sizeof(quantum_thread_args));
    new_quantum_thread_args->pid = pcb->pid;
    new_quantum_thread_args->is_active = true;
    new_quantum_thread_args->ended = false;
    pthread_mutex_init(&new_quantum_thread_args->args_mutex, NULL);
    pthread_t quantum_thread;
    add_argument((void *)new_quantum_thread_args);
    pthread_create(&quantum_thread, 0, (void *)quantumRR, new_quantum_thread_args);
    pthread_detach(quantum_thread);
}