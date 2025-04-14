#include "hilo_dispatch.h"

void atender_cliente_dispatch(client_thread_args_dipatch *args)
{
    t_log *logger = get_logger();

    t_paquete *paquete_a_recibir;
    while (recv_paquete(*(args->fd), &paquete_a_recibir))
    {
        assert(paquete_a_recibir->codigo_operacion == KERNEL_CPU_EXECUTE);
        t_contexto_ejecucion cont = deserializar_contexto_ejecucion(paquete_a_recibir);
        set_all(cont);
        paquete_destroy(paquete_a_recibir);
        t_paquete *paquete_a_mandar = ciclo_instruccion();
        send_paquete(*(args->fd), paquete_a_mandar);
    }
    log_warning(logger, "Se desconecto un cliente");

    close(*(args->fd));
    free(args->fd);
    free(args);
    pthread_exit(NULL);
}
