#ifndef ACCESSORS_MULTIPRIGRAMACION_H_
#define ACCESSORS_MULTIPRIGRAMACION_H_

#include <pthread.h>

/// @brief Inicializa el mutex para la variable de multiprogramacion
void init_mutex_multiprogramacion();

/// @brief Finaliza el mutex para la variable de multiprogramacion
void destroy_mutex_multiprogramacion();

/// @brief Setea el valor del grado de multiprogramacion en un valor deseado.
/// @param new_value Valor que sera seteado en el grado de multiprogramacion.
void set_multiprogramacion(int new_value);

/// @brief Devuelve el grado de multiprogramacion que este seteado.
/// @return Un numero entero que simboliza el grado de multiprogramacion.
int get_multiprogramacion();

#endif