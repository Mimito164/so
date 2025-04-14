#include "comunicacion_cpu/comunicacion_cpu.h"
#include "comunicacion_entradasalida/iniciar_comunicacion.h"
#include "comunicacion_memoria/iniciar_comunicacion.h"
#include "consola/consola.h"
#include "gestor_recursos/gestor_recursos_share.h"
#include "globals/globals.h"
#include "handlers/colas_estados/colas_handler.h"
#include "semaforos_planificadores/semaforos_planificadores.h"
#include <signal.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    setup_signal_handler();
    check_args(argc, argv);
    initialize_cfg(argv[1]);
    init_mutex_multiprogramacion();

    sem_init(&executing_mutex, 0, 1);

    initialize_lists();
    init_gestor_recursos();
    inicializar_semaforos_pausa_planificador_corto();

    iniciar_comunicacion_memoria();

    // send_Kernel_memoria_creacion(1, "./pepe.text");
    // send_Kernel_memoria_finalizacion(1);

    iniciar_comunicacion_cpu();
    iniciar_consola();
    iniciar_comunicacion_io();
    // TODO(All): se queda loopeado esperando conexion/desconexion de kernel, no deberia ser bloqueante. Reemplazar por
    // un hilo.

    return EXIT_SUCCESS;
}
