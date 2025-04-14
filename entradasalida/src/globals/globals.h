#ifndef GLOBALS_IO_H_
#define GLOBALS_IO_H_

#include <commons/config.h>
#include <commons/log.h>

#include "../t_config/t_config.h"

// logger
t_log *get_logger();
void destroy_logger();

// global fd_memoria
int get_fd_memoria();
void set_fd_memoria(int valor);
// global nombre_io
char *get_nombre_io();
void set_nombre_io(char *nombre);
// global rutaMetadata
char *get_rutaMetadata();
void set_rutaMetadata(char *rutaMetadata);
// global rutaBloques
char *get_rutaBloques();
void set_rutaBloques(char *rutaBloques);
// global rutaBitmap
char *get_rutaBitmap();
void set_rutaBitmap(char *rutaBitmap);

// esta cosas estan antes de que yo llegue, funcionan => no las toco
void setup_signal_handler();
void check_args(int argc, char *argv[]);
void mem_clean_exit();

#endif