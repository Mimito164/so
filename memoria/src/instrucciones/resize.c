#include "resize.h"

static int calcular_cantidad_frames(int tamanio)
{
    int resto = tamanio % get_config()->TAM_PAGINA;
    if (resto != 0)
    {
        return tamanio / get_config()->TAM_PAGINA + 1;
    }
    return tamanio / get_config()->TAM_PAGINA;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
bool resize(uint32_t pid, uint32_t tamanio)
{
    int cantidad = calcular_cantidad_frames((int)tamanio);
    t_tabla_paginas_proceso *tabla = buscar_tabla_paginas_proceso(pid);
    sem_wait(&(tabla->mutex));
    if (list_size(tabla->frames) < cantidad)
    {
        int cantidad_real = cantidad - list_size(tabla->frames);
        log_info(get_logger(), "Ampliacion De Proceso PID %i - Tamanio actual %i - Tamanio a ampliar %i", pid,
                 list_size(tabla->frames), cantidad);
        bool respuesta = asignarFrames(cantidad_real, tabla->frames);
        sem_post(&(tabla->mutex));
        return respuesta;
    }
    if (list_size(tabla->frames) == cantidad)
    {
        log_info(get_logger(), "Mismo tamanio, no hacemos nada :)");
        sem_post(&(tabla->mutex));
        return true;
    }
    int cantidad_real = list_size(tabla->frames) - cantidad;
    log_info(get_logger(), "Reduccion De Proceso PID %i - Tamanio actual %i - Tamanio a reducir %i", pid,
             list_size(tabla->frames), cantidad);
    liberar_frames(cantidad_real, tabla->frames);
    sem_post(&(tabla->mutex));
    return true;
}