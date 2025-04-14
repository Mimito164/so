#ifndef GESTOR_RECURSOS_SHARE_H_
#define GESTOR_RECURSOS_SHARE_H_

#include "TAD/TAD.h"
#include "comunicacion_entradasalida/iniciar_comunicacion.h"
#include "serializacion/mensajes/cpu_kernel/msj_cpu_kernel.h"
#include "serializacion/mensajes/kernel_io/msj_kernel_io.h"
#include <commons/collections/list.h>
#include <pthread.h>

typedef union {
    s_kernel_io_fs_create fs_create;
    s_kernel_io_fs_delete fs_delete;
    s_kernel_io_fs_read fs_read;
    s_kernel_io_fs_truncate fs_truncate;
    s_kernel_io_fs_write fs_write;
    s_kernel_io_sleep sleep;
    s_kernel_io_stdin_read stdin_read;
    s_kernel_io_stdout_write stdout_write;
    s_cpu_kernel wati_o_signal;
} s_kernel_data;

typedef enum
{
    INST_CPU_KERNEL_DESALOJO_SIGNAL = CPU_KERNEL_DESALOJO_SIGNAL,
    INST_CPU_KERNEL_DESALOJO_WAIT = CPU_KERNEL_DESALOJO_WAIT,
    INST_KERNEL_IO_GEN_ZZZ = KERNEL_IO_GEN_ZZZ,
    INST_KERNEL_IO_STDIN_READ = KERNEL_IO_STDIN_READ,
    INST_KERNEL_IO_STDOUT_WRITE = KERNEL_IO_STDOUT_WRITE,
    INST_KERNEL_IO_FS_CREATE = KERNEL_IO_FS_CREATE,
    INST_KERNEL_IO_FS_DELETE = KERNEL_IO_FS_DELETE,
    INST_KERNEL_IO_FS_TRUNCATE = KERNEL_IO_FS_TRUNCATE,
    INST_KERNEL_IO_FS_WRITE = KERNEL_IO_FS_WRITE,
    INST_KERNEL_IO_FS_READ = KERNEL_IO_FS_READ,
} t_instruccion;

typedef enum
{
    SISTEMA_SIGNAL_WAIT,
    IO_GEN,
    IO_STDOUT,
    IO_STDIN,
    IO_FS
} TIPO_RECURSOS;

typedef struct
{
    char *nombre;
    int instancias;
    int instancias_disponibles;
    int fd_io;
    TIPO_RECURSOS tipo;
    t_list *solicitudes;
    pthread_mutex_t mutex_recurso;
} t_recurso;

typedef struct
{
    uint32_t pid;
    bool procesando;
    int fd_io;
    t_instruccion tipo_instruccion;
    s_kernel_data data_instruccion;
} t_solicitud;

/// @brief Busca un recurso por su nombre y lo destruye junto con todo su contenido\
/// @brief Manda kill a todos los procesos que tengan solicitudes al recurso
/// @param recurso el recurso para eliminar
void eliminar_recurso(char *nombre);

typedef enum
{
    KILL_AL_PROCESO,
    PROCESANDO_AHORA,
    ESPERANDO_SER_PROCESADO,
} resultado_encolar;

/// @brief Agrega el recurso a la lista de recursos
/// @param nombre El nombre (que debe ser único) del recurso
/// @param instancias La cantidad de instancias del recurso. Si es única, hay una sola instancia
/// @param tipo El tipo de IO o si es un recurso de sistema
void agregar_recurso(char *nombre, int instancias, TIPO_RECURSOS tipo, int fd_io);

/// @brief Crea el diccionario de recursos e instancia los recursos dados por las config
void init_gestor_recursos();

/// @brief Elimina el diccionario del gestor de recursos y todo su contenido
void destroy_gestor_recursos();

/// @brief Encolamos la solicitud en la lista de solicitudes del recurso\
/// @brief Si se puede, la solicitud será procesada en el momento.
/// @param nombre_recurso que se solicita usar
/// @param tipo_instruccion el tipo de instrucción que se debe realizar
/// @param data_instruccion la información de la instrucción a realizar
/// @returns KILL_AL_PROCESO | PROCESANDO_AHORA | ESPERANDO_SER_PROCESADO
resultado_encolar encolar_solicitud(char *nombre_recurso, t_instruccion tipo_instruccion,
                                    s_kernel_data data_instruccion);

/// @brief Quita la solicitud de la cola de solicitudes para un recurso\
/// @brief Si se puede poner la siguiente a procesar, lo hace
/// @warning esta funcion solo se usa para las io
/// @param nombre_recurso el recurso que tiene la solicitud encolada
/// @param pid el pid del proceso a quien pertenece la solicitud
/// @return `true` si salió todo bien\
/// @return `false` si hubo un error
bool desencolar_solicitud_io(char *nombre_recurso, uint32_t pid);

/// @brief Quita la solicitud de la cola de solicitudes para un recurso\
/// @brief Si se puede poner la siguiente a procesar, lo hace
/// @param nombre_recurso el recurso que tiene la solicitud encolada
/// @param pid el pid del proceso a quien pertenece la solicitud
/// @return `true` si salió todo bien\
/// @return `false` si hubo un error
bool desencolar_solicitud_sistema(char *nombre_recurso, uint32_t pid, bool es_exterminio);

/// @brief busca si un proceso tiene asignados recursos y luego los libera
/// @param pid el pid del proceso al que deben pertenecer las solicitudes
void liberar_recursos(uint32_t pid);

#endif