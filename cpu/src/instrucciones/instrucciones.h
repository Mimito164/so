#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

#include <TAD/TAD.h>
#include <commons/collections/dictionary.h>
#include <comunicacion_kernel/contextoEjecucionHandlers/contextoEjecucionHandlers.h>
#include <comunicacion_kernel/mensajes/mensajes.h>
#include <comunicacion_memoria/mensajes/mensajes_a_memoria.h>
#include <diccionarios/diccionario_registros.h>
#include <globals/globals.h>
#include <mmu/mmu.h>
#include <serializacion/mensajes/cpu_kernel/msj_cpu_kernel.h>
#include <serializacion/serializacion.h>

//  NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
t_paquete *set(char *argumentos);
t_paquete *sum(char *argumentos);
t_paquete *sub(char *argumentos);
t_paquete *jnz(char *argumentos);
t_paquete *gen_io_sleep(char *argumentos);
t_paquete *nuestro_wait(char *argumentos);
t_paquete *nuestro_signal(char *argumentos);
t_paquete *resize(char *argumentos);
t_paquete *mov_in(char *argumentos);
t_paquete *mov_out(char *argumentos);
t_paquete *io_fs_truncate(char *argumentos);
t_paquete *io_fs_delete(char *argumentos);
t_paquete *io_fs_create(char *argumentos);
t_paquete *io_stdout_write(char *argumentos);
t_paquete *io_stdin_read(char *argumentos);
t_paquete *copy_string(char *argumentos);
t_paquete *io_fs_write(char *argumentos);
t_paquete *io_fs_read(char *argumentos);

#endif