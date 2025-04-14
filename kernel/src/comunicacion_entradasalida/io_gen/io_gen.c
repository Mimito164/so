#include "io_gen.h"

s_send_kernel_io_gen_args send_kernel_io_gen_args = {};
int recv_io_kernel_gen_sleep_args = -1;
s_planificador_corto_args args_p_corto_io_gen_test = {};

static bool send_kernel_io_gen(s_kernel_io_sleep msj, int fd_IO);
static void *handle_io_gen_sleep_hilo(void *raw_args);
static bool recv_io_kernel_gen_sleep(int fd_io);

void handle_io_gen_sleep(t_solicitud *solicitud, t_recurso *recurso)
{
    args_p_solicitud *args = malloc(sizeof(args_p_solicitud));
    args->solicitud = solicitud;
    args->recurso = recurso;

    pthread_t io_communication;
    pthread_create(&io_communication, NULL, handle_io_gen_sleep_hilo, (void *)args);
#ifdef TEST
    pthread_join(io_communication, NULL);
#else
    pthread_detach(io_communication);
#endif
}

static void *handle_io_gen_sleep_hilo(void *raw_args)
{
    args_p_solicitud *args = (args_p_solicitud *)raw_args;
    t_solicitud *solicitud = args->solicitud;
    t_recurso *recurso = args->recurso;

    t_log *logger = get_logger();

    log_trace(logger, "KERNEL_IO_GEN_ZZZ enviando a: <%s> - solicitado por el PID <%i>", recurso->nombre,
              solicitud->pid);

    if (!send_kernel_io_gen(solicitud->data_instruccion.sleep, recurso->fd_io))
    {
        log_trace(logger,
                  "send_kernel_io_gen() falló. Asumimos que la IO <%s> se desconecto. \n "
                  "Mataremos a todos los procesos con solicitudes a este recurso.",
                  recurso->nombre);

        // eliminar_recurso manda a matar todos los procesos con solicitudes encoladas
        eliminar_recurso(recurso->nombre);
        free(raw_args);
        return NULL;
    }

    if (!recv_io_kernel_gen_sleep(recurso->fd_io))
    {
        log_trace(logger,
                  "recv_io_kernel_gen_sleep() falló, asumimos que la IO <%s> se desconecto. \n "
                  "Mataremos a todos los procesos con solicitudes a este recurso.",
                  recurso->nombre);

        // eliminar_recurso manda a matar todos los procesos con solicitudes encoladas
        eliminar_recurso(recurso->nombre);
        free(raw_args);
        return NULL;
    }

    log_trace(logger, "IO_KERNEL_GEN_ZZZ recibido de: <%s> - solicitado por el PID <%i>", recurso->nombre,
              solicitud->pid);

    s_planificador_corto_args args_p_corto = {.transicion = BLOCKED_A_READY, .op_pid.PID = (int)solicitud->pid};

    bool result = desencolar_solicitud_io(recurso->nombre, solicitud->pid);

    if (result == true)
    {
#ifndef TEST
        planificador_corto(args_p_corto);
#else
        args_p_corto_io_gen_test = args_p_corto;
#endif
    }
    else
    {
        log_error(logger, "desencolar_solicitud(%s, %i) falló", recurso->nombre, solicitud->pid);
    }

    free(raw_args);
    return NULL;
}

#ifndef TEST
static bool send_kernel_io_gen(s_kernel_io_sleep msj, int fd_IO)
{
    t_paquete *paquete_send = serializar_kernel_io_sleep_struct(msj);
    bool send_status = send_paquete(fd_IO, paquete_send);
    return send_status;
}
#else
static bool send_kernel_io_gen(s_kernel_io_sleep msj, int fd_IO)
{

    send_kernel_io_gen_args.msj = msj;
    send_kernel_io_gen_args.fd_IO = fd_IO;
    return true;
}
#endif

#ifndef TEST
static bool recv_io_kernel_gen_sleep(int fd_io)
{
    t_paquete *paquete = paquete_new();
    t_log *logger = get_logger();

    if (!recv_paquete(fd_io, &paquete))
    {
        // tira false si se desconectó o nos envió basura
        log_trace(logger, "RECV_PAQUETE FALSE !!!");
        return false;
    }

    if (paquete->codigo_operacion != IO_KERNEL_GEN_ZZZ)
    {
        log_error(logger, "Estamos recibiendo en recv_io_kernel_gen_sleep() un opcode raro: %i",
                  paquete->codigo_operacion);
        assert(!"recv_io_kernel_gen_sleep() rompió");
    }

    return true;
}
#else
static bool recv_io_kernel_gen_sleep(int fd_io)
{
    recv_io_kernel_gen_sleep_args = fd_io;
    return true;
}
#endif