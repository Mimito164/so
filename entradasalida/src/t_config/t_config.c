#include "t_config.h"
#include "globals/globals.h"

static t_config_io *config = NULL;

int initialize_cfg(char *path_config)
{
    t_log *logger = get_logger();
    t_config *cfg = config_create(path_config);
    config = malloc(sizeof(t_config_io));

    if (cfg == NULL)
    {
        log_error(logger, "No se encontro  el entradasalida.config\n");
        mem_clean_exit();
    }
    if (!config_has_property(cfg, "TIPO_INTERFAZ"))
    {
        log_error(logger, "Le falta el tipo de interfaz a entradasalida.config\n");
        mem_clean_exit();
    }
    char *auxiliar = strdup(config_get_string_value(cfg, "TIPO_INTERFAZ"));

    if (strcmp(auxiliar, "DIALFS") == 0)
    {
        config->TIPO_INTERFAZ = DialFS;
    }
    else if (strcmp(auxiliar, "STDOUT") == 0)
    {
        config->TIPO_INTERFAZ = STDOUT;
    }
    else if (strcmp(auxiliar, "STDIN") == 0)
    {
        config->TIPO_INTERFAZ = STDIN;
    }
    else if (strcmp(auxiliar, "GENERICA") == 0)
    {
        config->TIPO_INTERFAZ = GENERICA;
    }
    else
    {
        log_error(logger, "El tipo de interfaz del entradasalida.config no es valido\n");
        mem_clean_exit();
    }

    free(auxiliar);

    switch (config->TIPO_INTERFAZ)
    {
    case DialFS:
        if (!config_has_property(cfg, "TIEMPO_UNIDAD_TRABAJO") || !config_has_property(cfg, "IP_KERNEL") ||
            !config_has_property(cfg, "PUERTO_KERNEL") || !config_has_property(cfg, "IP_MEMORIA") ||
            !config_has_property(cfg, "PUERTO_MEMORIA") || !config_has_property(cfg, "PATH_BASE_DIALFS") ||
            !config_has_property(cfg, "BLOCK_SIZE") || !config_has_property(cfg, "BLOCK_COUNT") ||
            !config_has_property(cfg, "RETRASO_COMPACTACION"))
        {
            log_error(logger, "Le faltan propiedades a entradasalida.config\n");
            mem_clean_exit();
        }

        config->TIEMPO_UNIDAD_TRABAJO = config_get_int_value(cfg, "TIEMPO_UNIDAD_TRABAJO");
        config->IP_KERNEL = strdup(config_get_string_value(cfg, "IP_KERNEL"));
        config->PUERTO_KERNEL = strdup(config_get_string_value(cfg, "PUERTO_KERNEL"));
        config->IP_MEMORIA = strdup(config_get_string_value(cfg, "IP_MEMORIA"));
        config->PUERTO_MEMORIA = strdup(config_get_string_value(cfg, "PUERTO_MEMORIA"));
        config->PATH_BASE_DIALFS = strdup(config_get_string_value(cfg, "PATH_BASE_DIALFS"));
        config->BLOCK_SIZE = config_get_int_value(cfg, "BLOCK_SIZE");
        config->BLOCK_COUNT = config_get_int_value(cfg, "BLOCK_COUNT");
        config->RETRASO_COMPACTACION = config_get_int_value(cfg, "RETRASO_COMPACTACION");
        break;

    case STDOUT:
    case STDIN:
        if (!config_has_property(cfg, "TIEMPO_UNIDAD_TRABAJO") || !config_has_property(cfg, "IP_KERNEL") ||
            !config_has_property(cfg, "PUERTO_KERNEL") || !config_has_property(cfg, "IP_MEMORIA") ||
            !config_has_property(cfg, "PUERTO_MEMORIA"))
        {
            log_error(logger, "Le faltan propiedades a entradasalida.config\n");
            mem_clean_exit();
        }

        config->TIEMPO_UNIDAD_TRABAJO = config_get_int_value(cfg, "TIEMPO_UNIDAD_TRABAJO");
        config->IP_KERNEL = strdup(config_get_string_value(cfg, "IP_KERNEL"));
        config->PUERTO_KERNEL = strdup(config_get_string_value(cfg, "PUERTO_KERNEL"));
        config->IP_MEMORIA = strdup(config_get_string_value(cfg, "IP_MEMORIA"));
        config->PUERTO_MEMORIA = strdup(config_get_string_value(cfg, "PUERTO_MEMORIA"));
        break;

    case GENERICA:
        if (!config_has_property(cfg, "TIEMPO_UNIDAD_TRABAJO") || !config_has_property(cfg, "IP_KERNEL") ||
            !config_has_property(cfg, "PUERTO_KERNEL"))
        {
            log_error(logger, "Le faltan propiedades a entradasalida.config\n");
            mem_clean_exit();
        }

        config->TIEMPO_UNIDAD_TRABAJO = config_get_int_value(cfg, "TIEMPO_UNIDAD_TRABAJO");
        config->IP_KERNEL = strdup(config_get_string_value(cfg, "IP_KERNEL"));
        config->PUERTO_KERNEL = strdup(config_get_string_value(cfg, "PUERTO_KERNEL"));
        break;

    default:
        break;
    }

    log_trace(logger, "Se cargo entradasalida.config correctamente :)\n");

    config_destroy(cfg);
    return 0;
}

t_config_io *get_config()
{
    return config;
}

void destroy_config_io()
{
    if (config == NULL)
    {
        return;
    }

    free(config->IP_KERNEL);
    free(config->PUERTO_KERNEL);
    if (config->TIPO_INTERFAZ == DialFS)
    {
        free(config->IP_MEMORIA);
        free(config->PUERTO_MEMORIA);
        free(config->PATH_BASE_DIALFS);
    }
    else if (config->TIPO_INTERFAZ == STDOUT || config->TIPO_INTERFAZ == STDIN)
    {
        free(config->IP_MEMORIA);
        free(config->PUERTO_MEMORIA);
    }

    free(config);
    config = NULL;
}