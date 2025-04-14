#include "msj_io_kernel.h"

t_paquete *serializar_io_kernel_INICIAR_CONEXION(s_io_kernel_iniciar_conexion s_io_kernel_init_conec)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = IO_KERNEL_INICIAR_CONEXION;
    serializar_string(&paquete->buffer, s_io_kernel_init_conec.nombreDeIO);       // 1. nombre
    serializar_uint32_t(&paquete->buffer, (uint32_t)s_io_kernel_init_conec.tipo); // 2. Tipo

    return paquete;
}

s_io_kernel_iniciar_conexion deserializar_io_kernel_INICIAR_CONEXION(t_paquete *paqueteDeserializar)
{
    s_io_kernel_iniciar_conexion s_io_kernel_init_conec;
    deserializar_string(paqueteDeserializar->buffer, &s_io_kernel_init_conec.nombreDeIO); // 1. nombre
    deserializar_uint32_t(paqueteDeserializar->buffer, &s_io_kernel_init_conec.tipo);     // 2. tipo
    paquete_destroy(paqueteDeserializar);
    return s_io_kernel_init_conec;
}

// t_paquete *serializar_kernel_io_KERNEL_IO_GEN_ZZZ(int cantidadDeUnidadesDeTrabajo)
// {
//     t_paquete *paquete = paquete_new();
//     paquete->codigo_operacion = KERNEL_IO_GEN_ZZZ;
//     serializar_uint32_t(&paquete->buffer, cantidadDeUnidadesDeTrabajo);
//     return paquete;
// }

// uint32_t deserializar_kernel_io_KERNEL_IO_GEN_ZZZ(t_paquete *paqueteDeserializar)
// {
//     uint32_t cantidadDeUnidadesDeTrabajo;
//     deserializar_uint32_t(paqueteDeserializar->buffer, &cantidadDeUnidadesDeTrabajo);
//     paquete_destroy(paqueteDeserializar);
//     return cantidadDeUnidadesDeTrabajo;
// }