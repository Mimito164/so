#ifndef MSJ_MEMORIA_CPU_H_
#define MSJ_MEMORIA_CPU_H_

#include "serializacion/serializacion.h"
#include <assert.h>

typedef struct
{
    uint32_t pid;
    uint32_t size;
    uint32_t source;
    uint32_t destiny;
} s_memoria_cpu_copy_string;
typedef struct
{
    uint32_t pid;
    uint32_t size;
} s_memoria_cpu_resize;

typedef struct
{
    uint32_t pid;
    uint32_t program_counter;
} s_memoria_cpu_instruccion;

typedef struct
{
    uint32_t pid;
    uint32_t pag;
} s_memoria_cpu_marco;

typedef struct
{
    uint32_t pid;
    uint32_t address;
    uint32_t size;
} s_cpu_mem_movin;

typedef struct
{
    uint32_t pid;
    uint32_t address;
    uint32_t size;
    uint8_t data;
} s_cpu_mem_movout;

// Desde CPU
s_memoria_cpu_resize create_mem_cpu_resize_struct(uint32_t pid, uint32_t size);
s_memoria_cpu_instruccion create_mem_cpu_instruccion_struct(uint32_t pid, uint32_t program_counter);
s_memoria_cpu_marco create_mem_cpu_marco_struct(uint32_t pid, uint32_t pag);
s_cpu_mem_movin create_cpu_mem_movin(uint32_t pid, uint32_t address, uint32_t size);
s_cpu_mem_movout create_cpu_mem_movout(uint32_t pid, uint32_t address, uint32_t size, uint8_t data);

// Desde CPU ← TODO(all): esto está mal, no es el archivo correcto
t_paquete *serializar_c_m_resize(s_memoria_cpu_resize s_mem_cpu_resize);
t_paquete *serializar_c_m_instruccion(s_memoria_cpu_instruccion s_mem_cpu_instruccion);
// t_paquete *serializar_c_m_marco(s_memoria_cpu_marco s_mem_cpu_marco);
t_paquete *serializar_c_m_movin(s_cpu_mem_movin s_c_m_movin);
t_paquete *serializar_c_m_movout(s_cpu_mem_movout s_c_m_movout);

s_memoria_cpu_resize deserializar_m_c_resize_struct(t_paquete *paquete);
s_memoria_cpu_instruccion deserializar_m_c_instruccion_struct(t_paquete *paquete);
s_memoria_cpu_marco deserializar_m_c_marco_struct(t_paquete *paquete);
s_cpu_mem_movin deserializar_c_m_movin(t_paquete *paquete);
s_cpu_mem_movout deserializar_c_m_movout(t_paquete *paquete);

// Desde Memoria
t_paquete *serializar_m_c_ok();
t_paquete *serializar_m_c_out_of_memory();

#endif