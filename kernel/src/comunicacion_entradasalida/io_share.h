#ifndef COMUNICACION_IO_SHARE_H_
#define COMUNICACION_IO_SHARE_H_

#include "gestor_recursos/gestor_recursos_share.h"

typedef struct
{
    t_recurso *recurso;
    t_solicitud *solicitud;
    op_code opCode;
} args_p_solicitud;

#endif