#ifndef MENSAJE_IO_MEMORIA_H_
#define MENSAJE_IO_MEMORIA_H_

#include "assert.h"
#include "serializacion/serializacion.h"

// IO_MEMORIA_WRITE
typedef struct
{
    int pid;
    int real_mem_dir;
    int size;
} s_io_memoria_write;

t_paquete *serializar_io_memoria_write(s_io_memoria_write estructura);
s_io_memoria_write deserializar_io_memoria_write(t_paquete *paquete);

// IO_MEMORIA_READ
typedef struct
{
    uint32_t pid;
    uint32_t real_mem_dir;
    uint32_t size;
    char *data;
} s_io_memoria_read;

t_paquete *serializar_io_memoria_read(s_io_memoria_read estructura);
s_io_memoria_read deserializar_io_memoria_read(t_paquete *paquete);

#endif