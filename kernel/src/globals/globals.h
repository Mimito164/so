#ifndef GLOBALS_KERNEL_H_
#define GLOBALS_KERNEL_H_

#include <assert.h>
#include <commons/config.h>
#include <commons/log.h>
#include <semaphore.h>

#include "../t_config/t_config.h"

extern sem_t semaforo_consola;

extern sem_t executing_mutex;

t_log *get_logger();
void destroy_logger();

void setup_signal_handler();
void check_args(int argc, char *argv[]);
void mem_clean_exit();

#endif