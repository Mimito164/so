#include "hilo_interrupt.h"

void atender_cliente_interrupt(client_thread_args_interrupt *args)
{
    t_log *logger = get_logger();
    init_mutex_interrupt();

    t_paquete *paquete_a_recibir;
    while (recv_paquete(*(args->fd), &paquete_a_recibir))
    {

        switch (paquete_a_recibir->codigo_operacion)
        {
        case KERNEL_CPU_INT_DESALOJO:

        {
            uint32_t pid;
            deserializar_uint32_t(paquete_a_recibir->buffer, &pid);
            if (get_interrupt_type() < DESALOJO)
            {
                break;
            }
            set_interrupt((int)pid, DESALOJO);
            log_debug(logger, "KERNEL_CPU_INT_DESALOJO recibido");
            break;
        }
        case KERNEL_CPU_INT_KILL:

        {
            uint32_t pid;
            deserializar_uint32_t(paquete_a_recibir->buffer, &pid);
            if (get_interrupt_type() < KILL)
            {
                break;
            }
            set_interrupt((int)pid, KILL);
            log_debug(logger, "KERNEL_CPU_INT_KILL recibido");
            break;
        }

        case KERNEL_CPU_INT_KILL_ALREADY_KILLED: {
            uint32_t pid;
            deserializar_uint32_t(paquete_a_recibir->buffer, &pid);
            if (get_interrupt_type() < KILL_KILLED)
            {
                break;
            }
            set_interrupt((int)pid, KILL_KILLED);
            log_debug(logger, "KERNEL_CPU_INT_KILL_ALREADY_KILLED recibido");
            break;
        }

        default:
            assert(!"CPU recibió por interrupt algo que no es una interrupción.");
            log_error(logger, "CPU recibió por interrupt algo que no es una interrupción.");
            break;
        }

        // paquete_a_recibir = paquete_new();
        paquete_destroy(paquete_a_recibir);
    }
    log_warning(logger, "Se desconecto un cliente");

    destroy_mutex_interrupt();
    close(*(args->fd));
    free(args->fd);
    free(args);

    pthread_exit(NULL);
}
