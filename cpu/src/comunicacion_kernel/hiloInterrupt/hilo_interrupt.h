#ifndef HILO_INTERRUPT_H_
#define HILO_INTERRUPT_H_

#include "assert.h"
#include "comunicacion_kernel/interruptHandlers/interrupt_handlers.h"
#include "comunicacion_kernel/mensajes/mensajes.h"
#include "globals/globals.h"
#include <serializacion/serializacion.h>
#include <unistd.h>

typedef struct
{
    int *fd;
} client_thread_args_interrupt;

void atender_cliente_interrupt(client_thread_args_interrupt *args);

#endif