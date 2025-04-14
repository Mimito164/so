#ifndef T_CONFIG_H_
#define T_CONFIG_H_

#include <stdlib.h>
#include <string.h>

#include <commons/config.h>
#include <commons/log.h>

typedef struct
{
    // agregados
    char *IP_MODULO;
    // fin agregados

    char *PUERTO_ESCUCHA;
    int TAM_MEMORIA;
    int TAM_PAGINA;
    char *PATH_INSTRUCCIONES;
    int RETARDO_RESPUESTA;
} t_config_memoria;

int initialize_cfg(char *path_config);
t_config_memoria *get_config();
void destroy_config_memoria();

// SOLO PARA TESTS
void set_tam_pag(int tamanio);

#endif