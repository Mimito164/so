#include "io_fs.h"

s_send_kernel_io_fs_args send_kernel_io_fs_args = {};
int recv_io_kernel_fs_args = -1;
s_planificador_corto_args args_p_corto_io_fs_test = {};

typedef void *(*fs_function)(void *);
static void create_fs_thread(fs_function fs_func, t_solicitud *solicitud, t_recurso *recurso, op_code opCode);
static bool recv_io_kernel_fs(int fd_io);
static bool send_kernel_io_fs(int fd_IO, s_kernel_data msj, op_code opCode);

static void create_fs_thread(fs_function fs_func, t_solicitud *solicitud, t_recurso *recurso, op_code opCode)
{
    args_p_solicitud *args = malloc(sizeof(args_p_solicitud));
    args->solicitud = solicitud;
    args->recurso = recurso;
    args->opCode = opCode;

    pthread_t io_communication;
    pthread_create(&io_communication, NULL, fs_func, (void *)args);
#ifndef TEST
    pthread_detach(io_communication);
#else
    pthread_join(io_communication, NULL);
#endif
}

#ifndef TEST
static bool send_kernel_io_fs(int fd_IO, s_kernel_data msj, op_code opCode)
{
    t_paquete *paquete_send;
    switch (opCode)
    {
    case KERNEL_IO_FS_CREATE:
        paquete_send = serializar_kernel_io_fs_create(msj.fs_create);
        break;

    case KERNEL_IO_FS_DELETE:
        paquete_send = serializar_kernel_io_fs_delete(msj.fs_delete);
        break;

    case KERNEL_IO_FS_READ:
        paquete_send = serializar_kernel_io_fs_read(msj.fs_read);
        break;

    case KERNEL_IO_FS_TRUNCATE:
        paquete_send = serializar_kernel_io_fs_truncate(msj.fs_truncate);
        break;

    case KERNEL_IO_FS_WRITE:
        paquete_send = serializar_kernel_io_fs_write(msj.fs_write);
        break;

    default:
        log_error(get_logger(), "send_kernel_io_fs recibió un opCode malo");
        return false;
    }

    bool send_status = send_paquete(fd_IO, paquete_send);

    return send_status;
}
#else
static bool send_kernel_io_fs(int fd_IO, s_kernel_data msj, op_code opCode)
{
    send_kernel_io_fs_args.fd_IO = fd_IO;
    send_kernel_io_fs_args.msj = msj;
    send_kernel_io_fs_args.opCode = opCode;
    return true;
}
#endif

static void *handle_io_fs_thread(void *raw_args)
{
    args_p_solicitud *args = (args_p_solicitud *)raw_args;
    t_solicitud *solicitud = args->solicitud;
    t_recurso *recurso = args->recurso;
    op_code opCode = args->opCode;

    t_log *logger = get_logger();

    log_trace(logger, "OP_CODE: %i enviando a: <%s> - solicitado por el PID <%i>", opCode, recurso->nombre,
              solicitud->pid);

    if (!send_kernel_io_fs(recurso->fd_io, solicitud->data_instruccion, opCode))
    {
        log_trace(logger,
                  "send_kernel_io_fs() op_code: %i falló. Asumimos que la IO <%s> se desconecto. \n "
                  "Mataremos a todos los procesos con solicitudes a este recurso.",
                  opCode, recurso->nombre);

        // eliminar_recurso manda a matar todos los procesos con solicitudes encoladas
        eliminar_recurso(recurso->nombre);
        free(raw_args);
        return NULL;
    }

    if (!recv_io_kernel_fs(recurso->fd_io))
    {
        log_trace(logger,
                  "recv_io_kernel_fs() falló, asumimos que la IO <%s> se desconecto. \n "
                  "Mataremos a todos los procesos con solicitudes a este recurso.",
                  recurso->nombre);

        // eliminar_recurso manda a matar todos los procesos con solicitudes encoladas
        eliminar_recurso(recurso->nombre);
        free(raw_args);
        return NULL;
    }

    log_trace(logger, "recv_io_kernel_fs() recibido de: <%s> - solicitado por el PID <%i>", recurso->nombre,
              solicitud->pid);

    s_planificador_corto_args args_p_corto = {.transicion = BLOCKED_A_READY, .op_pid.PID = (int)solicitud->pid};

    bool result = desencolar_solicitud_io(recurso->nombre, solicitud->pid);

    if (result == true)
    {
#ifndef TEST
        planificador_corto(args_p_corto);
#else
        args_p_corto_io_fs_test = args_p_corto;
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
static bool recv_io_kernel_fs(int fd_io)
{
    t_paquete *paquete = paquete_new();
    t_log *logger = get_logger();

    if (!recv_paquete(fd_io, &paquete))
    {
        // tira false si se desconectó o nos envió basura
        log_trace(logger, "RECV_PAQUETE FALSE !!!");
        return false;
    }

    if (paquete->codigo_operacion != IO_KERNEL_FS_CREATE && paquete->codigo_operacion != IO_KERNEL_FS_DELETE &&
        paquete->codigo_operacion != IO_KERNEL_FS_READ && paquete->codigo_operacion != IO_KERNEL_FS_TRUNCATE &&
        paquete->codigo_operacion != IO_KERNEL_FS_WRITE)
    {
        log_error(logger, "Estamos recibiendo en recv_io_kernel_fs() un opcode raro: %i", paquete->codigo_operacion);
        assert(!"recv_io_kernel_fs() rompió");
    }

    return true;
}
#else
static bool recv_io_kernel_fs(int fd_io)
{
    recv_io_kernel_fs_args = fd_io;
    return true;
}
#endif

void handle_io_fs_create(t_solicitud *solicitud, t_recurso *recurso)
{
    create_fs_thread((void *)&handle_io_fs_thread, solicitud, recurso, KERNEL_IO_FS_CREATE);
}

void handle_io_fs_delete(t_solicitud *solicitud, t_recurso *recurso)
{
    create_fs_thread((void *)&handle_io_fs_thread, solicitud, recurso, KERNEL_IO_FS_DELETE);
}

void handle_io_fs_read(t_solicitud *solicitud, t_recurso *recurso)
{
    create_fs_thread((void *)&handle_io_fs_thread, solicitud, recurso, KERNEL_IO_FS_READ);
}

void handle_io_fs_truncate(t_solicitud *solicitud, t_recurso *recurso)
{
    create_fs_thread((void *)&handle_io_fs_thread, solicitud, recurso, KERNEL_IO_FS_TRUNCATE);
}

void handle_io_fs_write(t_solicitud *solicitud, t_recurso *recurso)
{
    create_fs_thread((void *)&handle_io_fs_thread, solicitud, recurso, KERNEL_IO_FS_WRITE);
}