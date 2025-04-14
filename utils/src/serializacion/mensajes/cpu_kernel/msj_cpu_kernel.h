#ifndef MSJ_CPU_KERNEL_H_
#define MSJ_CPU_KERNEL_H_

#include "../../cpu/src/comunicacion_kernel/interruptHandlers/interrupt_handlers.h"
#include "TAD/TAD.h"
#include "serializacion/serializacion.h"
#include <assert.h>
#include <serializacion/mensajes/TODOS/todos.h>

typedef char *args_signal;
typedef char *args_wait;
typedef struct
{
    char *nombre_io;
    uint32_t unidades_trabajo;
} args_io_sleep;
typedef struct
{
    char *nombre_io;
    uint32_t size;
    t_list *lista_frames;
} args_io_stdin;
typedef struct
{
    char *nombre_io;
    uint32_t size;
    t_list *lista_frames;
} args_io_stdout;
typedef struct
{
    char *nombre_io;
    char *nombre_archivo;
} args_io_fs_create;
typedef struct
{
    char *nombre_io;
    char *nombre_archivo;
} args_io_fs_delete;
typedef struct
{
    char *nombre_io;
    char *nombre_archivo;
    uint32_t tamano;
} args_io_fs_truncate;
typedef struct
{
    char *nombre_io;
    char *nombre_archivo;
    t_list *info_marcos;
    uint32_t tamano;
    uint32_t puntero_archivo;
} args_io_fs_write;
typedef struct
{
    char *nombre_io;
    char *nombre_archivo;
    t_list *info_marcos;
    uint32_t tamano;
    uint32_t puntero_archivo;
} args_io_fs_read;

typedef struct
{
    t_contexto_ejecucion contexto;

    union {
        args_signal args_signal;
        args_wait args_wait;
        args_io_sleep args_io_sleep;
        args_io_stdin args_io_stdin;
        args_io_stdout args_io_stdout;
        args_io_fs_create args_io_fs_create;
        args_io_fs_delete args_io_fs_delete;
        args_io_fs_truncate args_io_fs_truncate;
        args_io_fs_write args_io_fs_write;
        args_io_fs_read args_io_fs_read;
    } solicitud_recurso;

} s_cpu_kernel;

void serializar_contexto_ejecucion(t_buffer **buffer, t_contexto_ejecucion ctx);
t_contexto_ejecucion deserializar_contexto_ejecucion(t_paquete *pkg);

t_interrupt deserializar_interrupcion(t_paquete *pkg);

s_cpu_kernel create_s_cpu_kernel_signal(t_contexto_ejecucion ctx, char *recurso);
t_paquete *serializar_cpu_kernel_signal(s_cpu_kernel s_cpu_kernel_signal);
s_cpu_kernel deserializar_cpu_kernel_signal(t_paquete *pkg);

s_cpu_kernel create_s_cpu_kernel_wait(t_contexto_ejecucion ctx, char *recurso);
t_paquete *serializar_cpu_kernel_wait(s_cpu_kernel s_cpu_kernel_wait);
s_cpu_kernel deserializar_cpu_kernel_wait(t_paquete *pkg);

s_cpu_kernel create_s_cpu_kernel_io_sleep(t_contexto_ejecucion ctx, char *io_name, uint32_t unidades_trabajo);
t_paquete *serializar_cpu_kernel_io_sleep(s_cpu_kernel s_cpu_kernel_io_sleep);
s_cpu_kernel deserializar_cpu_kernel_io_sleep(t_paquete *pkg);

s_cpu_kernel create_s_cpu_kernel_io_stdin(t_contexto_ejecucion ctx, char *nombreIO, t_list *lista_frames,
                                          uint32_t size);
t_paquete *serializar_cpu_kernel_io_stdin(s_cpu_kernel s_cpu_kernel_io_stdin);
s_cpu_kernel deserializar_cpu_kernel_io_stdin(t_paquete *pkg);

// s_cpu_kernel create_s_cpu_kernel_io_stdout(t_contexto_ejecucion ctx, uint32_t direccion_traducida, char *io_name,
//                                            uint32_t tamano);

t_paquete *serializar_cpu_kernel_io_stdout(s_cpu_kernel s_cpu_kernel_io_stdout);
s_cpu_kernel deserializar_cpu_kernel_io_stdout(t_paquete *pkg);

s_cpu_kernel create_s_cpu_kernel_io_fs_create(char *io_name, t_contexto_ejecucion ctx, char *nombre_archivo);
t_paquete *serializar_cpu_kernel_io_fs_create(s_cpu_kernel s_cpu_kernel_io_fs_create);
s_cpu_kernel deserializar_cpu_kernel_io_fs_create(t_paquete *pkg);

s_cpu_kernel create_s_cpu_kernel_io_fs_delete(char *io_name, t_contexto_ejecucion ctx, char *nombre_archivo);
t_paquete *serializar_cpu_kernel_io_fs_delete(s_cpu_kernel s_cpu_kernel_io_fs_delete);
s_cpu_kernel deserializar_cpu_kernel_io_fs_delete(t_paquete *pkg);

s_cpu_kernel create_s_cpu_kernel_io_fs_truncate(t_contexto_ejecucion ctx, char *io_name, uint32_t tamano,
                                                char *nombre_archivo);
t_paquete *serializar_cpu_kernel_io_fs_truncate(s_cpu_kernel s_cpu_kernel_io_fs_truncate);
s_cpu_kernel deserialziar_cpu_kernel_io_fs_truncate(t_paquete *pkg);

s_cpu_kernel create_s_cpu_kernel_io_fs_write(char *io_name, uint32_t tamano, t_contexto_ejecucion ctx,
                                             t_list *info_marcos, char *nombre_archivo, uint32_t puntero_archivo);
t_paquete *serializar_cpu_kernel_io_fs_write(s_cpu_kernel s_cpu_kernel_io_fs_write);
s_cpu_kernel deserializar_cpu_kernel_io_fs_write(t_paquete *pkg);

s_cpu_kernel create_s_cpu_kernel_io_fs_read(char *io_name, uint32_t tamano, t_contexto_ejecucion ctx,
                                            t_list *direcciones_traducidas, char *nombre_archivo,
                                            uint32_t puntero_archivo);
t_paquete *serializar_cpu_kernel_io_fs_read(s_cpu_kernel s_cpu_kernel_io_fs_read);
s_cpu_kernel deserializar_cpu_kernel_io_fs_read(t_paquete *pkg);

#endif