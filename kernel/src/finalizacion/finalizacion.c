#include "finalizacion.h"

static bool fin_en_new(uint32_t pid, t_pcb *pcb);
static bool fin_en_ready(uint32_t pid, t_pcb *pcb);
static bool fin_en_exec(uint32_t pid, t_pcb *pcb);
static bool fin_en_blocked(uint32_t pid, t_pcb *pcb);

bool finalizacion(t_transicion transicion, uint32_t pid, t_pcb *pcb, // NOLINT(bugprone-easily-swappable-parameters)
                  motivo_de_muerte *motivo)
{
    bool found = false;

    switch (transicion)
    {
    case NEW_A_READY:
        found = fin_en_new(pid, pcb);
        break;
    case READY_A_EXEC:
        found = fin_en_ready(pid, pcb);
        break;
    case EXEC_A_BLOCKED:
    case EXEC_A_EXIT:
    case EXEC_A_READY:
        found = fin_en_exec(pid, pcb);
        break;
    case BLOCKED_A_EXIT:
    case BLOCKED_A_READY:
        found = fin_en_blocked(pid, pcb);
        break;

    default:
        log_error(get_logger(), "No se puede finalizar un proceso en una transición inválida");
        assert(!"No se puede finalizar un proceso en una transición inválida");
        break;
    }

    if (found)
    {
#ifndef TEST
        liberar_recursos(pid);
        log_fin_proceso((int)pid, motivo);
        pthread_t HILO_ADMIT;
        pthread_create(&HILO_ADMIT, NULL, (void *)admit, NULL);
        send_Kernel_memoria_finalizacion(pid);
#endif
    }

    return found;
}

typedef enum
{
    NEW,
    READY,
    BLOCKED,
    EXEC
} statuses;

void kill_pcb(t_pcb *pcb, statuses current_status)
{
    if (current_status == EXEC)
    {
#ifndef TEST
        send_interrupt(pcb->pid, KERNEL_CPU_INT_KILL);
#endif
    }

#ifndef TEST
    send_Kernel_memoria_finalizacion(pcb->pid);
#endif
    destroy_pcb(pcb);
}

static bool fin_en_new(uint32_t pid, t_pcb *pcb)
{
    if (pcb == NULL)
    {
        pcb = find_pcb_by_pid("NEW", pid);
    }
    if (!pcb)
    {
        return false;
    }
    destroy_pcb(pcb);
    return true;
}

static bool fin_en_ready(uint32_t pid, t_pcb *pcb)
{
    if (pcb == NULL)
    {
        pcb = find_pcb_by_pid("READY", pid);
    }
    if (!pcb)
    {
        return false;
    }
    destroy_pcb(pcb);
    return true;
}

static bool fin_en_exec(uint32_t pid, t_pcb *pcb)
{
    if (pcb == NULL)
    {
        pcb = get_executing(); // esto NO lo quita de exec
    }
    if (pcb == NULL)
    {
        return false;
    }
    if (pcb->pid == pid) // checkeamos si es
    {
        destroy_pcb(pcb);
        set_executing(NULL); // lo quitamos
#ifndef TEST
        send_interrupt(pid,
                       KERNEL_CPU_INT_KILL_ALREADY_KILLED); // cpu tolera que llegue una interrupcion que no coincida el
                                                            // pid, por si mandamos una interrupcion duplicada
                                                            // creamos un nuevo op_code xq el proceso ya esta muerto
                                                            // si recibimos el kill de cpu genera segmentation fault
#endif
        return true;
    }
    return false;
}

static bool fin_en_blocked(uint32_t pid, t_pcb *pcb)
{
    if (pcb == NULL)
    {
        pcb = find_pcb_by_pid("BLOCKED", pid);
    }
    if (!pcb)
    {
        return false;
    }
    // TODO(fed): En el gestor de recrusos si está en una IO, al recibir la respuesta no va a encontrar la solicitud
    // TODO(fed): Quizá valga la pena tener una variable en la solicitud que indique que el proceso fue killed y
    // borrarla cuanddo recibamos la response de io
    destroy_pcb(pcb);
    return true;
}

/*
El fin de los procesos llega a todos por igual.
Sin misericordia, sin excepciones.

⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢤⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⡾⠿⢿⡀⠀⠀⠀⠀⣠⣶⣿⣿⣿⣷
⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣦⣴⣿⡋⠀⠀⠈⢳⡄⠀⢠⣾⣿⣿⡿⠋⠀⣿⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⠿⠛⠉⠉⠁⠀⠀⠀⠹⡄⣿⣿⣿⣿⠁⠀⢠⣿
⠀⠀⠀⠀⠀⣠⣾⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⣰⣏⢻⣿⣿⣿⠀⠀⣿⡿
⠀⠀⠀⢀⣴⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣆⠹⣿⣿⣆⢠⣿⡇
⠀⠀⢀⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⡿⠿⠿⠿⢳⠸⣿⣿⣾⣿⣧⠀⠀
⠀⢠⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣶⣿⣿⣿⣿⣿⣿⣿⠃⠈⠙⣿⣿⣿⣷⠀
⠰⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⢀⣀⢿⣿⣿⣿⠀
⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠸⣿⣆⠹⣿⢿⣷
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠛⠿⢿⣿⣿⣿⣿⣧⠀⣿⣿⠀⠀⣠⡿
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣶⣦⣉⠛⢿⡿⢠⣿⣿⣿⣆⢻⡇
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣷⣶⣾⣿⣿⣿⣿⣆⠁
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆

*/