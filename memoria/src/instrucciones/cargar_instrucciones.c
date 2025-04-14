#include "cargar_instrucciones.h"

t_list *cargar_instrucciones(char *archivo)
{
    // log_debug(get_logger(), "cargar_instrucciones 1");

    FILE *file;
    file = fopen(archivo, "r");

    // log_debug(get_logger(), "cargar_instrucciones 2");

    if (file == NULL)
    {
        // log_debug(get_logger(), "cargar_instrucciones FILE NULL");
        // TODO: hacer una función que limpie todo correctamente, mate los hilos y cierre el programa.
        //  NOLINTNEXTLINE(concurrency-mt-unsafe)
        exit(1);
    }

    // log_debug(get_logger(), "cargar_instrucciones 3");

    t_list *list_instrucciones = list_create();

    char linea[LINE_MAX]; // linea que se guarda del fgets

    // log_debug(get_logger(), "cargar_instrucciones 4");

    // NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
    while (fgets(linea, LINE_MAX, file) != NULL)
    {
        // log_debug(get_logger(), "cargar_instrucciones while");

        // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.strcpy)
        strcpy(linea, string_replace(linea, "\n", ""));
        char *str = string_duplicate(linea);
        list_add(list_instrucciones, str);
    }

    // log_debug(get_logger(), "cargar_instrucciones 5");

    // NOLINTNEXTLINE(cert-err33-c)
    fclose(file);
    free(archivo);
    // log_debug(get_logger(), "cargar_instrucciones 6");

    return list_instrucciones;
}