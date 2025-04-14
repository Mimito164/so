#ifndef MENSAJES_H_
#define MENSAJES_H_

#include <globals/globals.h>
#include <serializacion/serializacion.h>

#include "comunicacion_kernel/contextoEjecucionHandlers/contextoEjecucionHandlers.h"
#include "comunicacion_kernel/interruptHandlers/interrupt_handlers.h"

void cargar_contexto_ejecucion(t_contexto_ejecucion temp_contexto);

void cargar_interrupcion(t_interrupt temp_interrupt);

void send_kill_proceso(int fd_kernel);
void send_desalojo_finq(int fd_kernel, t_contexto_ejecucion ctx);
void send_desalojo_signal(int fd_kernel, t_contexto_ejecucion ctx,
                          char *recurso); // necesita el nombre del  recurso libera
void send_desalojo_wait(int fd_kernel, t_contexto_ejecucion ctx,
                        char *recurso); // necesita el nombre del  recurso libera
void send_cpu_kernel_io_sleep(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, uint32_t unidades_trabajo);
void send_desalojo_io_stdin_read(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, uint32_t direccion_traducida,
                                 uint32_t tamano);

void send_desalojo_io_stdout_write(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, uint32_t direccion_traducida,
                                   uint32_t tamano);

void send_desalojo_io_fs_create(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo);
void send_desalojo_io_fs_delete(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo);

void send_desalojo_io_fs_truncate(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo,
                                  uint32_t tamano);

void send_desalojo_io_fs_write(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo,
                               t_list *direccion_traducida, uint32_t tamano, uint32_t puntero_archivo);

void send_desalojo_io_fs_read(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo,
                              t_list *direccion_traducida, uint32_t tamano, uint32_t puntero_archivo);
t_paquete *crear_paquete_resize();

#endif
