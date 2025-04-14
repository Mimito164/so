#include "pcb_handler.h"

uint32_t static next_pid = 0;

t_pcb *create_pcb()
{
    t_pcb *new_pcb = malloc(sizeof(t_pcb));
    new_pcb->pid = next_pid++;
    t_config_kernel *config = get_config();
    t_registros_cpu *registros = malloc(sizeof(t_registros_cpu));
    new_pcb->registros_cpu = registros;
    new_pcb->quantum = config->QUANTUM;
    new_pcb->registros_cpu->PC = 0;
    new_pcb->registros_cpu->AX = 0;
    new_pcb->registros_cpu->BX = 0;
    new_pcb->registros_cpu->CX = 0;
    new_pcb->registros_cpu->DX = 0;
    new_pcb->registros_cpu->EAX = 0;
    new_pcb->registros_cpu->EBX = 0;
    new_pcb->registros_cpu->ECX = 0;
    new_pcb->registros_cpu->EDX = 0;
    new_pcb->registros_cpu->SI = 0;
    new_pcb->registros_cpu->DI = 0;

    t_log *logger = get_logger();
    log_trace(logger, "Se creo el pcb con pid %d\n", new_pcb->pid);

    return new_pcb;
}

void destroy_pcb(t_pcb *pcb_to_destroy)
{
    t_log *logger = get_logger();
    log_trace(logger, "Se elimina el pcb con pid %d\n", pcb_to_destroy->pid);
    free(pcb_to_destroy->registros_cpu);
    free(pcb_to_destroy);
    pcb_to_destroy = NULL;
}

t_contexto_ejecucion create_contexto(t_pcb *temp)
{
    t_contexto_ejecucion contexto;
    t_registros_cpu registros;
    registros.PC = temp->registros_cpu->PC;
    registros.AX = temp->registros_cpu->AX;
    registros.BX = temp->registros_cpu->BX;
    registros.CX = temp->registros_cpu->CX;
    registros.DX = temp->registros_cpu->DX;
    registros.EAX = temp->registros_cpu->EAX;
    registros.EBX = temp->registros_cpu->EBX;
    registros.ECX = temp->registros_cpu->ECX;
    registros.EDX = temp->registros_cpu->EDX;
    registros.SI = temp->registros_cpu->SI;
    registros.DI = temp->registros_cpu->DI;
    contexto.registros_cpu = registros;
    contexto.pid = temp->pid;
    return contexto;
}

int update_pcb(t_pcb *pcb_to_update, t_contexto_ejecucion context)
{
    if (pcb_to_update->pid == context.pid)
    {
        t_log *logger = get_logger();
        log_trace(logger, "Se hace update del pcb con pid %d\n", pcb_to_update->pid);
        pcb_to_update->registros_cpu->PC = context.registros_cpu.PC;
        pcb_to_update->registros_cpu->AX = context.registros_cpu.AX;
        pcb_to_update->registros_cpu->BX = context.registros_cpu.BX;
        pcb_to_update->registros_cpu->CX = context.registros_cpu.CX;
        pcb_to_update->registros_cpu->DX = context.registros_cpu.DX;
        pcb_to_update->registros_cpu->EAX = context.registros_cpu.EAX;
        pcb_to_update->registros_cpu->EBX = context.registros_cpu.EBX;
        pcb_to_update->registros_cpu->ECX = context.registros_cpu.ECX;
        pcb_to_update->registros_cpu->EDX = context.registros_cpu.EDX;
        pcb_to_update->registros_cpu->SI = context.registros_cpu.SI;
        pcb_to_update->registros_cpu->DI = context.registros_cpu.DI;
        return 0;
    }
    t_log *logger = get_logger();
    log_error(logger, "Fallo de update del pcb con pid %d, se paso el pid %d\n", pcb_to_update->pid, context.pid);
    return 1;
}

// esto es para los tests
void gh_lpm()
{
    next_pid = 0;
}