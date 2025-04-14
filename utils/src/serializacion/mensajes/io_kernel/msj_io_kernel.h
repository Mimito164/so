#ifndef MSJ_IO_KERNEL_H_
#define MSJ_IO_KERNEL_H_

#include "assert.h"
#include "serializacion/serializacion.h"

typedef struct
{
    char *nombreDeIO;
    tipo_IO tipo;
} s_io_kernel_iniciar_conexion;

t_paquete *serializar_io_kernel_INICIAR_CONEXION(s_io_kernel_iniciar_conexion s_io_kernel_init_conec);
s_io_kernel_iniciar_conexion deserializar_io_kernel_INICIAR_CONEXION(t_paquete *paqueteDeserializar);

t_paquete *serializar_kernel_io_KERNEL_IO_GEN_ZZZ(int cantidadDeUnidadesDeTrabajo);
uint32_t deserializar_kernel_io_KERNEL_IO_GEN_ZZZ(t_paquete *paqueteDeserializar);

#endif