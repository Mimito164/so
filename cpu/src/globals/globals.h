#ifndef GLOBALS_CPU_H_
#define GLOBALS_CPU_H_

#include <commons/config.h>
#include <commons/log.h>
#include <stdint.h>

#include "../t_config/t_config.h"

t_log *get_logger();
void destroy_logger();

void setup_signal_handler();
void check_args(int argc, char *argv[]);
void mem_clean_exit();

uint32_t get_tam_pag();
void set_tam_pag(uint32_t tam);

#endif