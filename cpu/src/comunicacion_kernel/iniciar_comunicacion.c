#include "iniciar_comunicacion.h"
#include "globals/globals.h"

int fd_escucha_dispatch;
int fd_escucha_interrupt;

void iniciar_servidor_cpu()
{
    t_log *logger = get_logger();
    t_config_cpu *cfg = get_config();

    fd_escucha_dispatch =
        iniciar_servidor(logger, "Servidor CPU Dispatch", cfg->IP_MODULO, cfg->PUERTO_ESCUCHA_DISPATCH);
    fd_escucha_interrupt =
        iniciar_servidor(logger, "Servidor CPU Interrupt", cfg->IP_MODULO, cfg->PUERTO_ESCUCHA_INTERRUPT);

    while (1)
    {
        client_thread_args_dipatch *args_dispatch = malloc(sizeof(client_thread_args_dipatch));
        args_dispatch->fd = malloc(sizeof(int));
        *(args_dispatch->fd) = esperar_cliente(logger, "Servidor CPU Dispatch", fd_escucha_dispatch);

        pthread_t thread_dispatch;
        pthread_create(&thread_dispatch, NULL, (void *)atender_cliente_dispatch, (void *)args_dispatch);
        pthread_detach(thread_dispatch);

        client_thread_args_interrupt *args_interrupt = malloc(sizeof(client_thread_args_interrupt));
        args_interrupt->fd = malloc(sizeof(int));
        *(args_interrupt->fd) = esperar_cliente(logger, "Servidor CPU Interrupt", fd_escucha_interrupt);

        pthread_t thread_interrupt;
        pthread_create(&thread_interrupt, NULL, (void *)atender_cliente_interrupt, (void *)args_interrupt);
        pthread_detach(thread_interrupt);
    }
}