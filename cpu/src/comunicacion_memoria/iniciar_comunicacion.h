#ifndef COMUNICACION_MEMORIA_H_
#define COMUNICACION_MEMORIA_H_

#include "globals/globals.h"
#include <pthread.h>
#include <sockets/sockets.h>

void iniciar_comunicacion_memoria();
int get_fd_memoria();

#endif