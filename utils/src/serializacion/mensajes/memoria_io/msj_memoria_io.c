#include "msj_memoria_io.h"

s_memoria_io_write create_mem_io_write_struct(uint32_t size, void *value, uint32_t position)
{
    s_memoria_io_write s_mem_io_write;
    s_mem_io_write.size = size;
    s_mem_io_write.position = position;
    s_mem_io_write.value = value;

    return s_mem_io_write;
}

/// @brief Respuesta a la petición de escribir en un valor en una dirección
/// @return OK or NOTOK
mem_io_status deserializar_m_i_read(t_paquete *paquete)
{
    assert(paquete->codigo_operacion == MEMORIA_IO_READ); // esto está para debugear, no es runtime check
    mem_io_status status;
    uint32_t _status;

    deserializar_uint32_t(paquete->buffer, &_status);

    if (_status == 1)
    {
        status = OK;
    }
    else
    {
        status = NOTOK;
    }

    return status;
}

/// @brief Respuesta a la petición de escribir un valor en una dirección
/// @return OK or NOTOK
t_paquete *serializar_m_i_read(mem_io_status status)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = MEMORIA_IO_READ;

    serializar_uint32_t(&pkg->buffer, (uint32_t)status);

    return pkg;
};

/// @brief Serializar la respuesta al pedido de un valor alojado en una dirección de memoria
/// @return `t_paquete` con el sig. formato: 1.`uint32_t`size 2.`uint32_t`position 3.`char *`value
t_paquete *serializar_m_i_write_struct(s_memoria_io_write s_mem_io_write)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = MEMORIA_IO_WRITE;

    serializar_uint32_t(&pkg->buffer, s_mem_io_write.size);     // 1. Size
    serializar_uint32_t(&pkg->buffer, s_mem_io_write.position); // 2. Position
    char *aux = (char *)s_mem_io_write.value;
    aux[s_mem_io_write.size] = '\0';
    serializar_string(&pkg->buffer, (char *)s_mem_io_write.value); // 3. Value

    return pkg;
}

/// @brief Deserializar la respuesta al pedido de un valor alojado en una dirección de memoria
/// @return `s_memoria_io_write` que contiene: size, position y value
s_memoria_io_write deserializar_m_i_write_struct(t_paquete *paquete)
{
    assert(paquete->codigo_operacion == MEMORIA_IO_WRITE); // esto está para debugear, no es runtime check
    s_memoria_io_write s_mem_io_write;

    deserializar_uint32_t(paquete->buffer, &s_mem_io_write.size);         // 1. Size
    deserializar_uint32_t(paquete->buffer, &s_mem_io_write.position);     // 2. Position
    deserializar_string(paquete->buffer, (char **)&s_mem_io_write.value); // 3. Value

    return s_mem_io_write;
}
