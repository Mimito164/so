#ifndef INICIAR_COMUNICACION_KERNEL_H_
#define INICIAR_COMUNICACION_KERNEL_H_

#include "comunicacion_kernel/resolver.h"
#include "globals/globals.h"
#include <pthread.h>
#include <serializacion/mensajes/io_kernel/msj_io_kernel.h>
#include <serializacion/serializacion.h>
#include <sockets/sockets.h>

void iniciar_comunicacion_kernel();

#endif