#include "t_config.h"
#include "globals/globals.h"

static t_config_cpu *config = NULL;

int initialize_cfg(char *path_config)
{
    t_log *logger = get_logger();
    t_config *cfg = config_create(path_config);

    if (cfg == NULL)
    {
        log_error(logger, "No se encontro  el cpu.config\n");
        destroy_logger();
        exit(EXIT_SUCCESS); // NOLINT
    }

    if (
        // agregados
        !config_has_property(cfg, "IP_MODULO") ||
        // fin agregados

        !config_has_property(cfg, "IP_MEMORIA") || !config_has_property(cfg, "PUERTO_MEMORIA") ||
        !config_has_property(cfg, "PUERTO_ESCUCHA_DISPATCH") || !config_has_property(cfg, "PUERTO_ESCUCHA_INTERRUPT") ||
        !config_has_property(cfg, "CANTIDAD_ENTRADAS_TLB") || !config_has_property(cfg, "ALGORITMO_TLB"))
    {
        log_error(logger, "Le faltan propiedades a cpu.config\n");
        destroy_logger();
        exit(EXIT_SUCCESS); // NOLINT
    }

    config = malloc(sizeof(t_config_cpu));

    // agregados
    config->IP_MODULO = strdup(config_get_string_value(cfg, "IP_MODULO"));
    ;
    // fin agregados

    config->IP_MEMORIA = strdup(config_get_string_value(cfg, "IP_MEMORIA"));
    config->PUERTO_MEMORIA = strdup(config_get_string_value(cfg, "PUERTO_MEMORIA"));
    config->PUERTO_ESCUCHA_DISPATCH = strdup(config_get_string_value(cfg, "PUERTO_ESCUCHA_DISPATCH"));
    config->PUERTO_ESCUCHA_INTERRUPT = strdup(config_get_string_value(cfg, "PUERTO_ESCUCHA_INTERRUPT"));
    config->CANTIDAD_ENTRADAS_TLB = config_get_int_value(cfg, "CANTIDAD_ENTRADAS_TLB");
    config->ALGORITMO_TLB = strdup(config_get_string_value(cfg, "ALGORITMO_TLB"));

    log_trace(logger, "Se cargo cpu.config correctamente :)\n");
    config_destroy(cfg);
    return 0;
}

t_config_cpu *get_config()
{
    assert(config); // la config tiene que existir
    return config;
}

void destroy_config_cpu()
{
    // agregados
    free(config->IP_MODULO);
    // fin agregados
    free(config->IP_MEMORIA);
    free(config->PUERTO_MEMORIA);
    free(config->PUERTO_ESCUCHA_DISPATCH);
    free(config->PUERTO_ESCUCHA_INTERRUPT);
    free(config->ALGORITMO_TLB);

    free(config);
    config = NULL;
}