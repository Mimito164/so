#include "diccionario_registros.h"

static t_dictionary *diccionario_registros_set = NULL;
static t_dictionary *diccionario_registros_get = NULL;

void crear_diccionario_registros_set()
{
    diccionario_registros_set = dictionary_create();
    dictionary_put(diccionario_registros_set, "AX", (void *)&set_ax);
    dictionary_put(diccionario_registros_set, "BX", (void *)&set_bx);
    dictionary_put(diccionario_registros_set, "CX", (void *)&set_cx);
    dictionary_put(diccionario_registros_set, "DX", (void *)&set_dx);
    dictionary_put(diccionario_registros_set, "EAX", (void *)&set_eax);
    dictionary_put(diccionario_registros_set, "EBX", (void *)&set_ebx);
    dictionary_put(diccionario_registros_set, "ECX", (void *)&set_ecx);
    dictionary_put(diccionario_registros_set, "EDX", (void *)&set_edx);
    dictionary_put(diccionario_registros_set, "SI", (void *)&set_si);
    dictionary_put(diccionario_registros_set, "DI", (void *)&set_di);
    dictionary_put(diccionario_registros_set, "PC", (void *)&set_pc);
}

void crear_diccionario_registros_get()
{
    diccionario_registros_get = dictionary_create();
    dictionary_put(diccionario_registros_get, "AX", (void *)&get_ax);
    dictionary_put(diccionario_registros_get, "BX", (void *)&get_bx);
    dictionary_put(diccionario_registros_get, "CX", (void *)&get_cx);
    dictionary_put(diccionario_registros_get, "DX", (void *)&get_dx);
    dictionary_put(diccionario_registros_get, "EAX", (void *)&get_eax);
    dictionary_put(diccionario_registros_get, "EBX", (void *)&get_ebx);
    dictionary_put(diccionario_registros_get, "ECX", (void *)&get_ecx);
    dictionary_put(diccionario_registros_get, "EDX", (void *)&get_edx);
    dictionary_put(diccionario_registros_get, "SI", (void *)&get_si);
    dictionary_put(diccionario_registros_get, "DI", (void *)&get_di);
    dictionary_put(diccionario_registros_get, "PC", (void *)&get_pc);
}

t_dictionary *get_diccionario_registros_set()
{
    if (diccionario_registros_set == NULL)
    {
        crear_diccionario_registros_set();
    }
    return diccionario_registros_set;
}

t_dictionary *get_diccionario_registros_get()
{
    if (diccionario_registros_get == NULL)
    {
        crear_diccionario_registros_get();
    }
    return diccionario_registros_get;
}

void diccionario_registros_set_destroy()
{
    dictionary_destroy(diccionario_registros_set);
}

void diccionario_registros_get_destroy()
{
    dictionary_destroy(diccionario_registros_get);
}