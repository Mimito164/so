#ifndef COMUNICACION_IO_STDIN_H_
#define COMUNICACION_IO_STDIN_H_

#include "../io_share.h"
#include "globals/globals.h"
#include "planificador_corto/planificador_corto.h"
#include <pthread.h>
#include <serializacion/mensajes/kernel_io/msj_kernel_io.h>
#include <sockets/sockets.h>

/// @brief HACER UNA DESCRIPCIÓN
/// @param solicitud
/// @param recurso
void handle_io_stdin_read(t_solicitud *solicitud, t_recurso *recurso);

typedef struct
{
    s_kernel_io_stdin_read msj;
    int fd_IO;
} s_send_kernel_io_stdin_read_args;

#endif