#ifndef MENSAJES_KERNEL_CPU
#define MENSAJES_KERNEL_CPU

#include "../../kernel/src/globals/globals.h"
#include <serializacion/serializacion.h>

#include <TAD/TAD.h>
#include <serializacion/mensajes/cpu_kernel/msj_cpu_kernel.h>

t_paquete *serializar_kernel_cpu_interrupt(uint32_t pid, op_code codigo);
t_paquete *serializar_kernel_cpu_execute(t_contexto_ejecucion contexto);

#endif
