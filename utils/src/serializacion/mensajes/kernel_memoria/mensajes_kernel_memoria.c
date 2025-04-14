#include "mensajes_kernel_memoria.h"

// KERNEL_MEMORIA_CREAR_PROCESO;
s_kernel_memoria_creacion_proceso create_k_m_creacion_proceso(uint32_t pid, char *pseudocodigo_file_path)
{
    s_kernel_memoria_creacion_proceso s_k_m_creacion;
    s_k_m_creacion.path = pseudocodigo_file_path;
    s_k_m_creacion.pid = pid;
    return s_k_m_creacion;
}
s_kernel_memoria_creacion_proceso deserializar_k_m_creacion_proceso(t_paquete *paquete)
{
    s_kernel_memoria_creacion_proceso s_k_m_creacion;
    deserializar_uint32_t(paquete->buffer, &s_k_m_creacion.pid);
    deserializar_string(paquete->buffer, &s_k_m_creacion.path);
    return s_k_m_creacion;
}
t_paquete *serializar_k_m_creacion_proceso(s_kernel_memoria_creacion_proceso s_k_m_creacion_pros)
{
    t_paquete *paquete_memoria = paquete_new();
    paquete_memoria->codigo_operacion = KERNEL_MEMORIA_CREAR_PROCESO;
    serializar_uint32_t(&paquete_memoria->buffer, s_k_m_creacion_pros.pid);
    serializar_string(&paquete_memoria->buffer, s_k_m_creacion_pros.path);
    return paquete_memoria;
}

uint32_t deserializar_k_m_finalizacion_proceso(t_paquete *paquete)
{
    uint32_t s_k_m_pid;
    deserializar_uint32_t(paquete->buffer, &s_k_m_pid);
    return s_k_m_pid;
}

t_paquete *finalizar_proceso_memoria(uint32_t pid)
{
    t_paquete *paquete_memoria = paquete_new();
    paquete_memoria->codigo_operacion = KERNEL_MEMORIA_FINALIZAR_PROCESO;
    serializar_uint32_t(&paquete_memoria->buffer, pid);

    return paquete_memoria;
}
