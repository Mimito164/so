#ifndef INICIAR_COMUNICACION_KERNEL_H_
#define INICIAR_COMUNICACION_KERNEL_H_

#include <sockets/sockets.h>
#include <pthread.h>
#include "globals/globals.h"
#include <serializacion/serializacion.h>

#include "hiloInterrupt/hilo_interrupt.h"
#include "hiloDispatch/hilo_dispatch.h"



void iniciar_servidor_cpu();

#endif