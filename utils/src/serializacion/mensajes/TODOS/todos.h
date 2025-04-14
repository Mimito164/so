#ifndef TODOS_H_
#define TODOS_H_

#include <TAD/TAD.h>
#include <serializacion/serializacion.h>

void serializar_elem_list_frames(t_buffer **buffer, frames_element *frames_element_pkt);
frames_element *deserializar_elem_list_frames(t_buffer *buffer);
void serializar_frames_element_fs(void **b, void *pkt);
void *deserializar_frames_element_fs(t_buffer *buffer);

#endif