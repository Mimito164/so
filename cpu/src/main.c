#include "TLB/TLB.h"
#include "comunicacion_kernel/iniciar_comunicacion.h"
#include "comunicacion_memoria/iniciar_comunicacion.h"
#include <globals/globals.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <t_config/t_config.h>
#include <utils/hello.h>

int main(int argc, char *argv[])
{

    setup_signal_handler();
    check_args(argc, argv);
    initialize_cfg(argv[1]);
    init_TLB();

    iniciar_comunicacion_memoria(); // se conecta con memoria
    obtener_tam_pag();
    iniciar_servidor_cpu(); // se queda loopeado esperando conexion/desconexion de kernel

    return EXIT_SUCCESS;
}
