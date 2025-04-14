#ifndef COMUNICACION_ENTRADASALIDA_H_
#define COMUNICACION_ENTRADASALIDA_H_

#include "globals/globals.h"
#include <pthread.h>
#include <serializacion/mensajes/kernel_io/msj_kernel_io.h>
#include <sockets/sockets.h>

void iniciar_comunicacion_io();

#endif