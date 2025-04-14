#include "diccionario_instrucciones.h"

t_dictionary *argumentos_comandos;

void iniciar_diccionario(void)
{
    argumentos_comandos = dictionary_create();

    dictionary_put(argumentos_comandos, "EJECUTAR_SCRIPT", (void *)1);
    dictionary_put(argumentos_comandos, "EXIT", (void *)0);
    dictionary_put(argumentos_comandos, "INICIAR_PROCESO", (void *)1);
    dictionary_put(argumentos_comandos, "FINALIZAR_PROCESO", (void *)1);
    dictionary_put(argumentos_comandos, "DETENER_PLANIFICACION", (void *)0);
    dictionary_put(argumentos_comandos, "INICIAR_PLANIFICACION", (void *)0);
    dictionary_put(argumentos_comandos, "MULTIPROGRAMACION", (void *)1);
    dictionary_put(argumentos_comandos, "PROCESO_ESTADO", 0);
}