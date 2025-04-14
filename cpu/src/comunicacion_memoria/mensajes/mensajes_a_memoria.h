#ifndef MENSAJES_A_MEMORIA_H_
#define MENSAJES_A_MEMORIA_H_

#include <comunicacion_kernel/contextoEjecucionHandlers/contextoEjecucionHandlers.h>
#include <comunicacion_memoria/iniciar_comunicacion.h>
#include <globals/globals.h>
#include <mmu/mmu.h>
#include <serializacion/mensajes/cpu_memoria/msj_cpu_memoria.h>
#include <serializacion/mensajes/memoria_cpu/msj_memoria_cpu.h>
#include <serializacion/serializacion.h>
#include <sockets/sockets.h>

void send_obtener_instruccion(uint32_t pid, uint32_t program_counter);
char *recv_instruccion();
char *obtener_instruccion(uint32_t pid, uint32_t program_counter);
void obtener_tam_pag();
void send_resize(uint32_t pid, uint32_t tamanio);
bool recv_resize();
uint32_t obtener_frame(uint32_t pid, uint32_t pagina);
void send_mov_in(uint32_t addres, uint32_t size);
void send_mov_out(uint32_t addres, uint32_t size, uint8_t data);
uint8_t recv_mov_in();
bool send_copy_string(uint32_t pid, t_list *source, t_list *destiny, uint32_t size);

#endif