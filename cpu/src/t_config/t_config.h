#ifndef T_CONFIG_H_
#define T_CONFIG_H_

#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <commons/config.h>
#include <commons/log.h>

typedef struct
{
    // agregados
    char *IP_MODULO;
    // fin agregados

    char *IP_MEMORIA;
    char *PUERTO_MEMORIA;
    char *PUERTO_ESCUCHA_DISPATCH;
    char *PUERTO_ESCUCHA_INTERRUPT;
    int CANTIDAD_ENTRADAS_TLB;
    char *ALGORITMO_TLB;
} t_config_cpu;

int initialize_cfg(char *path_config);
t_config_cpu *get_config();
void destroy_config_cpu();

#endif