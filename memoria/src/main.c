#include "comunicacion/iniciar_servidor_memoria.h"
#include "user_mem/user_mem.h"
#include <estructuras_administrativas/bitmap_frames_libres.h>
#include <estructuras_administrativas/tabla_instrucciones.h>
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
    instanciate_user_mem();
    crear_tabla_instrucciones();
    crear_bitmap_frames_libres();
    crear_tabla_paginas_global();
    iniciar_servidor_memoria(); // se queda loopeado dentro de la funcion

    return EXIT_SUCCESS;
}
