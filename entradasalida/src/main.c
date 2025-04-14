#include <TAD/TAD.h>
#include <comunicacion_kernel/iniciar_comunicacion.h>
#include <comunicacion_memoria/iniciar_comunicacion.h>
#include <estructuras/estructuras.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils/hello.h>

int main(int argc, char *argv[])
{
    setup_signal_handler();
    // Verificamos que esten bien los argumentos pasados al llamar a la funcion
    check_args(argc, argv);
    // Leemos el archivo config y lo cargamos en memoria
    initialize_cfg(argv[1]);
    // Almacenamos el nombre de la IO como global
    set_nombre_io(argv[2]);
    // Iniciamos las estructuras de las DialFS (archivo de bloques y bitarray)
    initialize_estructuras();

    //------zona para jugar------

    //------zona para jugar------

    //-----zona de laburo-----
    iniciar_comunicacion_memoria();
    iniciar_comunicacion_kernel();
    //-----zona de laburo-----

    return EXIT_SUCCESS;
}
