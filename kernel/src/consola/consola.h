#ifndef CONSOLA_H_
#define CONSOLA_H_

#include "diccionario_instrucciones.h"
#include "finalizacion/blacklist.h"
#include "globals/globals.h"
#include "hilo_genesis/hilo_genesis.h"
#include "multiprogramacion/handlers_multiprogramacion.h"
#include "semaforos_planificadores/semaforos_planificadores.h"
#include <commons/log.h>
#include <commons/string.h>

#include <pthread.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_t HILO_CONOSOLA;

void *consola();
void iniciar_consola();
// void finalizar_consola();

#endif