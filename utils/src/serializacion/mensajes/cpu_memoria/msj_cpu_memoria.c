#include "msj_cpu_memoria.h"

// TODO(all): falta testear

void serializar_frames_element(void **b, void *pkt)
{
    t_buffer **buffer = (t_buffer **)b;
    frames_element *frames_element_pkt = (frames_element *)pkt;

    serializar_uint32_t(buffer, frames_element_pkt->pid);     // 1. pid
    serializar_uint32_t(buffer, frames_element_pkt->offset);  // 2. offset
    serializar_uint32_t(buffer, frames_element_pkt->padding); // 3. padding
    serializar_uint32_t(buffer, frames_element_pkt->frame);   // 4. frame
    serializar_uint32_t(buffer, frames_element_pkt->tam_pag); // 5. tamaño de pag

    // serializar_void(buffer, frames_element_pkt->data); // TODO(all): no lo necesito en copy string
};

void *deserializar_frames_element(t_buffer *buffer)
{
    frames_element *frames_element_pkt = malloc(sizeof(frames_element));

    deserializar_uint32_t(buffer, &frames_element_pkt->pid);     // 1. pid
    deserializar_uint32_t(buffer, &frames_element_pkt->offset);  // 2. offset
    deserializar_uint32_t(buffer, &frames_element_pkt->padding); // 3. padding
    deserializar_uint32_t(buffer, &frames_element_pkt->frame);   // 4. frame
    deserializar_uint32_t(buffer, &frames_element_pkt->tam_pag); // 5. tamaño de pag

    // deserializar_void(buffer, frames_element_pkt->data); // TODO(all): no lo necesito en copy string

    return (void *)frames_element_pkt;
};

t_paquete *serializar_c_m_copy_string(s_cpu_memoria_copy_string s_c_m_copy_string)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = CPU_MEMORIA_COPIAR_STRING;

    serializar_uint32_t(&paquete->buffer, s_c_m_copy_string.pid);                                // 1. PID
    serializar_uint32_t(&paquete->buffer, s_c_m_copy_string.size);                               // 2. Size
    serializar_t_list(&(paquete->buffer), s_c_m_copy_string.source, serializar_frames_element);  // 3. Source
    serializar_t_list(&(paquete->buffer), s_c_m_copy_string.destiny, serializar_frames_element); // 4. Destiny

    return paquete;
}

s_cpu_memoria_copy_string deserializar_c_m_copy_string(t_paquete *paquete)
{
    s_cpu_memoria_copy_string s_c_m_copy_string;
    assert(paquete->codigo_operacion == CPU_MEMORIA_COPIAR_STRING);

    s_c_m_copy_string.source = list_create();
    s_c_m_copy_string.destiny = list_create();

    deserializar_uint32_t(paquete->buffer, &s_c_m_copy_string.pid);                                // 1. PID
    deserializar_uint32_t(paquete->buffer, &s_c_m_copy_string.size);                               // 2. Size
    deserializar_t_list(paquete->buffer, &s_c_m_copy_string.source, deserializar_frames_element);  // 3. Source
    deserializar_t_list(paquete->buffer, &s_c_m_copy_string.destiny, deserializar_frames_element); // 4. Destiny

    return s_c_m_copy_string;
}

t_paquete *serializar_c_m_marco(s_cpu_memoria_marco s_cpu_mem_marco)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = CPU_MEMORIA_OBTENER_MARCO;

    serializar_uint32_t(&paquete->buffer, s_cpu_mem_marco.pid); // 1. PID
    serializar_uint32_t(&paquete->buffer, s_cpu_mem_marco.pag); // 2. Pag

    return paquete;
}

s_cpu_memoria_marco deserializar_c_m_marco(t_paquete *paquete)
{
    assert(paquete->codigo_operacion == CPU_MEMORIA_OBTENER_MARCO);
    s_cpu_memoria_marco s_cpu_mem_marco;

    deserializar_uint32_t(paquete->buffer, &s_cpu_mem_marco.pid); // 1. PID
    deserializar_uint32_t(paquete->buffer, &s_cpu_mem_marco.pag); // 2. Pag

    return s_cpu_mem_marco;
}
