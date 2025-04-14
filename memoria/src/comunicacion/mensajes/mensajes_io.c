#include "mensajes_io.h"
#include "globals/globals.h"

void send_m_i_write(int file_descriptor, uint32_t position, uint32_t size, char *value)
{
    // log_error(get_logger(), "le enviamos a IO: %s", value);
    // log_error(get_logger(), "con el size: %i", size);
    s_memoria_io_write s_mem_io_write = {.position = position, .size = size, .value = value};

    t_paquete *pkg = serializar_m_i_write_struct(s_mem_io_write);
    send_paquete(file_descriptor, pkg);
}
