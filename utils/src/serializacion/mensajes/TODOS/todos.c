#include "todos.h"

void serializar_elem_list_frames(t_buffer **buffer, frames_element *frames_element_pkt)
{
    // t_buffer **buffer = (t_buffer **)b;
    // frames_element *frames_element_pkt = (frames_element *)pkt;

    serializar_uint32_t(buffer, frames_element_pkt->pid);     // 1. pid
    serializar_uint32_t(buffer, frames_element_pkt->offset);  // 2. offset
    serializar_uint32_t(buffer, frames_element_pkt->padding); // 3. padding
    serializar_uint32_t(buffer, frames_element_pkt->frame);   // 4. frame
    serializar_uint32_t(buffer, frames_element_pkt->tam_pag); // 5. tamaño de pag

    // serializar_string(buffer, frames_element_pkt->data);      // 6. data (a priori lo evitamos)
}

frames_element *deserializar_elem_list_frames(t_buffer *buffer)
{
    // t_buffer *buffer2 = (t_buffer *)buffer;
    frames_element *frames_element_pkt = malloc(sizeof(frames_element));

    deserializar_uint32_t(buffer, &frames_element_pkt->pid);     // 1. pid
    deserializar_uint32_t(buffer, &frames_element_pkt->offset);  // 2. offset
    deserializar_uint32_t(buffer, &frames_element_pkt->padding); // 3. padding
    deserializar_uint32_t(buffer, &frames_element_pkt->frame);   // 4. frame
    deserializar_uint32_t(buffer, &frames_element_pkt->tam_pag); // 5. tamaño de pag

    // deserializar_string(buffer, &frames_element_pkt->data);      // 6. data (a priori lo evitamos)

    return frames_element_pkt;
}

void serializar_frames_element_fs(void **b, void *pkt)
{
    t_buffer **buffer = (t_buffer **)b;
    frames_element *frames_element_pkt = (frames_element *)pkt;

    serializar_uint32_t(buffer, frames_element_pkt->pid);     // 1. pid
    serializar_uint32_t(buffer, frames_element_pkt->offset);  // 2. offset
    serializar_uint32_t(buffer, frames_element_pkt->padding); // 3. padding
    serializar_uint32_t(buffer, frames_element_pkt->frame);   // 4. frame
    serializar_uint32_t(buffer, frames_element_pkt->tam_pag); // 5. tamaño de pag (creo q io no lo necesita)
};

void *deserializar_frames_element_fs(t_buffer *buffer)
{
    frames_element *frames_element_pkt = malloc(sizeof(frames_element));

    deserializar_uint32_t(buffer, &frames_element_pkt->pid);     // 1. pid
    deserializar_uint32_t(buffer, &frames_element_pkt->offset);  // 2. offset
    deserializar_uint32_t(buffer, &frames_element_pkt->padding); // 3. padding
    deserializar_uint32_t(buffer, &frames_element_pkt->frame);   // 4. frame
    deserializar_uint32_t(buffer, &frames_element_pkt->tam_pag); // 5. tamaño de pag (creo q io no lo necesita)

    return (void *)frames_element_pkt;
};