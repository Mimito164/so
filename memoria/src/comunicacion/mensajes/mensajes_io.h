#ifndef MENSAJES_IO_H_
#define MENSAJES_IO_H_

#include "serializacion/mensajes/memoria_io/msj_memoria_io.h"
#include <TAD/TAD.h>
#include <sockets/sockets.h>

/// @brief Envia una cadena de bites a la io
/// @param file_descriptor el fd al cual le vamos a enviar los bytes
/// @param bytes_to_send los bytes a enviar
/// @param opcode el opcode que necesita el paquete
void send_m_i_write(int file_descriptor, uint32_t position, uint32_t size, char *value);

#endif