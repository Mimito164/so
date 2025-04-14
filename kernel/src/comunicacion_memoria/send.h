#ifndef MENSAJES_SEND_H_
#define MENSAJES_SEND_H_

#include "../../kernel/src/globals/globals.h"
#include "iniciar_comunicacion.h"
#include <serializacion/mensajes/kernel_memoria/mensajes_kernel_memoria.h>
#include <serializacion/serializacion.h>

#include <TAD/TAD.h>

void send_Kernel_memoria_creacion(uint32_t pid, char *pseudocodigo_file_path);
void send_Kernel_memoria_finalizacion(uint32_t pid);

#endif