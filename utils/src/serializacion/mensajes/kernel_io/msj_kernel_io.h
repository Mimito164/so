#ifndef MSJ_KERNEL_IO_H_
#define MSJ_KERNEL_IO_H_

#include "assert.h"
#include "serializacion/mensajes/TODOS/todos.h"
#include "serializacion/serializacion.h"
#include <serializacion/mensajes/TODOS/todos.h>

// IO_GEN_SLEEP
typedef struct
{
    uint32_t pid;
    uint32_t work_unit;
} s_kernel_io_sleep;

s_kernel_io_sleep create_kernel_io_sleep_struct(uint32_t pid, uint32_t work_unit);
t_paquete *serializar_kernel_io_sleep_struct(s_kernel_io_sleep s_knel_io_sleep);
s_kernel_io_sleep deserializar_kernel_io_sleep_struct(t_paquete *paquete);

// IO_STDIN_READ
typedef struct
{
    uint32_t pid;
    uint32_t size;
    t_list *lista_frames;
} s_kernel_io_stdin_read;

s_kernel_io_stdin_read create_kernel_io_stdin_read(uint32_t pid, t_list *lista_frames, uint32_t size);
t_paquete *serializar_kernel_io_stdin_read(s_kernel_io_stdin_read s_knel_io_stdin_read);
s_kernel_io_stdin_read deserializar_kernel_io_stdin_read(t_paquete *paquete);

// IO_STDOUT_WRITE
typedef struct
{
    uint32_t pid;
    t_list *lista_frames;
} s_kernel_io_stdout_write;

s_kernel_io_stdout_write create_kernel_io_stdout_write(uint32_t pid, t_list *lista_frames);
t_paquete *serializar_kernel_io_stdout_write(s_kernel_io_stdout_write s_knel_io_stdout_write);
s_kernel_io_stdout_write deserializar_kernel_io_stdout_write(t_paquete *paquete);

// IO_FS_CREATE
typedef struct
{
    uint32_t pid;
    char *file_name;
} s_kernel_io_fs_create;

s_kernel_io_fs_create create_kernel_io_fs_create(uint32_t pid, char *file_name);
void destroy_kernel_io_fs_create(s_kernel_io_fs_create *s_knel_io_fs_create);
t_paquete *serializar_kernel_io_fs_create(s_kernel_io_fs_create s_knel_io_fs_create);
s_kernel_io_fs_create deserializar_kernel_io_fs_create(t_paquete *paquete);

// IO_FS_DELETE
typedef struct
{
    uint32_t pid;
    char *file_name;
} s_kernel_io_fs_delete;

s_kernel_io_fs_delete create_kernel_io_fs_delete(uint32_t pid, char *file_name);
void destroy_kernel_io_fs_delete(s_kernel_io_fs_delete *s_knel_io_fs_delete);
t_paquete *serializar_kernel_io_fs_delete(s_kernel_io_fs_delete s_knel_io_fs_delete);
s_kernel_io_fs_delete deserializar_kernel_io_fs_delete(t_paquete *paquete);

// IO_FS_TRUNCATE
typedef struct
{
    uint32_t pid;
    uint32_t new_size;
    char *file_name;
} s_kernel_io_fs_truncate;

s_kernel_io_fs_truncate create_kernel_io_fs_truncate(uint32_t pid, char *file_name, uint32_t new_size);
void destroy_kernel_io_fs_truncate(s_kernel_io_fs_truncate *s_knel_io_fs_truncate);
t_paquete *serializar_kernel_io_fs_truncate(s_kernel_io_fs_truncate s_knel_io_fs_truncate);
s_kernel_io_fs_truncate deserializar_kernel_io_fs_truncate(t_paquete *paquete);

// IO_FS_WRITE
typedef struct
{
    uint32_t pid;
    char *file_name;
    uint32_t file_pointer;
    t_list *info_marcos;
    uint32_t size;

} s_kernel_io_fs_write;

s_kernel_io_fs_write create_kernel_io_fs_write(uint32_t pid, char *file_name, uint32_t file_pointer,
                                               t_list *info_marcos, uint32_t size);
void destroy_kernel_io_fs_write(s_kernel_io_fs_write *s_knel_io_fs_write);
t_paquete *serializar_kernel_io_fs_write(s_kernel_io_fs_write s_knel_io_fs_write);
s_kernel_io_fs_write deserializar_kernel_io_fs_write(t_paquete *paquete);

// IO_FS_READ
typedef struct
{
    uint32_t pid;
    char *file_name;
    uint32_t file_pointer;
    t_list *info_marcos;
    uint32_t size;

} s_kernel_io_fs_read;

s_kernel_io_fs_read create_kernel_io_fs_read(uint32_t pid, char *file_name, uint32_t file_pointer, t_list *info_marcos,
                                             uint32_t size);
void destroy_kernel_io_fs_read(s_kernel_io_fs_read *s_knel_io_fs_read);
t_paquete *serializar_kernel_io_fs_read(s_kernel_io_fs_read s_knel_io_fs_read);
s_kernel_io_fs_read deserializar_kernel_io_fs_read(t_paquete *paquete);

#endif
