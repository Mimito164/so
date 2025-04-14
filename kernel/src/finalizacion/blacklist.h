#ifndef BLACKLIST_H_
#define BLACKLIST_H_

#include "commons/collections/dictionary.h"
#include "commons/string.h"
#include "finalizacion.h"
#include "handlers/pcb_handler/pcb_handler.h"
#include "logs/logs.h"
#include "transiciones/transiciones.h"
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

void blackilst_process(uint32_t pid, motivo_de_muerte motivo);
bool check_blacklist(t_transicion transicion, uint32_t pid, t_pcb *pcb);
void find_and_kill(uint32_t pid);
void destroy_blacklist();

// testing
typedef struct
{
    t_dictionary **blacklist;
    pthread_mutex_t blacklist_mutex;
    bool initialized;
    void (*init_blacklist)(void);
    void *(*add_to_blacklist)(int *);
    void (*remove_from_blacklist)(uint32_t);
    bool (*find_in_blacklist)(uint32_t);
    bool (*find_and_kill_per_list)(t_transicion, uint32_t);
} statics_t;

statics_t *get_statics();

#endif