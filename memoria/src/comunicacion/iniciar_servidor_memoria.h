#ifndef INICIAR_SERVIDOR_MEMORIA_H_
#define INICIAR_SERVIDOR_MEMORIA_H_

#include "globals/globals.h"
#include "mensajes/mensajes_cpu.h"
#include "mensajes/mensajes_io.h"
#include "user_mem/user_mem.h"
#include <estructuras_administrativas/tabla_instrucciones.h>
#include <estructuras_administrativas/tabla_paginas.h>
#include <instrucciones/cargar_instrucciones.h>
#include <instrucciones/resize.h>
#include <pthread.h>
#include <serializacion/mensajes/cpu_memoria/msj_cpu_memoria.h>
#include <serializacion/mensajes/io_memoria/mensajes_io_memoria.h>
#include <serializacion/mensajes/kernel_memoria/mensajes_kernel_memoria.h>
#include <serializacion/mensajes/memoria_cpu/msj_memoria_cpu.h>
#include <serializacion/serializacion.h>
#include <sockets/sockets.h>

typedef struct
{
    int *fd;
} client_thread_args;

void iniciar_servidor_memoria();

#endif