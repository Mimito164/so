#include "tabla_paginas.h"

static t_tabla_paginas_global *tabla_paginas_global;

void crear_tabla_paginas_global()
{
    tabla_paginas_global = malloc(sizeof(t_tabla_paginas_global));
    tabla_paginas_global->tablas_por_proceso = list_create();
    sem_init(&(tabla_paginas_global->mutex), 0, 1);
}

/*
static void destroy_tabla_paginas_proceso_general(void *tabla)
{
    list_destroy(((t_tabla_paginas_proceso *)tabla)->frames);
    sem_destroy(&((t_tabla_paginas_proceso *)tabla)->mutex);
    free(tabla);
    tabla = NULL;
}
*/

void destroy_tabla_paginas_global()
{
    if (list_size(tabla_paginas_global->tablas_por_proceso) != 0)
    {
        list_destroy_and_destroy_elements(tabla_paginas_global->tablas_por_proceso,
                                          (void *)destroy_tabla_paginas_proceso);
    }
    else
    {
        list_destroy(tabla_paginas_global->tablas_por_proceso);
    }
    sem_destroy(&(tabla_paginas_global->mutex));
    free(tabla_paginas_global);
    tabla_paginas_global = NULL;
}

static t_tabla_paginas_proceso *crear_tabla_paginas_proceso(uint32_t pid)
{
    t_tabla_paginas_proceso *tabla = malloc(sizeof(t_tabla_paginas_proceso));
    tabla->pid = pid;
    tabla->frames = list_create();
    sem_init(&(tabla->mutex), 0, 1);
    return tabla;
}

void agregar_tabla_paginas_proceso_en_tabla_global(uint32_t pid)
{
    log_info(get_logger(), "Creacion de Tabla De Paginas PID %i - Tamanio 0", pid);
    t_tabla_paginas_proceso *tabla = crear_tabla_paginas_proceso(pid);
    sem_wait(&(tabla_paginas_global->mutex));
    list_add(tabla_paginas_global->tablas_por_proceso, tabla);
    sem_post(&(tabla_paginas_global->mutex));
}

int obtener_indice_tabla_paginas_proceso(uint32_t pid)
{
    t_list_iterator *iterator = list_iterator_create(tabla_paginas_global->tablas_por_proceso);
    int indice = -1;

    while (list_iterator_has_next(iterator))
    {
        t_tabla_paginas_proceso *tabla = list_iterator_next(iterator);
        if (tabla->pid == pid)
        {
            indice = list_iterator_index(iterator);
            break;
        }
    }
    list_iterator_destroy(iterator);

    if (indice == -1)
    {
        log_error(get_logger(), "El PID que recibi no existe");
    }

    return indice;
}

t_tabla_paginas_proceso *quitar_tabla_paginas_proceso(uint32_t pid)
{
    sem_wait(&(tabla_paginas_global->mutex));
    int indice = obtener_indice_tabla_paginas_proceso(pid);
    t_tabla_paginas_proceso *tabla = list_remove(tabla_paginas_global->tablas_por_proceso, indice);
    sem_post(&(tabla_paginas_global->mutex));
    return tabla;
}

void destroy_tabla_paginas_proceso(t_tabla_paginas_proceso *tabla)
{
    list_destroy(tabla->frames);
    sem_destroy(&(tabla->mutex));
    free(tabla);
    tabla = NULL;
}

static t_tabla_paginas_proceso *obtener_tabla_paginas_proceso(int indice)
{
    return list_get(tabla_paginas_global->tablas_por_proceso, indice);
}

t_tabla_paginas_proceso *buscar_tabla_paginas_proceso(uint32_t pid)
{
    sem_wait(&(tabla_paginas_global->mutex));
    int indice = obtener_indice_tabla_paginas_proceso(pid);
    t_tabla_paginas_proceso *tabla = obtener_tabla_paginas_proceso(indice);
    sem_post(&(tabla_paginas_global->mutex));
    return tabla;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
int obtener_frame(uint32_t pid, uint32_t pagina)
{
    t_tabla_paginas_proceso *tabla = buscar_tabla_paginas_proceso(pid);
    sem_wait(&(tabla->mutex));
    intptr_t puntero_frame = (intptr_t)list_get(tabla->frames, (int)pagina);
    // printf("puntero int: %ld", puntero_frame);
    int frame = (int)puntero_frame;
    // printf("int: %d", frame);
    sem_post(&(tabla->mutex));
    return frame;
}

// Para tests nomas
int obtener_tamanio_tabla_pags_global()
{
    return list_size(tabla_paginas_global->tablas_por_proceso);
}

t_tabla_paginas_global get_tabla_paginas_global()
{
    return *tabla_paginas_global;
}