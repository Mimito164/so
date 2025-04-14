#ifndef MENSAJES_H_
#define MENSAJES_H_

#include "../../kernel/src/globals/globals.h"
#include <serializacion/serializacion.h>

#include <TAD/TAD.h>

typedef struct
{
    uint32_t pid;
    char *path;
} s_kernel_memoria_creacion_proceso;

s_kernel_memoria_creacion_proceso create_k_m_creacion_proceso(uint32_t pid, char *pseudocodigo_file_path);
s_kernel_memoria_creacion_proceso deserializar_k_m_creacion_proceso(t_paquete *paquete);

t_paquete *serializar_k_m_creacion_proceso(s_kernel_memoria_creacion_proceso s_k_m_creacion_pros);
uint32_t deserializar_k_m_finalizacion_proceso(t_paquete *paquete);

t_paquete *finalizar_proceso_memoria(uint32_t pid);

#endif