#ifndef TABLA_INSTRUCCIONES_H_
#define TABLA_INSTRUCCIONES_H_

#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <globals/globals.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct t_tabla_instrucciones
{
    sem_t mutex;
    t_list *instrucciones_por_proceso;
} t_tabla_instrucciones;

void agregar_instrucciones_en_tabla(uint32_t pid, t_list *instrucciones);
void crear_tabla_instrucciones();
void destroy_tabla_de_instrucciones();
char *obtener_instruccion(uint32_t pid, uint32_t program_counter);
void destroy_tabla_instrucciones_proceso(uint32_t pid);

#endif