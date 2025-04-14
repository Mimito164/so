#include "t_config.h"
#include "globals/globals.h"

static t_config_kernel *config = NULL;

bool initialize_cfg(char *path_config)
{
    t_log *logger = get_logger();
    t_config *cfg = config_create(path_config);

    if (cfg == NULL)
    {
        log_error(logger, "No se encontro el kernel.config\n");
        destroy_logger();
        exit(EXIT_SUCCESS); // NOLINT
    }

    if (
        // agregados
        !config_has_property(cfg, "IP_MODULO") ||
        // fin agregados

        !config_has_property(cfg, "PUERTO_ESCUCHA") || !config_has_property(cfg, "IP_MEMORIA") ||
        !config_has_property(cfg, "PUERTO_MEMORIA") || !config_has_property(cfg, "IP_CPU") ||
        !config_has_property(cfg, "PUERTO_CPU_DISPATCH") || !config_has_property(cfg, "PUERTO_CPU_INTERRUPT") ||
        !config_has_property(cfg, "ALGORITMO_PLANIFICACION") || !config_has_property(cfg, "QUANTUM") ||
        !config_has_property(cfg, "RECURSOS") || !config_has_property(cfg, "INSTANCIAS_RECURSOS") ||
        !config_has_property(cfg, "GRADO_MULTIPROGRAMACION"))
    {
        log_error(logger, "Le faltan propiedades a kernel.config\n");
        destroy_logger();
        exit(EXIT_SUCCESS); // NOLINT
    }

    config = malloc(sizeof(t_config_kernel));
    // agregados
    config->IP_MODULO = strdup(config_get_string_value(cfg, "IP_MODULO"));
    // fin agregados

    config->PUERTO_ESCUCHA = strdup(config_get_string_value(cfg, "PUERTO_ESCUCHA"));
    config->IP_MEMORIA = strdup(config_get_string_value(cfg, "IP_MEMORIA"));
    config->PUERTO_MEMORIA = strdup(config_get_string_value(cfg, "PUERTO_MEMORIA"));
    config->IP_CPU = strdup(config_get_string_value(cfg, "IP_CPU"));
    config->PUERTO_CPU_DISPATCH = strdup(config_get_string_value(cfg, "PUERTO_CPU_DISPATCH"));
    config->PUERTO_CPU_INTERRUPT = strdup(config_get_string_value(cfg, "PUERTO_CPU_INTERRUPT"));
    config->ALGORITMO_PLANIFICACION = strdup(config_get_string_value(cfg, "ALGORITMO_PLANIFICACION"));
    config->QUANTUM = config_get_int_value(cfg, "QUANTUM");
    config->RECURSOS = strdup(config_get_string_value(cfg, "RECURSOS"));
    config->INSTANCIAS_RECURSOS = strdup(config_get_string_value(cfg, "INSTANCIAS_RECURSOS"));
    config->GRADO_MULTIPROGRAMACION = config_get_int_value(cfg, "GRADO_MULTIPROGRAMACION");

    log_trace(logger, "Se cargo kernel.config correctamente :)\n");

    config_destroy(cfg);

    return true;
}

t_config_kernel *get_config()
{
    return config;
}

void destroy_config_kernel()
{
    // agregados
    free(config->IP_MODULO);
    // fin agregados

    free(config->PUERTO_ESCUCHA);
    free(config->IP_MEMORIA);
    free(config->PUERTO_MEMORIA);
    free(config->IP_CPU);
    free(config->PUERTO_CPU_DISPATCH);
    free(config->PUERTO_CPU_INTERRUPT);
    free(config->ALGORITMO_PLANIFICACION);
    free(config->RECURSOS);
    free(config->INSTANCIAS_RECURSOS);

    free(config);
}