#include "logs.h"

t_log *logger = NULL;
static char *motivo_de_muerte_str(motivo_de_muerte motivo);
static void init_logger();

void log_proceso_creado(int pid)
{
    init_logger();
    log_info(logger, "Se crea el proceso <%i> en NEW", pid);
}

void log_fin_proceso(int pid, motivo_de_muerte *motivo) // NOLINT(bugprone-easily-swappable-parameters)
{
    init_logger();
    char *motivo_str = motivo_de_muerte_str(*motivo);
    log_info(logger, "Finaliza el proceso <%i> - Motivo: %s", pid, motivo_str);
}

void log_cambio_de_estado(int pid, char *estado_anterior, char *estado_actual) // falta revisar bien VRR
{
    init_logger();

// no tengo ni puta idea porque en el test se rompe el logger ....
#ifndef TEST
    log_info(logger, "PID: <%i> - Estado Anterior: <%s> - Estado Actual: <%s>", pid, estado_anterior, estado_actual);
#else
    log_info(get_logger(), "PID: <%i> - Estado Anterior: <%s> - Estado Actual: <%s>", pid, estado_anterior,
             estado_actual);
#endif
}

void log_bloqueo(int pid, char *nombre_io_o_recurso)
{
    init_logger();
    log_info(logger, "PID: <%i> - Bloqueado por: <%s>", pid, nombre_io_o_recurso);
}

void log_fin_de_q(int pid)
{
    init_logger();
    log_info(logger, "PID: <%i> - Desalojado por fin de Quantum", pid);
}

// TODO(all) esto parece que está mal en la consigna.
// void log_ingreso_a_ready(t_list *list)
// {
//     init_logger();
//     log_info(logger, "Cola Ready <COLA>: [<LISTA DE PIDS>]");
// }

static void init_logger()
{
    if (logger == NULL)
    {
        logger = get_logger();
    }
}

static char *motivo_de_muerte_str(motivo_de_muerte motivo)
{
    char *motivo_str;
    switch (motivo)
    {
    case SUCCESS: {
        motivo_str = "SUCCESS";
        break;
    }
    case INVALID_RESOURCE: {
        motivo_str = "INVALID_RESOURCE";
        break;
    }

    case INVALID_INTERFACE: {
        motivo_str = "INVALID_INTERFACE";
        break;
    }

    case OUT_OF_MEMORY: {
        motivo_str = "OUT_OF_MEMORY";
        break;
    }

    case INTERRUPTED_BY_USER: {
        motivo_str = "INTERRUPTED_BY_USER";
        break;
    }

    default: {
        motivo_str = "UNKNOWN";
        break;
    }
    }
    return motivo_str;
}