#ifndef CARGAR_INSTRUCCIONES_H_
#define CARGAR_INSTRUCCIONES_H_

#include "leer_linea.h"
#include <TAD/TAD.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <globals/globals.h>

t_list *cargar_instrucciones(char *archivo);

#define CANTIDAD_MAXIMA_INSTRUCCIONES 100

#endif