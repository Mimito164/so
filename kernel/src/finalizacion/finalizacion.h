#ifndef FINALIZACION_H_
#define FINALIZACION_H_

#include "admision/admision.h"
#include "blacklist.h"
#include "commons/collections/list.h"
#include "comunicacion_cpu/comunicacion_cpu.h"
#include "comunicacion_memoria/send.h"
#include "gestor_recursos/gestor_recursos_share.h"
#include "globals/globals.h"
#include "handlers/colas_estados/colas_handler.h"
#include "handlers/pcb_handler/pcb_handler.h"
#include "logs/logs.h"
#include "transiciones/transiciones.h"
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

bool finalizacion(t_transicion transicion, uint32_t pid, t_pcb *pcb, motivo_de_muerte *motivo);

#endif