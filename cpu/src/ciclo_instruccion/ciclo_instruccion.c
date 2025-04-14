#include "ciclo_instruccion.h"

char *fetch()
{
    log_info(get_logger(), "PID: <%i> - FETCH - Program Counter: <%i>\n", get_pid(), get_pc());
    char *instruccion = obtener_instruccion(get_pid(), get_pc());

    return instruccion;
}

static void free_string_array(char **array)
{
    for (int i = 0; array[i] != NULL; i++)
    {
        free(array[i]);
    }
    free(array);
}

char **decode(char *instruccion)
{
    char **instruccion_decodificada = string_array_new();

    char **lista_strings = string_split(instruccion, " ");
    char *prefijo_instruccion = lista_strings[0];

    char *argumentos = string_new();

    for (int i = 1; i < string_array_size(lista_strings); i++)
    {
        string_append(&argumentos, lista_strings[i]);
        string_append(&argumentos, " ");
    }
    string_trim_right(&argumentos);

    string_array_push(&instruccion_decodificada, prefijo_instruccion);
    string_array_push(&instruccion_decodificada, argumentos);

    // free(argumentos);
    // free_string_array(lista_strings);

    return instruccion_decodificada;
}

t_paquete *execute(char **instruccion_decodificada)
{
    if (strcmp(instruccion_decodificada[0], "EXIT") == 0)
    {
        log_info(get_logger(), "PID: <%i> -EXIT-\n", get_pid());

        t_paquete *paquete = paquete_new();

        paquete->codigo_operacion = CPU_KERNEL_KILL;
        return paquete;
    }
    t_paquete *(*funcion_instruccion)(char *) =
        dictionary_get(get_diccionario_instrucciones(), instruccion_decodificada[0]);
    t_paquete *paquete = (funcion_instruccion)(instruccion_decodificada[1]);

    // string_array_destroy(instruccion_decodificada);

    return paquete;
}

t_paquete *manejoDeInterrupciones()
{
    t_paquete *paquete = NULL;
    if (!get_interrupt_state())
    {
        clear_interrupt();
        return paquete;
    }

    if (get_interrupt_pid() != get_pid())
    {
        clear_interrupt();
        return paquete;
    }

    switch (get_interrupt_type())
    {
    case DESALOJO:
        paquete = paquete_new();
        paquete->codigo_operacion = CPU_KERNEL_DESALOJO_FINQ;
        serializar_contexto_ejecucion(&paquete->buffer, get_all());
        clear_interrupt();
        break;
    case KILL:
        paquete = paquete_new();
        paquete->codigo_operacion = CPU_KERNEL_KILL;
        clear_interrupt();
        break;
    case KILL_KILLED:
        paquete = paquete_new();
        paquete->codigo_operacion = CPU_KERNEL_KILL_ALREADY_KILLED;
        clear_interrupt();
        break;
    default:
        break;
    }
    return paquete;
}

t_paquete *paquete_prioritario(t_paquete *paquete1, t_paquete *paquete2) // NOLINT(bugprone-easily-swappable-parameters)
{
    if (paquete1 == NULL)
    {
        return paquete2;
    }
    if (paquete2 == NULL)
    {
        return paquete1;
    }
    // "paquete1 == NULL && paquete2 == NULL" este caso esta manejado en la condicion del propio while
    // el que tenga opcode mas chico es mas importante
    if (paquete1->codigo_operacion < paquete2->codigo_operacion)
    {
        paquete_destroy(paquete2);
        return paquete1;
    }
    paquete_destroy(paquete1);
    return paquete2;
}

t_paquete *ciclo_instruccion()
{
    t_paquete *paquete_interrupt = NULL;
    t_paquete *paquete_execute = NULL;

    while (paquete_execute == NULL && paquete_interrupt == NULL)
    {
        char *instruccion = fetch();
        char **instruccion_decodificada = decode(instruccion);
        paquete_execute = execute(instruccion_decodificada);
        paquete_interrupt = manejoDeInterrupciones();
        free(instruccion);
        free_string_array(instruccion_decodificada);
    }
    t_paquete *paquete_a_enviar = paquete_prioritario(paquete_execute, paquete_interrupt);

    log_info(get_logger(), "Se devuelve el contexto");
    clear_contexto_ejecucion();
    return paquete_a_enviar;
}