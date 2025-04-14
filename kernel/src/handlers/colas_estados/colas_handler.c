#include "colas_handler.h"

// colas de procesos
static t_list *new_list = NULL;
static t_list *ready_list = NULL;
static t_list *pready_list = NULL;
static t_list *blocked_list = NULL; // seguramente termina siendo una lista de lista
static t_list *exit_list = NULL;    // yo creo que deberia ser una funcion, pero agrego la lista de momento
static t_pcb *executing = NULL;     // los mutex de executing los inicializamos donde los usemos
static t_dictionary *dictionary_list_mutex;

// semaforos
sem_t mutex_new_list;
sem_t mutex_lista_ready;
sem_t mutex_lista_pready;
sem_t mutex_lista_bloqueados;
sem_t mutex_lista_exit;

pthread_mutex_t mutex_exec;

void initialize_new_list()
{
    sem_init(&mutex_new_list, 0, 1);
    new_list = list_create();
    dictionary_put(dictionary_list_mutex, "NEW", new_list);
    dictionary_put(dictionary_list_mutex, "MUTEX_NEW", &mutex_new_list);
}

void destroy_new_list()
{
    dictionary_remove(dictionary_list_mutex, "NEW");
    dictionary_remove(dictionary_list_mutex, "MUTEX_NEW");
    sem_destroy(&mutex_new_list);
    list_destroy_and_destroy_elements(new_list, (void *)destroy_pcb);
    new_list = NULL;
}

void initialize_ready_list()
{
    sem_init(&mutex_lista_ready, 0, 1);
    ready_list = list_create();
    dictionary_put(dictionary_list_mutex, "READY", ready_list);
    dictionary_put(dictionary_list_mutex, "MUTEX_READY", &mutex_lista_ready);
}

void destroy_ready_list()
{
    dictionary_remove(dictionary_list_mutex, "READY");
    dictionary_remove(dictionary_list_mutex, "MUTEX_READY");
    sem_destroy(&mutex_lista_ready);
    list_destroy_and_destroy_elements(ready_list, (void *)destroy_pcb);
    ready_list = NULL;
}

void initialize_pready_list()
{
    sem_init(&mutex_lista_pready, 0, 1);
    pready_list = list_create();
    dictionary_put(dictionary_list_mutex, "PREADY", pready_list);
    dictionary_put(dictionary_list_mutex, "MUTEX_PREADY", &mutex_lista_pready);
}

void destroy_pready_list()
{
    dictionary_remove(dictionary_list_mutex, "PREADY");
    dictionary_remove(dictionary_list_mutex, "MUTEX_PREADY");
    sem_destroy(&mutex_lista_pready);
    list_destroy_and_destroy_elements(pready_list, (void *)destroy_pcb);
    pready_list = NULL;
}

void initialize_blocked_list()
{
    sem_init(&mutex_lista_bloqueados, 0, 1);
    blocked_list = list_create();
    dictionary_put(dictionary_list_mutex, "BLOCKED", blocked_list);
    dictionary_put(dictionary_list_mutex, "MUTEX_BLOCKED", &mutex_lista_bloqueados);
}

void destroy_blocked_list()
{
    dictionary_remove(dictionary_list_mutex, "BLOCKED");
    dictionary_remove(dictionary_list_mutex, "MUTEX_BLOCKED");
    sem_destroy(&mutex_lista_bloqueados);
    list_destroy_and_destroy_elements(blocked_list, (void *)destroy_pcb);
    blocked_list = NULL;
}

void initialize_exit_list()
{
    sem_init(&mutex_lista_exit, 0, 1);
    exit_list = list_create();
    dictionary_put(dictionary_list_mutex, "EXIT", exit_list);
    dictionary_put(dictionary_list_mutex, "MUTEX_EXIT", &mutex_lista_exit);
}

void destroy_exit_list()
{
    dictionary_remove(dictionary_list_mutex, "EXIT");
    dictionary_remove(dictionary_list_mutex, "MUTEX_EXIT");
    sem_destroy(&mutex_lista_exit);
    list_destroy_and_destroy_elements(exit_list, (void *)destroy_pcb);
    exit_list = NULL;
}

static void initialize_mutex_exec()
{
    pthread_mutex_init(&mutex_exec, NULL);
}

void initialize_executing()
{
    initialize_mutex_exec();
    executing = NULL;
}

static void destroy_mutex_exec()
{
    pthread_mutex_destroy(&mutex_exec);
}

void destroy_executing()
{
    free(executing);
    destroy_mutex_exec();
}

void set_executing(t_pcb *executing_pcb)
{

    pthread_mutex_lock(&mutex_exec);
    executing = executing_pcb;
    pthread_mutex_unlock(&mutex_exec);
}

t_pcb *get_executing()
{
    t_log *logger = get_logger();
    if (executing)
    {
        log_trace(logger, "get_executing: en executing está pid %d\n", executing->pid);
    }
    else
    {
        log_trace(logger, "get_executing: No hay nada en executing\n");
    }
    return executing;
}

t_pcb *get_next_pcb(char *string_list)
{
    char *concatenated_key = malloc(strlen(string_list) + strlen("MUTEX_") + 1);
    t_log *logger = get_logger();
    strcpy(concatenated_key, "MUTEX_");
    strcat(concatenated_key, string_list);

    sem_t *lista_mutex = (sem_t *)dictionary_get(dictionary_list_mutex, concatenated_key);
    free(concatenated_key);
    sem_wait(lista_mutex);

    t_list *lista = (t_list *)dictionary_get(dictionary_list_mutex, string_list);

    if (!list_is_empty(lista))
    {
        t_pcb *to_send = (t_pcb *)list_remove(lista, 0);
        sem_post(lista_mutex);
        if (to_send != NULL)
        {
            log_trace(logger, "Se saca el pcb %d de la lista %s\n", to_send->pid, string_list);
        }
        return to_send;
    }

    sem_post(lista_mutex);
    log_trace(logger, "La lista %s esta vacia\n", string_list);
    return NULL;
}

