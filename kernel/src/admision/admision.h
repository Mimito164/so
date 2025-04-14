#ifndef ADMISION_H_
#define ADMISION_H_

#include "handlers/colas_estados/colas_handler.h"
#include "logs/logs.h"
#include "multiprogramacion/accessors_multiprogramacion.h"
#include "planificador_corto/planificador_corto.h"
#include <stdbool.h>

/// @brief Esta funcion se encarga de verificar si existen procesos en new esperando para transitar al estado de READY y
/// evaluar si se dan las condiciones necesarias realizar esta accion.
void admit();

#endif