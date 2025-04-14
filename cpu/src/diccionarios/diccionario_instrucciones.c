#include "diccionario_instrucciones.h"

static t_dictionary *diccionario_instrucciones = NULL;

void crear_diccionario_instrucciones()
{
    diccionario_instrucciones = dictionary_create();
    dictionary_put(diccionario_instrucciones, "SET", &set);
    dictionary_put(diccionario_instrucciones, "SUM", &sum);
    dictionary_put(diccionario_instrucciones, "SUB", &sub);
    dictionary_put(diccionario_instrucciones, "JNZ", &jnz);
    dictionary_put(diccionario_instrucciones, "WAIT",
                   &nuestro_wait); // tienen este nombre porque wait estaba reservada por
                                   // c
    dictionary_put(diccionario_instrucciones, "SIGNAL",
                   &nuestro_signal); // tienen este nombre porque signal estaba reservada por c
    dictionary_put(diccionario_instrucciones, "IO_GEN_SLEEP", &gen_io_sleep);
    dictionary_put(diccionario_instrucciones, "RESIZE", &resize);
    dictionary_put(diccionario_instrucciones, "MOV_IN", &mov_in);
    dictionary_put(diccionario_instrucciones, "MOV_OUT", &mov_out);
    dictionary_put(diccionario_instrucciones, "IO_FS_TRUNCATE", &io_fs_truncate);
    dictionary_put(diccionario_instrucciones, "IO_FS_DELETE", &io_fs_delete);
    dictionary_put(diccionario_instrucciones, "IO_FS_CREATE", &io_fs_create);
    dictionary_put(diccionario_instrucciones, "IO_STDIN_READ", &io_stdin_read);
    dictionary_put(diccionario_instrucciones, "IO_STDOUT_WRITE", &io_stdout_write);
    dictionary_put(diccionario_instrucciones, "COPY_STRING", &copy_string);
    dictionary_put(diccionario_instrucciones, "IO_FS_READ", &io_fs_read);
    dictionary_put(diccionario_instrucciones, "IO_FS_WRITE", &io_fs_write);
}

t_dictionary *get_diccionario_instrucciones()
{
    if (diccionario_instrucciones == NULL)
    {
        crear_diccionario_instrucciones();
    }
    return diccionario_instrucciones;
}

void diccionario_instrucciones_destroy()
{
    dictionary_destroy(diccionario_instrucciones);
}