#include "planificador_corto.h"

static void blocked_a_ready(int PID);
static void ready_a_exec();
static void exec_a_ready();
static void exec_a_blocked();

/// @brief Mueve procesos entre las listas de blocked, ready y exec
/// @attention las transiciones EXEC_A_... se encargan de mover luego algo a exec
/// @param args.transicion algúna de las siguientes: BLOCKED_A_READY | READY_A_EXEC | EXEC_A_READY | EXEC_A_BLOCKED
/// @param args.PID es Opcional, necesario solamente para la transición BLOCKED_A_READY
void planificador_corto(s_planificador_corto_args args)
{
    sem_wait(&semaforo_planificador_corto);
    t_transicion transicion = args.transicion;

    t_log *logger = get_logger();

    switch (transicion)
    {
    case BLOCKED_A_READY:
        log_trace(logger, "planificador corto: Blocked → Ready");
        int PID = args.op_pid.PID;
        assert(PID > -1);
        blocked_a_ready(PID);
        ready_a_exec();
        break;

    case READY_A_EXEC:
        log_trace(logger, "planificador corto: Ready → Exec");
        ready_a_exec();
        break;
    case EXEC_A_READY:
        log_trace(logger, "planificador corto: Exec → Ready");
        exec_a_ready();
        ready_a_exec(); // Una vez que quitamos un proceso de ejecución hay que mandar uno nuevo
        break;
    case EXEC_A_BLOCKED:
        log_trace(logger, "planificador corto: Exec → Blocked");
        // contexto = deserializar_contexto()
        exec_a_blocked();
        ready_a_exec(); // Una vez que quitamos un proceso de ejecución hay que mandar uno nuevo
        break;

    default:
        assert(!"planificador_corto() recibió una transición invalida");

        log_error(logger, "planificador_corto() recibió una transición invalida");
        break;
    }
    sem_post(&semaforo_planificador_corto);
}

static void blocked_a_ready(int PID)
{
    t_config_kernel *config = get_config();
#ifndef TEST
    bool dead = check_blacklist(BLOCKED_A_READY, PID, NULL);
    if (dead)
    {
        return;
    }
#endif
    t_pcb *pcb = find_pcb_by_pid("BLOCKED", (uint32_t)PID);

    if (!pcb)
    {
        return;
    }

    if (pcb->quantum < config->QUANTUM)
    {
        add_pcb("PREADY", pcb);
        log_cambio_de_estado(PID, "BLOCKED", "PREADY");
    }
    else
    {
        add_pcb("READY", pcb);
        log_cambio_de_estado(PID, "BLOCKED", "READY");
    }
}

static void ready_a_exec()
{
    if (get_executing())
    {
        return;
    }
    t_config_kernel *cfg = get_config();
    if (!strcmp(cfg->ALGORITMO_PLANIFICACION, "FIFO"))
    {
        enviar_proceso_fifo();
    }
    if (!strcmp(cfg->ALGORITMO_PLANIFICACION, "RR"))
    {
        enviar_proceso_round_robin();
    }
    if (!strcmp(cfg->ALGORITMO_PLANIFICACION, "VRR"))
    {
        enviar_proceso_v_round_robin();
    }
}

static void exec_a_ready()
{

    t_pcb *pcb_executing = get_executing();

#ifndef TEST
    bool dead = check_blacklist(EXEC_A_READY, pcb_executing->pid, NULL);
    if (dead)
    {
        return;
    }
#endif
    set_executing(NULL);

    update_pcb_q(pcb_executing); // VRR

    if (pcb_executing->quantum < (get_config())->QUANTUM)
    {
        add_pcb("PREADY", pcb_executing);
        log_cambio_de_estado((int)pcb_executing->pid, "EXEC", "PREADY");
    }
    else
    {
        add_pcb("READY", pcb_executing);
        log_cambio_de_estado((int)pcb_executing->pid, "EXEC", "READY");
    }
}

void exec_a_blocked()
{
    t_pcb *pcb_executing = get_executing();
#ifndef TEST
    bool dead = check_blacklist(EXEC_A_BLOCKED, pcb_executing->pid, NULL);
    if (dead)
    {
        return;
    }
#endif
    set_executing(NULL);
    update_pcb_q(pcb_executing); // VRR
    add_pcb("BLOCKED", pcb_executing);
    log_cambio_de_estado((int)pcb_executing->pid, "EXEC", "BLOCKED");
}
