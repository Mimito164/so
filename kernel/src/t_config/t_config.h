#ifndef T_CONFIG_H_
#define T_CONFIG_H_

#include <commons/config.h>
#include <commons/log.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct
{
    // agregados
    char *IP_MODULO;
    // fin agregados

    char *PUERTO_ESCUCHA;
    char *IP_MEMORIA;
    char *PUERTO_MEMORIA;
    char *IP_CPU;
    char *PUERTO_CPU_DISPATCH;
    char *PUERTO_CPU_INTERRUPT;
    char *ALGORITMO_PLANIFICACION;
    int QUANTUM;
    char *RECURSOS;
    char *INSTANCIAS_RECURSOS;
    int GRADO_MULTIPROGRAMACION;
} t_config_kernel;

bool initialize_cfg(char *path_config);
t_config_kernel *get_config();
void destroy_config_kernel();

#endif