void add_pcb(char *string_list, t_pcb *pcb)
{
    char *concatenated_key = malloc(strlen(string_list) + strlen("MUTEX_") + 1);
    strcpy(concatenated_key, "MUTEX_");
    strcat(concatenated_key, string_list);
    sem_wait((sem_t *)dictionary_get(dictionary_list_mutex, concatenated_key));
    list_add((t_list *)dictionary_get(dictionary_list_mutex, string_list), pcb);
    sem_post((sem_t *)dictionary_get(dictionary_list_mutex, concatenated_key));
    free(concatenated_key);
    t_log *logger = get_logger();
    log_trace(logger, "Se agrega el pcb %d a la lista %s\n", pcb->pid, string_list);
}

t_pcb *find_pcb_by_pid(char *string_list, uint32_t pid)
{
    bool _is_the_one(t_pcb * p)
    {
        return p->pid == pid;
    }

    char *concatenated_key = malloc(strlen(string_list) + strlen("MUTEX_") + 1);
    t_log *logger = get_logger();
    strcpy(concatenated_key, "MUTEX_");
    strcat(concatenated_key, string_list);

    sem_t *lista_mutex = (sem_t *)dictionary_get(dictionary_list_mutex, concatenated_key);
    free(concatenated_key);
    sem_wait(lista_mutex);

    t_list *lista = (t_list *)dictionary_get(dictionary_list_mutex, string_list);

    if (!list_is_empty(lista))
    {
        t_pcb *to_send = (t_pcb *)list_remove_by_condition(lista, (void *)&_is_the_one);
        sem_post(lista_mutex);
        if (to_send != NULL)
        {
            log_trace(logger, "Se saca el pcb %d de la lista %s\n", to_send->pid, string_list);
            return to_send;
        }
        else
        {
            log_trace(logger, "El pcb %d no se encontró en la lista %s\n", pid, string_list);
            return NULL;
        }
    }

    sem_post(lista_mutex);
    log_trace(logger, "La lista %s esta vacia\n", string_list);
    return NULL;
}

void initialize_lists()
{
    dictionary_list_mutex = dictionary_create();
    initialize_new_list();
    initialize_ready_list();
    initialize_pready_list();
    initialize_blocked_list();
    initialize_exit_list();
    initialize_executing();
    t_log *logger = get_logger();
    log_trace(logger, "Se inicializaron las listas correctamente\n");
}

void destroy_lists()
{
    destroy_new_list();
    destroy_ready_list();
    destroy_pready_list();
    destroy_blocked_list();
    destroy_exit_list();
    destroy_executing();
    dictionary_destroy(dictionary_list_mutex);
    t_log *logger = get_logger();
    log_trace(logger, "Se eliminaron las listas correctamente\n");
}

static void print_pcb(void *raw_pcb)
{
    t_pcb *pcb = (t_pcb *)raw_pcb;

    log_info(get_logger(), "\n\
        ================ \n\
        PID: %i \n\
        Quantum: %i \n\
        Registros CPU: \n\
        - PC: %i \n\
        - AX: %i \n\
        - BX: %i \n\
        - CX: %i \n\
        - DX: %i \n\
        - EAX: %i \n\
        - EBX: %i \n\
        - ECX: %i \n\
        - EDX: %i \n\
        - SI: %i \n\
        - DI: %i \n\
        ================ \n",
             pcb->pid, pcb->quantum, pcb->registros_cpu->PC, pcb->registros_cpu->AX, pcb->registros_cpu->BX,
             pcb->registros_cpu->CX, pcb->registros_cpu->DX, pcb->registros_cpu->EAX, pcb->registros_cpu->EBX,
             pcb->registros_cpu->ECX, pcb->registros_cpu->EDX, pcb->registros_cpu->SI, pcb->registros_cpu->DI);
}

static void print_list(char *string_list)
{
    char *concatenated_key = malloc(strlen(string_list) + strlen("MUTEX_") + 1);
    t_log *logger = get_logger();
    strcpy(concatenated_key, "MUTEX_");
    strcat(concatenated_key, string_list);

    sem_t *lista_mutex = (sem_t *)dictionary_get(dictionary_list_mutex, concatenated_key);
    free(concatenated_key);
    sem_wait(lista_mutex);

    t_list *lista = (t_list *)dictionary_get(dictionary_list_mutex, string_list);

    log_info(logger, "\n\
        ================================ \n\
        %s: %i \n\
        ================================ \n",
             string_list, list_size(lista));
    list_iterate(lista, &print_pcb);

    sem_post(lista_mutex);
}

void print_all_lists()
{
    print_list("NEW");
    print_list("READY");
    print_list("BLOCKED");
    t_pcb *exec_pcb = get_executing();
    if (exec_pcb == NULL)
    {
        log_info(get_logger(), "\n\
        ================================ \n\
        EXEC: 0 \n\
        ================================ \n");
    }
    else
    {
        log_info(get_logger(), "\n\
        ================================ \n\
        EXEC: 1 \n\
        ================================ \n");
        print_pcb(get_executing());
    }
}

// estas funciones estan solo para los tests
int len_dict()
{
    return dictionary_size(dictionary_list_mutex);
}

int len_list(char *string_list)
{
    return list_size((t_list *)dictionary_get(dictionary_list_mutex, string_list));
}
