#include "mensajes_kernel_cpu.h"

t_paquete *serializar_kernel_cpu_interrupt(uint32_t pid, op_code codigo) // NOLINT(bugprone-easily-swappable-parameters)
{
    t_paquete *paquete_cpu = paquete_new();
    paquete_cpu->codigo_operacion = codigo;
    serializar_uint32_t(&paquete_cpu->buffer, pid);

    return paquete_cpu;
}

t_paquete *serializar_kernel_cpu_execute(t_contexto_ejecucion contexto)
{
    t_paquete *paquete_cpu = paquete_new();
    paquete_cpu->codigo_operacion = KERNEL_CPU_EXECUTE;

    serializar_contexto_ejecucion(&paquete_cpu->buffer, contexto);

    return paquete_cpu;
}