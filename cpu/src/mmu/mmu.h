#ifndef MMU_H_
#define MMU_H_

#include "commons/collections/list.h"
#include "comunicacion_memoria/mensajes/mensajes_a_memoria.h"
#include "serializacion/mensajes/cpu_memoria/msj_cpu_memoria.h"
#include <TAD/TAD.h>
#include <TLB/TLB.h>
#include <globals/globals.h>
#include <math.h>
#include <stdint.h>
#include <t_config/t_config.h>

// uint32_t obtener_pagina(uint32_t direccion_logica);
// uint32_t obtener_desplazamiento(uint32_t direccion_logica, uint32_t pagina);

/// @brief Traduce direcciones logicas a fisicas. El tamaño le permite traducir más de una página-frame si es necesario
/// @param direccion_logica a traducir
/// @param size para saber si entra en uno o más frames
/// @return `t_list` de `frames_element`
t_list *traducir_memoria(uint32_t direccion_logica, uint32_t size, uint32_t pid);

#endif