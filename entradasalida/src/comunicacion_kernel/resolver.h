#ifndef RESOLVER_H_
#define RESOLVER_H_

#include "globals/globals.h"
#include <commons/collections/list.h>
#include <commons/string.h>
#include <estructuras/estructuras.h>
#include <math.h>
#include <serializacion/mensajes/io_kernel/msj_io_kernel.h>
#include <serializacion/mensajes/io_memoria/mensajes_io_memoria.h>
#include <serializacion/mensajes/kernel_io/msj_kernel_io.h>
#include <serializacion/mensajes/memoria_io/msj_memoria_io.h>
#include <unistd.h>

extern t_paquete *paqueteEnviarMemoria_test;

t_paquete *resolver_KERNEL_IO_GEN_ZZZ(t_paquete *paquete_reciv);
t_paquete *resolver_KERNEL_IO_STDIN_READ(t_paquete *paquete_reciv);
t_paquete *resolver_KERNEL_IO_STDOUT_WRITE(t_paquete *paquete_reciv);
t_paquete *resolver_KERNEL_IO_FS_CREATE(t_paquete *paquete_reciv);
t_paquete *resolver_KERNEL_IO_FS_DELETE(t_paquete *paquete_reciv);
t_paquete *resolver_KERNEL_IO_FS_TRUNCATE(t_paquete *paquete_reciv);
t_paquete *resolver_KERNEL_IO_FS_READ(t_paquete *paquete_reciv);
t_paquete *resolver_KERNEL_IO_FS_WRITE(t_paquete *paquete_reciv);

#endif