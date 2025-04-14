#include "msj_memoria_cpu.h"

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_memoria_cpu_resize create_mem_cpu_resize_struct(uint32_t pid, uint32_t size)
{
    s_memoria_cpu_resize s_mem_cpu_resize;
    s_mem_cpu_resize.pid = pid;
    s_mem_cpu_resize.size = size;

    return s_mem_cpu_resize;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_memoria_cpu_instruccion create_mem_cpu_instruccion_struct(uint32_t pid, uint32_t program_counter)
{
    s_memoria_cpu_instruccion s_mem_cpu_instruccion;
    s_mem_cpu_instruccion.pid = pid;
    s_mem_cpu_instruccion.program_counter = program_counter;

    return s_mem_cpu_instruccion;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_memoria_cpu_marco create_mem_cpu_marco_struct(uint32_t pid, uint32_t pag)
{
    s_memoria_cpu_marco s_mem_cpu_marco;
    s_mem_cpu_marco.pid = pid;
    s_mem_cpu_marco.pag = pag;

    return s_mem_cpu_marco;
}

s_cpu_mem_movin create_cpu_mem_movin(uint32_t pid, uint32_t address, uint32_t size)
{
    s_cpu_mem_movin estructura = {.pid = pid, .address = address, .size = size};
    return estructura;
}

s_cpu_mem_movout create_cpu_mem_movout(uint32_t pid, uint32_t address, uint32_t size, uint8_t data)
{
    s_cpu_mem_movout estructura = {.pid = pid, .address = address, .size = size, .data = data};
    return estructura;
}

s_memoria_cpu_resize deserializar_m_c_resize_struct(t_paquete *paquete)
{
    s_memoria_cpu_resize s_mem_cpu_resize;

    deserializar_uint32_t(paquete->buffer, &s_mem_cpu_resize.pid);
    deserializar_uint32_t(paquete->buffer, &s_mem_cpu_resize.size);

    return s_mem_cpu_resize;
}

s_memoria_cpu_instruccion deserializar_m_c_instruccion_struct(t_paquete *paquete)
{
    s_memoria_cpu_instruccion s_mem_cpu_instruccion;

    deserializar_uint32_t(paquete->buffer, &s_mem_cpu_instruccion.pid);
    deserializar_uint32_t(paquete->buffer, &s_mem_cpu_instruccion.program_counter);

    return s_mem_cpu_instruccion;
}

s_memoria_cpu_marco deserializar_m_c_marco_struct(t_paquete *paquete)
{
    s_memoria_cpu_marco s_mem_cpu_marco;

    deserializar_uint32_t(paquete->buffer, &s_mem_cpu_marco.pid);
    deserializar_uint32_t(paquete->buffer, &s_mem_cpu_marco.pag);

    return s_mem_cpu_marco;
}

s_cpu_mem_movin deserializar_c_m_movin(t_paquete *paquete)
{
    s_cpu_mem_movin estructura;
    deserializar_uint32_t(paquete->buffer, &estructura.pid);
    deserializar_uint32_t(paquete->buffer, &estructura.address);
    deserializar_uint32_t(paquete->buffer, &estructura.size);
    return estructura;
}

s_cpu_mem_movout deserializar_c_m_movout(t_paquete *paquete)
{
    s_cpu_mem_movout estructura;
    deserializar_uint32_t(paquete->buffer, &estructura.pid);
    deserializar_uint32_t(paquete->buffer, &estructura.address);
    deserializar_uint32_t(paquete->buffer, &estructura.size);
    deserializar_uint8_t(paquete->buffer, &estructura.data);
    return estructura;
}

t_paquete *serializar_m_c_ok()
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = OK_GRAL;

    return paquete;
}

t_paquete *serializar_m_c_out_of_memory()
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = MEMORIA_CPU_OUT_OF_MEMORY;

    return paquete;
}

// Serializacion desde CPU // esto esta mal, este archivo es memoria → cpu, y esta funcion es cpu → memoria
t_paquete *serializar_c_m_resize(s_memoria_cpu_resize s_mem_cpu_resize)

{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = CPU_MEMORIA_RESIZE;

    serializar_uint32_t(&paquete->buffer, s_mem_cpu_resize.pid);
    serializar_uint32_t(&paquete->buffer, s_mem_cpu_resize.size);

    return paquete;
}

t_paquete *serializar_c_m_instruccion(s_memoria_cpu_instruccion s_mem_cpu_instruccion)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = CPU_MEMORIA_OBTENER_INSTRUCCIONES;

    serializar_uint32_t(&paquete->buffer, s_mem_cpu_instruccion.pid);
    serializar_uint32_t(&paquete->buffer, s_mem_cpu_instruccion.program_counter);

    return paquete;
}

// t_paquete *serializar_c_m_marco(s_memoria_cpu_marco s_mem_cpu_marco)
// {
//     t_paquete *paquete = paquete_new();
//     paquete->codigo_operacion = CPU_MEMORIA_OBTENER_MARCO;

//     serializar_uint32_t(&paquete->buffer, s_mem_cpu_marco.pid);
//     serializar_uint32_t(&paquete->buffer, s_mem_cpu_marco.pag);

//     return paquete;
// }
//    return paquete;
// }

t_paquete *serializar_c_m_movin(s_cpu_mem_movin s_c_m_movin)
{
    t_paquete *pkg = paquete_new();
    serializar_uint32_t(&pkg->buffer, s_c_m_movin.pid);
    serializar_uint32_t(&pkg->buffer, s_c_m_movin.address);
    serializar_uint32_t(&pkg->buffer, s_c_m_movin.size);
    return pkg;
}

t_paquete *serializar_c_m_movout(s_cpu_mem_movout s_c_m_movout)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_MEMORIA_MOV_OUT;
    serializar_uint32_t(&pkg->buffer, s_c_m_movout.pid);
    serializar_uint32_t(&pkg->buffer, s_c_m_movout.address);
    serializar_uint32_t(&pkg->buffer, s_c_m_movout.size);
    serializar_uint8_t(&pkg->buffer, s_c_m_movout.data);
    return pkg;
}

// Serializacion desde CPU
// t_paquete *serializar_c_m_copy_string(s_memoria_cpu_copy_string s_m_c_copy_string)
// {
//     t_paquete *paquete = paquete_new();
//     paquete->codigo_operacion = CPU_MEMORIA_COPIAR_STRING;

//     serializar_uint32_t(&paquete->buffer, s_m_c_copy_string.pid);     // 1. PID
//     serializar_uint32_t(&paquete->buffer, s_m_c_copy_string.size);    // 2. Size
//     serializar_uint32_t(&paquete->buffer, s_m_c_copy_string.source);  // 3. Source
//     serializar_uint32_t(&paquete->buffer, s_m_c_copy_string.destiny); // 4. Destiny

//     return paquete;
// }

// s_memoria_cpu_copy_string deserializar_c_m_copy_string(t_paquete *paquete)
// {
//     s_memoria_cpu_copy_string s_m_c_copy_string;
//     assert(paquete->codigo_operacion == CPU_MEMORIA_COPIAR_STRING);

//     deserializar_uint32_t(paquete->buffer, &s_m_c_copy_string.pid);     // 1. PID
//     deserializar_uint32_t(paquete->buffer, &s_m_c_copy_string.size);    // 2. Size
//     deserializar_uint32_t(paquete->buffer, &s_m_c_copy_string.source);  // 3. Source
//     deserializar_uint32_t(paquete->buffer, &s_m_c_copy_string.destiny); // 4. Destiny

//     return s_m_c_copy_string;
// }
