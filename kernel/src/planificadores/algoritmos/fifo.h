#ifndef FIFO_H_
#define FIFO_H_

#include "comunicacion_cpu/comunicacion_cpu.h"
#include "handlers/colas_estados/colas_handler.h"
#include "logs/logs.h"
#include <TAD/TAD.h>
#include <globals/globals.h>
#include <serializacion/serializacion.h>
#include <t_config/t_config.h>
#include <unistd.h>

extern bool send_Kernel_cpu_execute_call_fifo;

void enviar_proceso_fifo();

#endif