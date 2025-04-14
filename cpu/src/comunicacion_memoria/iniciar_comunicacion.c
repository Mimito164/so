#include "iniciar_comunicacion.h"
#include "globals/globals.h"

static int fd_server_memoria;

void iniciar_comunicacion_memoria()
{
    t_log *logger = get_logger();
    t_config_cpu *cfg = get_config();
    if (!generar_conexion(logger, "servidor memoria", &fd_server_memoria, cfg->IP_MEMORIA, cfg->PUERTO_MEMORIA))
    {
        exit(1); // TODO crear funcion para liberar datos de todo.
    }
}

int get_fd_memoria()
{
    return fd_server_memoria;
}
