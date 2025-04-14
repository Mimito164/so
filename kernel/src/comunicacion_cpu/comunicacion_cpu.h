#ifndef COMUNICACION_CPU_H_
#define COMUNICACION_CPU_H_

#include "assert.h"
#include "finalizacion/blacklist.h"
#include "globals/globals.h"
#include "handlers/colas_estados/colas_handler.h"
#include "handlers/pcb_handler/pcb_handler.h"
#include "planificador_corto/planificador_corto.h"
#include "pthread.h"
#include "serializacion/mensajes/cpu_kernel/msj_cpu_kernel.h"
#include "serializacion/mensajes/kernel_cpu/mensajes_kernel_cpu.h"
#include <gestor_recursos/gestor_recursos_share.h>
#include <sockets/sockets.h>

void iniciar_comunicacion_cpu();
void send_dispatch(t_contexto_ejecucion contexto);
void send_interrupt(uint32_t pid, op_code codigo);

#endif