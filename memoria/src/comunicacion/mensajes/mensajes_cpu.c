#include "mensajes_cpu.h"

void send_instruccion(int file_descriptor, char *instruccion)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = MEMORIA_CPU_OBTENER_INSTRUCCIONES;
    serializar_string(&paquete->buffer, instruccion);
    send_paquete(file_descriptor, paquete);
}

void send_tam_pag(int file_descriptor)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = MEMORIA_CPU_TAM_PAG;
    serializar_uint32_t(&paquete->buffer, get_config()->TAM_PAGINA);
    send_paquete(file_descriptor, paquete);
}

void send_ok(int file_descriptor)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = OK_GRAL;
    send_paquete(file_descriptor, paquete);
}

void send_out_of_memory(int file_descriptor)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = MEMORIA_CPU_OUT_OF_MEMORY;
    send_paquete(file_descriptor, paquete);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters
void send_frame(int file_descriptor, int frame)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = MEMORIA_CPU_OBTENER_MARCO;
    serializar_uint32_t(&paquete->buffer, (uint32_t)frame);
    send_paquete(file_descriptor, paquete);
}

void send_bytes(int file_descriptor, uint8_t byte_to_send)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = MEMORIA_CPU_MOV_IN;
    serializar_uint8_t(&pkg->buffer, byte_to_send);
    send_paquete(file_descriptor, pkg);
}
