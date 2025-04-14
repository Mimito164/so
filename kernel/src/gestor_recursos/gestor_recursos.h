#ifndef GESTOR_RECURSOS_H_
#define GESTOR_RECURSOS_H_

#include "comunicacion_entradasalida/io_fs/io_fs.h"
#include "comunicacion_entradasalida/io_gen/io_gen.h"
#include "comunicacion_entradasalida/io_stdin/io_stdin.h"
#include "comunicacion_entradasalida/io_stdout/io_stdout.h"
#include "finalizacion/blacklist.h"
#include "gestor_recursos_share.h"
#include "globals/globals.h"
#include "t_config/t_config.h"
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    t_dictionary **recursos;
    pthread_mutex_t mutex_recursos;
    t_solicitud *(*create_solicitud)(int pid, s_kernel_data data_instruccion, t_instruccion tipo_instruccion);
    void (*destroy_solicitud_and_kill_process)(void *solicitud);
    bool (*procesar_solicitud)(t_solicitud *solicitud, t_recurso *recurso);
    t_recurso *(*create_recurso)(int instancias, TIPO_RECURSOS tipo, int fd_io, char *nombre);
    t_recurso *(*get_recurso)(char *nombre);

} gestor_recursos_statics;

gestor_recursos_statics get_gestor_recursos_statics();

#endif
