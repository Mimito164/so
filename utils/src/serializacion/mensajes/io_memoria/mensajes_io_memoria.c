#include "mensajes_io_memoria.h"

t_paquete *serializar_io_memoria_write(s_io_memoria_write estructura)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = IO_MEMORIA_WRITE;

    serializar_uint32_t(&paquete->buffer, estructura.pid);          // 1 PID
    serializar_uint32_t(&paquete->buffer, estructura.real_mem_dir); // 2 DIRECCION FISICA
    serializar_uint32_t(&paquete->buffer, estructura.size);         // 3 SIZE
    return paquete;
}

s_io_memoria_write deserializar_io_memoria_write(t_paquete *paquete)
{
    s_io_memoria_write estructura;
    paquete->codigo_operacion = IO_MEMORIA_WRITE;

    deserializar_uint32_t(paquete->buffer, &estructura.pid);          // 1 PID
    deserializar_uint32_t(paquete->buffer, &estructura.real_mem_dir); // 2 DIRECCION FISICA
    deserializar_uint32_t(paquete->buffer, &estructura.size);         // 3 SIZE

    return estructura;
}

t_paquete *serializar_io_memoria_read(s_io_memoria_read estructura)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = IO_MEMORIA_READ;

    serializar_uint32_t(&paquete->buffer, estructura.pid);          // 1 pid
    serializar_uint32_t(&paquete->buffer, estructura.size);         // 2 size
    serializar_uint32_t(&paquete->buffer, estructura.real_mem_dir); // 3 mem dir
    serializar_string(&paquete->buffer, estructura.data);           // 4 data
    return paquete;
}

s_io_memoria_read deserializar_io_memoria_read(t_paquete *paquete)
{
    assert(paquete->codigo_operacion == IO_MEMORIA_READ);
    s_io_memoria_read estructura;
    deserializar_uint32_t(paquete->buffer, &estructura.pid);          // 1 pid
    deserializar_uint32_t(paquete->buffer, &estructura.size);         // 2 size
    deserializar_uint32_t(paquete->buffer, &estructura.real_mem_dir); // 3 mem dir
    deserializar_string(paquete->buffer, &estructura.data);           // 4 data

    return estructura;
}