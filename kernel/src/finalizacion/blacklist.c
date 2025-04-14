#include "blacklist.h"

static t_dictionary *blacklist;
static pthread_mutex_t blacklist_mutex;
static bool initialized = false;

static void init_blacklist()
{
    initialized = true;
    blacklist = dictionary_create();
    pthread_mutex_init(&blacklist_mutex, NULL);
}

static void *add_to_blacklist(int *pid)
{
    char *key = string_itoa(*pid);
    pthread_mutex_lock(&blacklist_mutex);
    dictionary_put(blacklist, key, NULL);
    pthread_mutex_unlock(&blacklist_mutex);

    free(key);
    free(pid);

    return NULL;
}

/// @brief Agrega un proceso a la blacklist. Crea un hilo y no es bloqueante
/// @param pid
void blackilst_process(uint32_t pid, motivo_de_muerte motivo)
{
    if (!initialized)
    {
        init_blacklist();
    }
    char *key = string_itoa((int)pid);
    motivo_de_muerte *motivo_malloqueado = malloc(sizeof(motivo_de_muerte));
    *motivo_malloqueado = motivo;
    pthread_mutex_lock(&blacklist_mutex);
    dictionary_put(blacklist, key, (void *)motivo_malloqueado);
    pthread_mutex_unlock(&blacklist_mutex);
    free(key);
}

static void remove_from_blacklist(uint32_t pid) // NOLINT(misc-unused-parameters)
{
    char *key = string_itoa((int)pid);
    pthread_mutex_lock(&blacklist_mutex);
    motivo_de_muerte *motivo = (dictionary_get(blacklist, key));
    free(motivo);
    dictionary_remove(blacklist, key);
    pthread_mutex_unlock(&blacklist_mutex);
    free(key);
}

static bool find_in_blacklist(uint32_t pid) // NOLINT(misc-unused-parameters)
{
    bool is_in_blacklist = false;
    char *key = string_itoa((int)pid);
    pthread_mutex_lock(&blacklist_mutex);
    is_in_blacklist = dictionary_has_key(blacklist, key);
    pthread_mutex_unlock(&blacklist_mutex);
    free(key);

    return is_in_blacklist;
}

static motivo_de_muerte *get_motivo(uint32_t pid)
{
    char *key = string_itoa((int)pid);
    pthread_mutex_lock(&blacklist_mutex);
    motivo_de_muerte *motivo = (dictionary_get(blacklist, key));
    pthread_mutex_unlock(&blacklist_mutex);
    free(key);

    return motivo;
}

/// @brief Revisa la blacklist y mata el proceso si se encuentra allí
/// @param transicion la transición en la que se realiza esto
/// @param pid el proceso que está en la transición
/// @return true si lo mató, false si el pid sigue vivo
bool check_blacklist(t_transicion transicion, uint32_t pid, t_pcb *pcb)
{
    if (!initialized)
    {
        init_blacklist();
    }

    bool is_in_blacklist = find_in_blacklist(pid); // revisamos que el pcb este en la blacklist

    if (!is_in_blacklist) // si no esta retornamos false
    {
        return false;
    }

    motivo_de_muerte *motivo = get_motivo(pid);
    bool found = finalizacion(transicion, pid, pcb, motivo); // si esta lo matamos
    if (found)
    {
        remove_from_blacklist(pid); // si lo matamos lo quitamos de la lista
    }
    return found; // si lo matamos retornamos true
}

static bool find_and_kill_per_list(t_transicion transicion, uint32_t pid)
{
    bool is_in_blacklist = find_in_blacklist(pid);

    if (!is_in_blacklist)
    {
        return true; // ya lo mataron
    }

    if (finalizacion(transicion, pid, NULL, get_motivo(pid)))
    {
        remove_from_blacklist(pid);
        return true; // lo matamos
    }

    return false; // sigue vivo
}

/// @brief Busca en cada lista un proceso y si lo encuentra lo mata \
/// @brief Frena si ya lo mataron en una transición
/// @param pid que se quiere matar
void find_and_kill(uint32_t pid)
{

    // Busca en BLOCKED
    if (find_and_kill_per_list(BLOCKED_A_EXIT, pid))
    {
        return;
    }

    // Busca en NEW
    if (find_and_kill_per_list(NEW_A_READY, pid))
    {
        return;
    }

    // Busca en READY
    if (find_and_kill_per_list(READY_A_EXEC, pid))
    {
        return;
    }

    // Busca en EXEC
    if (find_and_kill_per_list(EXEC_A_EXIT, pid))
    {
        return;
    }
}

// testing
statics_t *get_statics()
{
    statics_t *statics_struct = malloc(sizeof(statics_t));
    statics_struct->blacklist = &blacklist;
    statics_struct->blacklist_mutex = blacklist_mutex;
    statics_struct->init_blacklist = &init_blacklist;
    statics_struct->add_to_blacklist = &add_to_blacklist;
    statics_struct->remove_from_blacklist = &remove_from_blacklist;
    statics_struct->find_in_blacklist = &find_in_blacklist;
    statics_struct->find_and_kill_per_list = &find_and_kill_per_list;
    return statics_struct;
}

void destroy_blacklist()
{
    if (!initialized)
    {
        return;
    }

    pthread_mutex_lock(&blacklist_mutex);
    dictionary_destroy_and_destroy_elements(blacklist, &free);
    pthread_mutex_unlock(&blacklist_mutex);
    pthread_mutex_destroy(&blacklist_mutex);
}
