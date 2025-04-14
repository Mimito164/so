#ifndef INICIAR_MEMORIA_USUARIO_H_
#define INICIAR_MEMORIA_USUARIO_H_

#include "globals/globals.h"
#include "serializacion/mensajes/cpu_memoria/msj_cpu_memoria.h"
#include "t_config/t_config.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void instanciate_user_mem();

void destroy_user_mem();

/// @brief Get a value from the user memory
/// @param position the initial position (phisical address) of the value to get
/// @param size the size of the value to get
void *get_val_from_usermem(uint32_t position, uint32_t size);

/// @brief Sets a value in the user memory, in a specific position
/// @param value the value to save in memory position (phisical address) of the value to get
/// @param position the initial position (phisical address) of the value to get
/// @param size the size of the value to set
void set_val_to_usermem(void *value, uint32_t position, uint32_t size);

/// @brief Get value from memory from various frames
/// @param list t_list de frames_element para poder iterar en todas las direcciones
/// @param total_size tamaño total de los valores que se quieren obtener
/// @return `char *` La colleción de bytes obtenidos. Usa char * y no void * para poder debuggear
char *get_val_from_usermem_iterator(t_list *list, uint32_t total_size);

/// @brief Sets values into memory across multiple frames
/// @param list t_list de frames_element para poder iterar por todas las direcciones
/// @param val el valor que se quiere guardar en memoria
void set_val_into_usermem_iterator(t_list *list, char *val);

/// @brief Función de debug para imprimir toda la memoria de usuario
void print_user_mem();

#endif