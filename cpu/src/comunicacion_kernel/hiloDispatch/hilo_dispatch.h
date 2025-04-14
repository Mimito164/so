#ifndef HILO_DISPATH_H_
#define HILO_DISPATH_H_

#include "assert.h"
#include "comunicacion_kernel//contextoEjecucionHandlers/contextoEjecucionHandlers.h"
#include "comunicacion_kernel/mensajes/mensajes.h"
#include "globals/globals.h"
#include <ciclo_instruccion/ciclo_instruccion.h>
#include <serializacion/mensajes/cpu_kernel/msj_cpu_kernel.h>

typedef struct
{
    int *fd;
} client_thread_args_dipatch;

void atender_cliente_dispatch(client_thread_args_dipatch *args);

#endif