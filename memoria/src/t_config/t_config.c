#include "t_config.h"
#include "globals/globals.h"

static t_config_memoria *config = NULL;

int initialize_cfg(char *path_config)
{
    t_log *logger = get_logger();
    t_config *cfg = config_create(path_config);

    if (cfg == NULL)
    {
        log_error(logger, "No se encontro  el memoria.config\n");
        destroy_logger();
        exit(EXIT_SUCCESS); // NOLINT
    }

    if (
        // agregados
        !config_has_property(cfg, "IP_MODULO") ||
        // fin agregados
        !config_has_property(cfg, "PUERTO_ESCUCHA") || !config_has_property(cfg, "TAM_MEMORIA") ||
        !config_has_property(cfg, "TAM_PAGINA") || !config_has_property(cfg, "PATH_INSTRUCCIONES") ||
        !config_has_property(cfg, "RETARDO_RESPUESTA"))
    {
        log_error(logger, "Le faltan propiedades a memoria.config\n");
        destroy_logger();
        exit(EXIT_SUCCESS); // NOLINT
    }

    config = malloc(sizeof(t_config_memoria));

    // agregados
    config->IP_MODULO = strdup(config_get_string_value(cfg, "IP_MODULO"));
    // fin agregados

    config->PUERTO_ESCUCHA = strdup(config_get_string_value(cfg, "PUERTO_ESCUCHA"));
    config->TAM_MEMORIA = config_get_int_value(cfg, "TAM_MEMORIA");
    config->TAM_PAGINA = config_get_int_value(cfg, "TAM_PAGINA");
    config->PATH_INSTRUCCIONES = strdup(config_get_string_value(cfg, "PATH_INSTRUCCIONES"));
    config->RETARDO_RESPUESTA = config_get_int_value(cfg, "RETARDO_RESPUESTA");

    log_trace(logger, "Se cargo memoria.config correctamente :)\n");
    config_destroy(cfg);
    return 0;
}

t_config_memoria *get_config()
{
    return config;
}

void destroy_config_memoria()
{
    // clean char*
    free(config->IP_MODULO);
    free(config->PUERTO_ESCUCHA);
    free(config->PATH_INSTRUCCIONES);
    // clean config pointer
    free(config);
    config = NULL;
    return;
}

void set_tam_pag(int tamanio)
{
    config->TAM_PAGINA = 4;
}