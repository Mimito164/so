#include "iniciar_comunicacion.h"

static int fd_memoria;

void atender_cliente(void *args)
{
    t_log *logger = get_logger();
    // log_debug(logger, "soy un nevo cliente");
}

void iniciar_comunicacion_memoria()
{
    t_log *logger = get_logger();
    t_config_io *cfg = get_config();
    if (cfg->TIPO_INTERFAZ == GENERICA)
        return;
    if (!generar_conexion(logger, "Server Memoria", &fd_memoria, cfg->IP_MEMORIA, cfg->PUERTO_MEMORIA))
    {
        mem_clean_exit();
    }
    set_fd_memoria(fd_memoria);
}
