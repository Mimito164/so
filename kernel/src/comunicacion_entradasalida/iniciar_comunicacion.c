#include "iniciar_comunicacion.h"
#include "gestor_recursos/gestor_recursos_share.h"
#include "globals/globals.h"
#include "serializacion/mensajes/io_kernel/msj_io_kernel.h"
#include "serializacion/serializacion.h"

static TIPO_RECURSOS traducir_a_tipo_recursos(tipo_IO tipo);
static void *agregador_de_recursos(void *args);

struct agregador_de_recursos_args
{
    s_io_kernel_iniciar_conexion new_IO;
    int fd_IO;
};

void iniciar_comunicacion_io()
{
    t_log *logger = get_logger();
    t_config_kernel *cfg = get_config();
    int fd_escucha = iniciar_servidor(logger, "Servidor Kernel", cfg->IP_MODULO, cfg->PUERTO_ESCUCHA);

    while (1)
    {
        int *fd_conexion_ptr = malloc(sizeof(int));
        *fd_conexion_ptr = esperar_cliente(logger, "Server Kernel", fd_escucha);

        t_paquete *paquete_a_recibir = paquete_new();
        recv_paquete(*fd_conexion_ptr, &paquete_a_recibir);
        if (paquete_a_recibir->codigo_operacion != IO_KERNEL_INICIAR_CONEXION)
        {
            log_error(logger, "Recibimos de IO un op_code erroneo: %i", paquete_a_recibir->codigo_operacion);
            exit(1);
        }
        s_io_kernel_iniciar_conexion new_IO = deserializar_io_kernel_INICIAR_CONEXION(paquete_a_recibir);
        log_info(logger, "Se conecto la IO: %s", new_IO.nombreDeIO);

        struct agregador_de_recursos_args *args = malloc(sizeof(struct agregador_de_recursos_args));
        args->new_IO = new_IO;
        args->fd_IO = *fd_conexion_ptr;

        pthread_t hilo_agregador_de_recursos;
        pthread_create(&hilo_agregador_de_recursos, NULL, agregador_de_recursos, (void *)args);
        pthread_detach(hilo_agregador_de_recursos);
    }
}

static void *agregador_de_recursos(void *raw_args)
{
    struct agregador_de_recursos_args *args = (struct agregador_de_recursos_args *)raw_args;
    agregar_recurso(args->new_IO.nombreDeIO, 1, traducir_a_tipo_recursos(args->new_IO.tipo), args->fd_IO);
    free(args);
    return NULL;
}

static TIPO_RECURSOS traducir_a_tipo_recursos(tipo_IO tipo)
{

    switch (tipo)
    {
    case DialFS:
        return IO_FS;
        break;
    case STDOUT:
        return IO_STDOUT;
        break;
    case STDIN:
        return IO_STDIN;
        break;
    case GENERICA:
        return IO_GEN;
        break;
    default:
        assert(!"El tipo de esta io no existe");
        break;
    }
}
