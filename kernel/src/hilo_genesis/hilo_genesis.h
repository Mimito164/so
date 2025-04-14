#ifndef HILO_GENESIS_H_
#define HILO_GENESIS_H_

#include "admision/admision.h"
#include "comunicacion_memoria/send.h"            // para hacer el send a memoria
#include "handlers/colas_estados/colas_handler.h" // para agregar a la lista new
#include <handlers/pcb_handler/pcb_handler.h>     // para crear el pcb

#include <pthread.h>

extern bool send_Kernel_memoria_creacion_call;
extern bool funcion_hilo_admision_call;

/// @brief Es una funcion que se llama cuando se ejecuta la instruccion `INICIAR_PROCESO`.
/// @attention No es bloqueante pero crea un hilo se queda bloqueado si alguien usa la lista new.
/// @brief Esta funcion se encarga de iniciar un proceso (duh).\
/// @brief Se encarga de:
/// @brief - mandar el mensaje a memoria con el path al pseudocodigo
/// @brief - crear un nuevo pcb
/// @brief - agregarlo a la lista new
/// @param path_pseudocodigo es la direccion al pseudocodigo del proceso.
void iniciar_proceso(char *path_pseudocodigo);

/// @brief Se encarga de esperar a que nadie use la lista de new (se bloquea) para
/// luego agregar un PCB pasado por argumento.
/// @param pcb es el pcb que se desea agregar al la lista de new.
void *hilo_genesis(void *pcb);

#endif