#include <TLB/TLB.h>

//  - PID | NºPAG | NºMARCO | TIMESTAMP
static t_list *tlb;
/// @brief Obtiene el tamanio de la tlb
/// @return int
int obtener_tamanio_tlb()
{
    return tlb->elements_count;
}
/// @brief Agrega una entrada a la tlb
/// @param int marco
/// @param int pag
/// @param int pid
void cargar_entrada(int marco, int pag, int pid)
{
    list_add(tlb, (void *)crear_entrada(marco, pag, pid));
}
/// @brief Crea una entrada en la tlb
/// @param int marco
/// @param int pag
/// @param int pid
/// @return t_entrada
t_entrada *crear_entrada(int marco, int pag, int pid)
{
    t_entrada *entrada = (t_entrada *)malloc(sizeof(t_entrada));
    entrada->timestamp = temporal_create();
    entrada->n_marco = marco;
    entrada->n_pag = pag;
    entrada->pid = pid;
    return entrada;
}
/// @brief Para Inicializars la TLB crea entradas en 0 y crea la lista de entradas
void init_TLB()
{
    tlb = list_create();
}

static void destroy_entradas_general(void *entrada)
{
    t_entrada *entrada_casteada = (t_entrada *)entrada;
    temporal_destroy(entrada_casteada->timestamp);
    free(entrada);
    entrada = NULL;
}

/// @brief Destruye tlb
void destroy_tlb()
{
    list_destroy_and_destroy_elements(tlb, destroy_entradas_general);
}

/// @brief Destruye la entrada y agrega un nuevo timestamp
/// @param t_entrada*
void actualizar_entrada(t_entrada *entrada_a_actualizar)
{
    temporal_destroy(entrada_a_actualizar->timestamp);
    entrada_a_actualizar->timestamp = temporal_create();
}

///-------------------------- Algoritmos -------------------

/// @brief Algoritmo de Fifo
/// @param marco
/// @param pag
/// @param pid
void reemplazar_por_fifo(int marco, int pag, int pid)
{
    t_entrada *entrada = list_remove(tlb, 0);
    log_debug(get_logger(), "Remplazo entrada PID: %i - Pagina: %i", entrada->pid, entrada->n_pag);
    temporal_destroy(entrada->timestamp);
    free(entrada);
    entrada = NULL;
    cargar_entrada(marco, pag, pid);
}
/// @brief Algoritmo para LRU
/// @param entrada1
/// @param entrada2
/// @return void*
void *lru(t_entrada *entrada1, t_entrada *entrada2)
{
    return temporal_diff(entrada1->timestamp, entrada2->timestamp) > 0 ? entrada1 : entrada2;
}

/// @brief Reemplazar utilizando LRU
void reemplazar_por_LRU(int marco, int pag, int pid)
{
    t_entrada *entrada_a_reemplazar = (t_entrada *)list_get_maximum(tlb, (void *)lru);
    log_debug(get_logger(), "Remplazo entrada PID: %i - Pagina: %i", entrada_a_reemplazar->pid,
              entrada_a_reemplazar->n_pag);
    bool buscar_entrada_maxima(t_entrada * entrada)
    {
        return entrada_a_reemplazar == entrada;
    }
    void *variable = list_replace_by_condition(tlb, (void *)buscar_entrada_maxima, crear_entrada(marco, pag, pid));
    if (!variable)
    {
        free(variable);
    }
    temporal_destroy(entrada_a_reemplazar->timestamp);
    free(entrada_a_reemplazar);
    entrada_a_reemplazar = NULL;
}

/// @brief Remplazar un frame por los algoritmos FIFO | LRU
/// @param marco
/// @param pag
/// @param pid
void reemplazar_frame(int marco, int pag, int pid)
{
    t_config_cpu *cfg = get_config();
    if (!strcmp(cfg->ALGORITMO_TLB, "FIFO"))
    {
        reemplazar_por_fifo(marco, pag, pid);
    }
    else
    {
        reemplazar_por_LRU(marco, pag, pid);
    }
} // return;

/// @brief Obtener numero marco de la tlb
/// @param pagina
/// @param pid
/// @return int (Numero de pagina o -1 si hace tlb miss)
int obtener_entrada(int pagina, int pid)
{
    int valor = -1;
    bool filter_pid_pagina(void *entrada)
    {
        return ((t_entrada *)entrada)->pid == pid && ((t_entrada *)entrada)->n_pag == pagina;
    }

    t_list *lista_filtrada = list_filter(tlb, filter_pid_pagina);

    if (list_size(lista_filtrada))
    {
        if (!strcmp(get_config()->ALGORITMO_TLB, "LRU"))
        {
            actualizar_entrada(list_get(lista_filtrada, 0));
        }

        valor = ((t_entrada *)list_get(lista_filtrada, 0))->n_marco;
    }

    list_destroy(lista_filtrada);
    return valor;
}

void cargar_si_puedo(int marco, int pag, int pid)
{
    t_config_cpu *cfg = get_config();

    if (cfg->CANTIDAD_ENTRADAS_TLB == 0)
    {
        return;
    }

    if (list_size(tlb) < cfg->CANTIDAD_ENTRADAS_TLB)
    {

        cargar_entrada(marco, pag, pid);
    }
    else
    {
        reemplazar_frame(marco, pag, pid);
    }
    // si da tlb miss cpu pide a memoria marco y cpu pasa tlb pid y marco
}