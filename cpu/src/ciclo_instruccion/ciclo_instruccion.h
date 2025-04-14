#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include <TAD/TAD.h>
#include <comunicacion_kernel/contextoEjecucionHandlers/contextoEjecucionHandlers.h>
#include <comunicacion_memoria/mensajes/mensajes_a_memoria.h>
#include <diccionarios/diccionario_instrucciones.h>
#include <globals/globals.h>
#include <instrucciones/instrucciones.h>

t_paquete *manejoDeInterrupciones();
t_paquete *ciclo_instruccion();
char **decode(char *instruccion);
t_paquete *execute(char **instruccion_decodificada);
t_paquete *paquete_prioritario(t_paquete *paquete1,
                               t_paquete *paquete2); // NOLINT(bugprone-easily-swappable-parameters)

#endif