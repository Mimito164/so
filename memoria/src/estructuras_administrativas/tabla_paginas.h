#ifndef TABLA_PAGINAS_H_
#define TABLA_PAGINAS_H_

#include <commons/collections/list.h>
#include <commons/log.h>
#include <globals/globals.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct t_tabla_paginas_global
{
    sem_t mutex;
    t_list *tablas_por_proceso;
} t_tabla_paginas_global;

typedef struct t_tabla_paginas_proceso
{
    sem_t mutex;
    t_list *frames;
    uint32_t pid;
} t_tabla_paginas_proceso;

void agregar_tabla_paginas_proceso_en_tabla_global(uint32_t pid);
void crear_tabla_paginas_global();
void destroy_tabla_paginas_global();
int obtener_frame(uint32_t pid, uint32_t pagina);
void destroy_tabla_paginas_proceso(t_tabla_paginas_proceso *tabla);
int obtener_indice_tabla_paginas_proceso(uint32_t pid);
t_tabla_paginas_proceso *buscar_tabla_paginas_proceso(uint32_t pid);
t_tabla_paginas_proceso *quitar_tabla_paginas_proceso(uint32_t pid);

// para tests nomas
int obtener_tamanio_tabla_pags_global();
t_tabla_paginas_global get_tabla_paginas_global();

#endif