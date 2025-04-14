#ifndef HANDLERS_MULTIPROGRAMACION_H_
#define HANDLERS_MULTIPROGRAMACION_H_

#include "accessors_multiprogramacion.h"
#include "admision/admision.h"
#include <stdlib.h>

/// @brief Aumenta en 1 el grado de multiprogramacion
void increment_multiprogramacion();

/// @brief Aumenta en 1 "n" veces el grado de multiprogramacion
/// @param times La cantidad de veces que se desea realizar esta operacion
void increment_n_multiprogramacion(int times);

/// @brief Disminuye en 1 el grado de multiprogramacion
void decrement_multiprogramacion();

/// @brief Disminuye en 1 "n" veces el grado de multiprogramacion
/// @param times La cantidad de veces que se desea realizar esta operacion
void decrement_n_multiprogramacion(int times);

/// @brief setea un nuevo valor de multiprogramacion y calcula la diferencia entre la multiprogramacion anterior y la
/// nuva para actualizar las veces que haga falta, tanto para incrementar como decrementar
/// @param new_multiprogramacion el nuevo valor de multiprogramacion
void update_multiprogramacion(int new_multiprogramacion);

#endif