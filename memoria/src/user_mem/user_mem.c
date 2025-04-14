#include "user_mem.h"

static void *user_mem = NULL;

static pthread_mutex_t mutex_user_mem;

void print_user_mem()
{
    return;
    char *user_mem_str = (char *)user_mem;
    t_config_memoria *cfg = get_config();

    log_debug(get_logger(), "USER MEM:");
    for (int i = 0; i <= cfg->TAM_MEMORIA; i++)
    {
        printf("%c", user_mem_str[i]);
    }
    printf("\n");
}

void instanciate_user_mem()
{
    t_config_memoria *cfg = get_config();
    if (user_mem == NULL)
    {
        user_mem = malloc(cfg->TAM_MEMORIA);
        for (int i = 0; i <= cfg->TAM_MEMORIA; i++)
        {
            char nothing = '_';
            memcpy(user_mem + i, &nothing, sizeof(char));
        }
        pthread_mutex_init(&mutex_user_mem, NULL);
    }
}

void destroy_user_mem()
{
    pthread_mutex_lock(&mutex_user_mem);
    free(user_mem);
    user_mem = NULL;
    pthread_mutex_unlock(&mutex_user_mem);
    pthread_mutex_destroy(&mutex_user_mem);
}

void *get_val_from_usermem(uint32_t position, uint32_t size)
{
    char *a = (char *)user_mem; // borrar solo para debugg
    void *value = NULL;
    value = malloc(size);
    pthread_mutex_lock(&mutex_user_mem);
    memcpy(value, user_mem + position, size);
    pthread_mutex_unlock(&mutex_user_mem);

    return value;
}

void set_val_to_usermem(void *value, uint32_t position, uint32_t size)
{
    pthread_mutex_lock(&mutex_user_mem);
    memcpy(user_mem + position, value, size);
    pthread_mutex_unlock(&mutex_user_mem);
}

// TODO: seguro hay muchos memory leaks acá
char *get_val_from_usermem_iterator(t_list *list, uint32_t total_size)
{
    t_list_iterator *l_iterator = list_iterator_create(list);
    char *data = malloc(total_size);
    int ptr = 0;

    while (list_iterator_has_next(l_iterator))
    {
        frames_element *current = (frames_element *)list_iterator_next(l_iterator);
        int current_size = (int)(current->tam_pag - current->offset - current->padding);
        int direction = (int)((current->tam_pag * current->frame) + current->offset);

        log_info(get_logger(), "PID: <%i> - Accion: <LEER> - Direccion fisica: <%i> - Tamaño <%i>", current->pid,
                 direction, current_size);

        void *algo_de_mem = get_val_from_usermem(direction, current_size);
        memcpy(data + ptr, algo_de_mem, current_size);
        ptr = ptr + current_size;
        free(algo_de_mem);
    }
    return data;
}

char *split_chunks(char *str, int start, int size)
{
    char *chunk = malloc(size);
    strncpy(chunk, str + start, size);
    return chunk;
}

// TODO: seguro hay muchos memory leaks acá
void set_val_into_usermem_iterator(t_list *list, char *val)
{
    t_list_iterator *l_iterator = list_iterator_create(list);
    int ptr = 0;

    while (list_iterator_has_next(l_iterator))
    {
        frames_element *current = (frames_element *)list_iterator_next(l_iterator);
        int current_size = (int)(current->tam_pag - current->offset - current->padding);
        int direction = (int)((current->tam_pag * current->frame) + current->offset);
        char *curr_val = split_chunks(val, ptr, current_size);
        // printf("curr size: %i \n", current_size);
        // printf("%s \n", curr_val);

        log_info(get_logger(), "PID: <%i> - Accion: <ESCRIBIR> - Direccion fisica: <%i> - Tamaño <%i>", current->pid,
                 direction, current_size);

        set_val_to_usermem(curr_val, direction, current_size);
        ptr = ptr + current_size;
        // char *memuria = (char *)user_mem;
        // printf("%s", memuria);
    }
}
