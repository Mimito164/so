#ifndef PLANIFICADOR_CORTO_H_
#define PLANIFICADOR_CORTO_H_

#include "assert.h"
#include "comunicacion_cpu/comunicacion_cpu.h" // TODO(all): resolver dependencia circular
#include "globals/globals.h"
#include "planificadores/algoritmos/fifo.h"
#include "planificadores/algoritmos/round_robin/round_robin.h"
#include "planificadores/algoritmos/v_round_robin.h"
#include "semaforos_planificadores/semaforos_planificadores.h"
#include "transiciones/transiciones.h"
#include <TAD/TAD.h>
// #include "planificadores/algoritmos/virtual_round_robin.h"

typedef struct
{
    t_transicion transicion;
    union {
        int PID;
    } op_pid;
} s_planificador_corto_args;

void planificador_corto(s_planificador_corto_args args);

#endif
