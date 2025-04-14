#include "tabla_instrucciones.h"

typedef struct instrucciones_por_proceso
{
    uint32_t pid;
    t_list *instrucciones;
} instrucciones_por_proceso;

static t_tabla_instrucciones *tabla_instrucciones;

static void destroy_instrucciones(void *instruccion)
{
    free(instruccion);
    instruccion = NULL;
}

void destroy_tabla_instrucciones_proceso(uint32_t pid)
{
    sem_wait(&(tabla_instrucciones->mutex));
    t_list_iterator *iterator = list_iterator_create(tabla_instrucciones->instrucciones_por_proceso);
    int indice = -1;

    while (list_iterator_has_next(iterator))
    {
        instrucciones_por_proceso *instrucciones = list_iterator_next(iterator);
        if (instrucciones->pid == pid)
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

    instrucciones_por_proceso *tabla_instruccion_proceso =
        list_remove(tabla_instrucciones->instrucciones_por_proceso, indice);
    list_destroy_and_destroy_elements(tabla_instruccion_proceso->instrucciones, destroy_instrucciones);
    sem_post(&(tabla_instrucciones->mutex));
    free(tabla_instruccion_proceso);
    tabla_instruccion_proceso = NULL;
}

instrucciones_por_proceso *crear_instrucciones_por_proceso(uint32_t pid, t_list *instrucciones)
{
    instrucciones_por_proceso *instrucciones_x_proceso = malloc(sizeof(instrucciones_por_proceso));
    instrucciones_x_proceso->pid = pid;
    instrucciones_x_proceso->instrucciones = instrucciones;
    return instrucciones_x_proceso;
}

void agregar_instrucciones_en_tabla(uint32_t pid, t_list *instrucciones)
{
    log_debug(get_logger(), "agregar_instrucciones_en_tabla");
    log_debug(get_logger(), "pid %i", pid);
    sem_wait(&(tabla_instrucciones->mutex));
    instrucciones_por_proceso *instrucciones_x_proceso = crear_instrucciones_por_proceso(pid, instrucciones);
    list_add(tabla_instrucciones->instrucciones_por_proceso, (void *)instrucciones_x_proceso);
    sem_post(&(tabla_instrucciones->mutex));
}

// void destroy_instrucciones_por_proceso(instrucciones_por_proceso *instrucciones_x_proceso)
// {
//     list_destroy_and_destroy_elements(instrucciones_x_proceso->instrucciones, destroy_instrucciones);
//     free(instrucciones_x_proceso);
//     instrucciones_x_proceso = NULL;
// }

void crear_tabla_instrucciones()
{
    tabla_instrucciones = malloc(sizeof(t_tabla_instrucciones));
    tabla_instrucciones->instrucciones_por_proceso = list_create();
    sem_init(&(tabla_instrucciones->mutex), 0, 1);
}

static void destroy_instrucciones_por_proceso_general(void *instrucciones_x_proceso)
{

    list_destroy_and_destroy_elements(((instrucciones_por_proceso *)instrucciones_x_proceso)->instrucciones,
                                      destroy_instrucciones);
    free(instrucciones_x_proceso);
    instrucciones_x_proceso = NULL;
}

void destroy_tabla_de_instrucciones()
{
    if (list_size(tabla_instrucciones->instrucciones_por_proceso) != 0)
    {
        list_destroy_and_destroy_elements(tabla_instrucciones->instrucciones_por_proceso,
                                          destroy_instrucciones_por_proceso_general);
    }
    else
    {
        list_destroy(tabla_instrucciones->instrucciones_por_proceso);
    }
    sem_destroy(&(tabla_instrucciones->mutex));
    free(tabla_instrucciones);
    tabla_instrucciones = NULL;
}

instrucciones_por_proceso *obtener_instruccion_por_proceso(uint32_t pid, t_list *tabla_de_instrucciones)
{
    log_debug(get_logger(), "obtener_instruccion_por_proceso");
    log_debug(get_logger(), "pid: %i", pid);

    t_list_iterator *iterator = list_iterator_create(tabla_de_instrucciones);
    int indice = -1;

    while (list_iterator_has_next(iterator))
    {
        instrucciones_por_proceso *instrucciones = list_iterator_next(iterator);
        if (instrucciones->pid == pid)
        {
            indice = list_iterator_index(iterator);
            break;
        }
    }
    list_iterator_destroy(iterator);

    if (indice == -1)
    {
        log_error(get_logger(), "El PID que recibi no existe");
        return NULL;
    }

    return list_get(tabla_de_instrucciones, indice);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
char *obtener_instruccion(uint32_t pid, uint32_t program_counter)
{
    sem_wait(&(tabla_instrucciones->mutex));
    instrucciones_por_proceso *instruccion_x_proceso =
        obtener_instruccion_por_proceso(pid, tabla_instrucciones->instrucciones_por_proceso);
    sem_post(&(tabla_instrucciones->mutex));
    return string_duplicate((char *)list_get(instruccion_x_proceso->instrucciones, (int)program_counter));
}