#ifndef MENSAJES_CPU_H_
#define MENSAJES_CPU_H_

#include <globals/globals.h>
#include <serializacion/serializacion.h>
#include <sockets/sockets.h>
#include <t_config/t_config.h>

void send_instruccion(int file_descriptor, char *instruccion);
void send_tam_pag(int file_descriptor);
void send_ok(int file_descriptor);
void send_out_of_memory(int file_descriptor);
void send_frame(int file_descriptor, int frame);
void send_bytes(int file_descriptor, uint8_t bytes_to_send);

#endif