#ifndef V_ROUND_ROBIN_H_
#define V_ROUND_ROBIN_H_

#include "comunicacion_cpu/comunicacion_cpu.h"
#include "handlers/colas_estados/colas_handler.h"
#include <TAD/TAD.h>
#include <commons/temporal.h>
#include <globals/globals.h>
#include <inttypes.h>
#include <serializacion/serializacion.h>
#include <t_config/t_config.h>
#include <unistd.h>

extern bool send_Kernel_cpu_int_desalojo_call_vrr;
extern bool send_Kernel_cpu_execute_call_vrr;

void enviar_proceso_v_round_robin();
void exec_a_ready_vrr();

void update_pcb_q(t_pcb *pcb);

#endif