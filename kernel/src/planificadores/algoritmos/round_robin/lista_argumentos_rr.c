#include "planificadores/algoritmos/round_robin/lista_argumentos_rr.h"

/**
 * la idea es que se guarden los argumentos de los hilos en una lista y a medida que se dejan de usar se van
 * autoeliminando
 */
static t_list *quantum_args_list;

static bool isEnded(void *quantum_args)
{
    quantum_thread_args *casted_args = (quantum_thread_args *)quantum_args;
    pthread_mutex_unlock(&casted_args->args_mutex);
    bool end_state = casted_args->ended;
    pthread_mutex_lock(&casted_args->args_mutex);

    return end_state;
}

static void remove_ended()
{

    // _remove_by_condition(quantum_args_list, isEnded);
    list_remove_and_destroy_by_condition(quantum_args_list, isEnded, free);
}

void initialize_quantum_args_list()
{
    quantum_args_list = list_create();
}

void destroy_quantum_args_list()
{
    remove_ended();
    list_destroy_and_destroy_elements(quantum_args_list, free);
}

void disable_last_quantum_args()
{
    if (!!strcmp(get_config()->ALGORITMO_PLANIFICACION, "RR"))
    {
        return;
    }
    remove_ended();
    int size_list = list_size(quantum_args_list);
    if (size_list == 0)
    {
        return;
    }

    quantum_thread_args *last_quantum_args = (quantum_thread_args *)list_get(quantum_args_list, size_list - 1);
    last_quantum_args->is_active = false;
}

void add_argument(void *argument)
{
    if (quantum_args_list == NULL)
    {
        initialize_quantum_args_list();
    }

    remove_ended();

    list_add(quantum_args_list, argument);
}