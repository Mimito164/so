#ifndef LISTA_ARGUMENTOS_RR_H_
#define LISTA_ARGUMENTOS_RR_H_

#include "globals/globals.h"
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

#include <commons/collections/list.h>
typedef struct
{
    pthread_mutex_t args_mutex;
    uint32_t pid;
    bool is_active;
    bool ended;
} quantum_thread_args;

void initialize_quantum_args_list();
void destroy_quantum_args_list();
void add_argument(void *argument);
void disable_last_quantum_args();

#endif