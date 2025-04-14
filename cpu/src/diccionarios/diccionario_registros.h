#ifndef DICCIONARIO_REGISTROS_CPU_H_
#define DICCIONARIO_REGISTROS_CPU_H_

#include <commons/collections/dictionary.h>
#include <comunicacion_kernel/contextoEjecucionHandlers/contextoEjecucionHandlers.h>
#include <stdlib.h>

t_dictionary *get_diccionario_registros_set();
t_dictionary *get_diccionario_registros_get();
void diccionario_registros_get_destroy();
void diccionario_registros_set_destroy();
#endif