#ifndef ROUND_ROBIN_H_
#define ROUND_ROBIN_H_

#include "comunicacion_cpu/comunicacion_cpu.h"
#include "handlers/colas_estados/colas_handler.h"
#include "lista_argumentos_rr.h"
#include <TAD/TAD.h>
#include <commons/temporal.h>
#include <globals/globals.h>
#include <serializacion/serializacion.h>
#include <t_config/t_config.h>
#include <unistd.h>

extern bool send_Kernel_cpu_int_desalojo_call_rr;
extern bool send_Kernel_cpu_execute_call_rr;

extern bool hay_fin_q;

void enviar_proceso_round_robin();

#endif