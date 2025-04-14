#ifndef MSJ_MEMORIA_IO_H_
#define MSJ_MEMORIA_IO_H_

#include "assert.h"
#include "serializacion/serializacion.h"

// MEMORIA_IO_READ
typedef enum
{
    NOTOK,
    OK
} mem_io_status;

mem_io_status deserializar_m_i_read(t_paquete *paquete);
t_paquete *serializar_m_i_read(mem_io_status status);

// MEMORIA_IO_WRITE
typedef struct
{
    uint32_t size;
    uint32_t position;
    void *value;
} s_memoria_io_write;

s_memoria_io_write create_mem_io_write_struct(uint32_t size, void *value, uint32_t position);

s_memoria_io_write deserializar_m_i_write_struct(t_paquete *paquete);
t_paquete *serializar_m_i_write_struct(s_memoria_io_write s_mem_io_write);

#endif