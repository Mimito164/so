#ifndef GLOBALS_MEMORIA_H_
#define GLOBALS_MEMORIA_H_

#include <commons/config.h>
#include <commons/log.h>

#include "../t_config/t_config.h"

t_log *get_logger();
void destroy_logger();

void setup_signal_handler();
void check_args(int argc, char *argv[]);
void mem_clean_exit();

#endif