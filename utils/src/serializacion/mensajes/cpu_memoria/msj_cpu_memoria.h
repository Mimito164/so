#ifndef MSJ_CPU_MEMORIA_H_
#define MSJ_CPU_MEMORIA_H_

#include "commons/collections/list.h"
#include "serializacion/serializacion.h"
#include <assert.h>

typedef struct
{
    uint32_t pid;
    uint32_t size;
    t_list *source;
    t_list *destiny;
} s_cpu_memoria_copy_string;

/// @brief serializa el mensaje de copy string
/// @param s_c_m_copy_string
/// @return el paquete serializado
t_paquete *serializar_c_m_copy_string(s_cpu_memoria_copy_string s_c_m_copy_string);

/// @brief deserializa un paquete de copy string
/// @param paquete
/// @return la struct con la información del mensaje
s_cpu_memoria_copy_string deserializar_c_m_copy_string(t_paquete *paquete);

typedef struct
{
    uint32_t pid;
    uint32_t pag;
} s_cpu_memoria_marco;

/// @brief serializa el mensaje de "get frame" (obtener marco)
/// @param s_mem_cpu_marco
/// @return el paquete serializado del mensaje
t_paquete *serializar_c_m_marco(s_cpu_memoria_marco s_mem_cpu_marco);

/// @brief deserializa el mensaje de "get frame" (obtener marco)
/// @param paquete
/// @return la struct con la información del mensaje
s_cpu_memoria_marco deserializar_c_m_marco(t_paquete *paquete);

#endif