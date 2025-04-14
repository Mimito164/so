#ifndef LOGS_H_
#define LOGS_H_

#include <globals/globals.h>

void log_proceso_creado(int pid);

typedef enum
{
    SUCCESS,
    INVALID_RESOURCE,
    INVALID_INTERFACE, // asumo que esto hace referencia a i/o
    OUT_OF_MEMORY,
    INTERRUPTED_BY_USER
} motivo_de_muerte;

void log_fin_proceso(int pid, motivo_de_muerte *motivo);

void log_cambio_de_estado(int pid, char *estado_anterior, char *estado_actual);

void log_bloqueo(int pid, char *nombre_io_o_recurso);

void log_fin_de_q(int pid);

#endif