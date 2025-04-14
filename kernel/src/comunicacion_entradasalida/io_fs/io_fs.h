#ifndef COMUNICACION_IO_FS_H_
#define COMUNICACION_IO_FS_H_

#include "../io_share.h"
#include "globals/globals.h"
#include "planificador_corto/planificador_corto.h"
#include <pthread.h>
#include <serializacion/mensajes/kernel_io/msj_kernel_io.h>
#include <sockets/sockets.h>

/// @brief Envía la solicitud a la io (el recurso) y luego espera y maneja la respuesta.
/// @param solicitud
/// @param recurso
void handle_io_fs_create(t_solicitud *solicitud, t_recurso *recurso);

/// @brief Envía la solicitud a la io (el recurso) y luego espera y maneja la respuesta.
/// @param solicitud
/// @param recurso
void handle_io_fs_delete(t_solicitud *solicitud, t_recurso *recurso);

/// @brief Envía la solicitud a la io (el recurso) y luego espera y maneja la respuesta.
/// @param solicitud
/// @param recurso
void handle_io_fs_read(t_solicitud *solicitud, t_recurso *recurso);

/// @brief Envía la solicitud a la io (el recurso) y luego espera y maneja la respuesta.
/// @param solicitud
/// @param recurso
void handle_io_fs_truncate(t_solicitud *solicitud, t_recurso *recurso);

/// @brief Envía la solicitud a la io (el recurso) y luego espera y maneja la respuesta.
/// @param solicitud
/// @param recurso
void handle_io_fs_write(t_solicitud *solicitud, t_recurso *recurso);

typedef struct
{
    int fd_IO;
    s_kernel_data msj;
    op_code opCode;
} s_send_kernel_io_fs_args;

#endif