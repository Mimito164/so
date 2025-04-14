#include "io_stdout.h"

s_send_kernel_io_stdout_write_args send_kernel_io_stdout_write_args = {};
int recv_kernel_io_stdout_write_args = -1;
s_planificador_corto_args args_p_corto_io_stdout_test = {};

static void *handle_io_stdout_write_hilo(void *raw_args);
static bool recv_kernel_io_stdout_write(int fd_io);
static bool send_kernel_io_stdout_write(s_kernel_io_stdout_write msj, int fd_IO);

void handle_io_stdout_write(t_solicitud *solicitud, t_recurso *recurso)
{
    args_p_solicitud *args = malloc(sizeof(args_p_solicitud));
    args->solicitud = solicitud;
    args->recurso = recurso;

    pthread_t io_communication;
    pthread_create(&io_communication, NULL, handle_io_stdout_write_hilo, (void *)args);
#ifndef TEST
    pthread_detach(io_communication);
#else
    pthread_join(io_communication, NULL);
#endif
}

static void *handle_io_stdout_write_hilo(void *raw_args)
{
    args_p_solicitud *args = (args_p_solicitud *)raw_args;
    t_solicitud *solicitud = args->solicitud;
    t_recurso *recurso = args->recurso;

    t_log *logger = get_logger();

    log_trace(logger, "KERNEL_IO_STDIN_READ enviando a: <%s> - solicitado por el PID <%i>", recurso->nombre,
              solicitud->pid);

    if (!send_kernel_io_stdout_write(solicitud->data_instruccion.stdout_write, recurso->fd_io))
    {
        log_trace(logger,
                  "send_kernel_io_stdin_read() falló. Asumimos que la IO <%s> se desconecto. \n "
                  "Mataremos a todos los procesos con solicitudes a este recurso.",
                  recurso->nombre);

        // eliminar_recurso manda a matar todos los procesos con solicitudes encoladas
        eliminar_recurso(recurso->nombre);
        free(raw_args);
        return NULL;
    }

    if (!recv_kernel_io_stdout_write(recurso->fd_io))
    {
        log_trace(logger,
                  "recv_kernel_io_stdin_read() falló, asumimos que la IO <%s> se desconecto. \n "
                  "Mataremos a todos los procesos con solicitudes a este recurso.",
                  recurso->nombre);

        // eliminar_recurso manda a matar todos los procesos con solicitudes encoladas
        eliminar_recurso(recurso->nombre);
        free(raw_args);
        return NULL;
    }

    log_trace(logger, "KERNEL_IO_STDIN_READ recibido de: <%s> - solicitado por el PID <%i>", recurso->nombre,
              solicitud->pid);

    s_planificador_corto_args args_p_corto = {.transicion = BLOCKED_A_READY, .op_pid.PID = (int)solicitud->pid};

    bool result = desencolar_solicitud_io(recurso->nombre, solicitud->pid);

    if (result == true)
    {
#ifndef TEST
        planificador_corto(args_p_corto);
#else
        args_p_corto_io_stdout_test = args_p_corto;
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
static bool send_kernel_io_stdout_write(s_kernel_io_stdout_write msj, int fd_IO)
{
    t_paquete *paquete_send = serializar_kernel_io_stdout_write(msj);
    bool send_status = send_paquete(fd_IO, paquete_send);
    return send_status;
}
#else
static bool send_kernel_io_stdout_write(s_kernel_io_stdout_write msj, int fd_IO)
{
    send_kernel_io_stdout_write_args.msj = msj;
    send_kernel_io_stdout_write_args.fd_IO = fd_IO;
    return true;
}
#endif

#ifndef TEST
static bool recv_kernel_io_stdout_write(int fd_io)
{
    t_paquete *paquete = paquete_new();
    t_log *logger = get_logger();

    if (!recv_paquete(fd_io, &paquete))
    {
        // tira false si se desconectó o nos envió basura
        log_trace(logger, "RECV_PAQUETE FALSE !!!");
        return false;
    }

    if (paquete->codigo_operacion != IO_KERNEL_STDOUT_WRITE)
    {
        log_error(logger, "Estamos recibiendo en recv_kernel_io_stdout_write() un opcode raro: %i",
                  paquete->codigo_operacion);
        assert(!"recv_kernel_io_stdout_write() rompió");
    }

    return true;
}
#else
static bool recv_kernel_io_stdout_write(int fd_io)
{
    recv_kernel_io_stdout_write_args = fd_io;
    return true;
}
#endif