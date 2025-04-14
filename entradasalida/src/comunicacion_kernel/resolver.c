#include "resolver.h"

t_paquete *paqueteEnviarMemoria_test = NULL;

t_paquete *resolver_KERNEL_IO_GEN_ZZZ(t_paquete *paquete_reciv)
{
    t_config_io *cfg = get_config();
    const int mil = 1000;
    s_kernel_io_sleep estructura = deserializar_kernel_io_sleep_struct(paquete_reciv);
    paquete_destroy(paquete_reciv);
    log_info(get_logger(), "IO : <%i> | PID : <%i> | OPERACION: <ZZZ POR %.2f segundos>", cfg->TIPO_INTERFAZ,
             estructura.pid, (double)estructura.work_unit * cfg->TIEMPO_UNIDAD_TRABAJO / mil);
    sleep(estructura.work_unit * cfg->TIEMPO_UNIDAD_TRABAJO / mil);
    t_paquete *paqueteEnviar = paquete_new();
    paqueteEnviar->codigo_operacion = IO_KERNEL_GEN_ZZZ; // asheeeee

    return paqueteEnviar;
}

t_paquete *resolver_KERNEL_IO_STDIN_READ(t_paquete *paquete_reciv)
{
    t_config_io *cfg = get_config();
    t_log *logger = get_logger();

    s_kernel_io_stdin_read estructura = deserializar_kernel_io_stdin_read(paquete_reciv);
    paquete_destroy(paquete_reciv);
    log_info(logger, "IO : <%i> | PID : <%i> | OPERACION: <STDIN_READ>", cfg->TIPO_INTERFAZ, estructura.pid);

    printf("\n\n");
    printf("---------------------------------------------------------\n\n");
    printf("INGRESAR UNA CADENA DE %i CARACTERES\n", (int)estructura.size);
    printf("\n");
    printf("---------------------------------------------------------\n\n");

    // le dejo un caracter de mas porque creo que fgets guarda el /o al final
    char *ingreso = (char *)malloc(estructura.size + 1);

    if (fgets(ingreso, (int)estructura.size + 1, stdin) == NULL)
    {
        // log_error(logger, "No se puedo realizar el ingreso -> seguramente es culpa de juano");
        mem_clean_exit();
    }
    // LIMPIO EL BUFFER
    char c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    t_list_iterator *list_iter = list_iterator_create(estructura.lista_frames);
    int offset = 0;

    while (list_iterator_has_next(list_iter))
    {
        frames_element *curr_el = (frames_element *)list_iterator_next(list_iter);

        s_io_memoria_read estructuraMemoria;
        estructuraMemoria.pid = (int)curr_el->pid;
        estructuraMemoria.size = (int)(curr_el->tam_pag - curr_el->offset - curr_el->padding);
        estructuraMemoria.real_mem_dir = (curr_el->tam_pag * curr_el->frame + curr_el->offset);

        uint32_t curr_el_size = curr_el->tam_pag - curr_el->offset - curr_el->padding;
        estructuraMemoria.data = string_substring_until(ingreso + offset, (int)curr_el_size); // esto no mallockea!!
        offset += (int)curr_el_size;

        t_paquete *paqueteEnviarMemoria = serializar_io_memoria_read(estructuraMemoria);
        send_paquete(get_fd_memoria(), paqueteEnviarMemoria);
        free(estructuraMemoria.data);
        t_paquete *paqueteDeMemoria;
        recv_paquete(get_fd_memoria(), &paqueteDeMemoria);

        if (paqueteDeMemoria->codigo_operacion != OK_GRAL)
        {
            // log_error(logger, "MEMORIA NOS ENVIO CUALQUIER OPCODE");
            mem_clean_exit();
        }
        paquete_destroy(paqueteDeMemoria);
    }
    list_iterator_destroy(list_iter);
    free(ingreso);
    list_destroy_and_destroy_elements(estructura.lista_frames, free);
    t_paquete *paqueteEnviarKernel = paquete_new();
    paqueteEnviarKernel->codigo_operacion = IO_KERNEL_STDIN_READ; // VIVA LA ROCA Y SU GIRAR!
    return paqueteEnviarKernel;
}

t_paquete *resolver_KERNEL_IO_STDOUT_WRITE(t_paquete *paquete_reciv)
{
    t_config_io *cfg = get_config();
    t_log *logger = get_logger();

    s_kernel_io_stdout_write estructura = deserializar_kernel_io_stdout_write(paquete_reciv);
    paquete_destroy(paquete_reciv);
    log_info(logger, "IO : <%i> | PID : <%i> | OPERACION: <STDOUT_WRITE>", cfg->TIPO_INTERFAZ, estructura.pid);

    t_list_iterator *listIterrada = list_iterator_create(estructura.lista_frames);

    char *data_de_memoria = string_new();

    while (list_iterator_has_next(listIterrada))
    {
        frames_element *elemento = list_iterator_next(listIterrada);
        s_io_memoria_write estructuraMemoria;
        estructuraMemoria.pid = (int)elemento->pid;
        estructuraMemoria.size = (int)(elemento->tam_pag - elemento->offset - elemento->padding);
        estructuraMemoria.real_mem_dir = (int)(elemento->tam_pag * elemento->frame + elemento->offset);
        t_paquete *paqueteEnviarMemoria = serializar_io_memoria_write(estructuraMemoria);

#ifdef TEST

        paqueteEnviarMemoria_test = paqueteEnviarMemoria; // TODO(ahora tendríamos que evaluar una lista)
#else
        t_paquete *paqueteRecividoMemoria = paquete_new();
        log_trace(logger, "se envia un paquete pidiendo bytes a memoria");
        send_paquete(get_fd_memoria(), paqueteEnviarMemoria);
        recv_paquete(get_fd_memoria(), &paqueteRecividoMemoria);
        log_trace(logger, "se recibe un paquete con los bytes de memoria");

        s_memoria_io_write structRecividoMemoria = deserializar_m_i_write_struct(paqueteRecividoMemoria);

        // log_error(logger, "memoria nos mando para escribir: %s\n", structRecividoMemoria.value);
        // log_error(logger, "el size es: %i y deberia ser %i\n", strlen(structRecividoMemoria.value),
        //           structRecividoMemoria.size);

        string_append(&data_de_memoria, (char *)structRecividoMemoria.value);
#endif
    }

    printf("---------------------------------\n");
    printf("%s\n", data_de_memoria);
    printf("---------------------------------\n");
    free(data_de_memoria);
    list_destroy_and_destroy_elements(estructura.lista_frames, free);
    t_paquete *paqueteEnviarKernel = paquete_new();
    paqueteEnviarKernel->codigo_operacion = IO_KERNEL_STDOUT_WRITE;
    return paqueteEnviarKernel;
}

t_paquete *resolver_KERNEL_IO_FS_CREATE(t_paquete *paquete_reciv)
{
    t_config_io *cfg = get_config();
    t_log *logger = get_logger();
    const int mil = 1000;

    s_kernel_io_fs_create estructura = deserializar_kernel_io_fs_create(paquete_reciv);
    paquete_destroy(paquete_reciv);
    log_info(logger, "PID : <%i> | OPERACION: <FS_CREATE>", estructura.pid);
    log_info(logger, "PID : <%i> | Crear Archivo: <%s>", estructura.pid, estructura.file_name);

    sleep(cfg->TIEMPO_UNIDAD_TRABAJO / mil); // todas las operaciones de DIALFS consumen una unidad de trabajo

    crearArchivo(estructura.file_name); // por ahora asumimos que siempre hay un bloque libre para crearlo

    t_paquete *paqueteEnviarKernel = paquete_new();
    paqueteEnviarKernel->codigo_operacion = IO_KERNEL_FS_CREATE;
    return paqueteEnviarKernel;
}

t_paquete *resolver_KERNEL_IO_FS_DELETE(t_paquete *paquete_reciv)
{
    t_config_io *cfg = get_config();
    t_log *logger = get_logger();
    const int mil = 1000;

    s_kernel_io_fs_delete estructura = deserializar_kernel_io_fs_delete(paquete_reciv);
    paquete_destroy(paquete_reciv);
    log_info(logger, "PID : <%i> | OPERACION: <FS_DELETE>", estructura.pid);
    log_info(logger, "PID : <%i> | Eliminar Archivo: <%s>", estructura.pid, estructura.file_name);

    sleep(cfg->TIEMPO_UNIDAD_TRABAJO / mil); // todas las operaciones de DIALFS consumen una unidad de trabajo

    eliminarArchivo(estructura.file_name);

    t_paquete *paqueteEnviarKernel = paquete_new();
    paqueteEnviarKernel->codigo_operacion = IO_KERNEL_FS_DELETE;
    return paqueteEnviarKernel;
}

t_paquete *resolver_KERNEL_IO_FS_TRUNCATE(t_paquete *paquete_reciv)
{
    t_config_io *cfg = get_config();
    t_log *logger = get_logger();
    const int mil = 1000;

    s_kernel_io_fs_truncate estructura = deserializar_kernel_io_fs_truncate(paquete_reciv);
    paquete_destroy(paquete_reciv);
    log_info(logger, "PID : <%i> | OPERACION: <FS_TRUNCATE>", estructura.pid);
    log_info(logger, "PID : <%i> | Truncar Archivo: <%s> | Tamanio <%i>", estructura.pid, estructura.file_name,
             estructura.new_size);

    sleep(cfg->TIEMPO_UNIDAD_TRABAJO / mil); // todas las operaciones de DIALFS consumen una unidad de trabajo
    int blocks = (int)ceil((double)estructura.new_size / (double)cfg->BLOCK_SIZE);
    truncarArchivo(estructura.file_name, blocks);

    t_paquete *paqueteEnviarKernel = paquete_new();
    paqueteEnviarKernel->codigo_operacion = IO_KERNEL_FS_TRUNCATE;
    return paqueteEnviarKernel;
}

t_paquete *resolver_KERNEL_IO_FS_READ(t_paquete *paquete_reciv)
{
    t_config_io *cfg = get_config();
    t_log *logger = get_logger();
    const int mil = 1000;

    s_kernel_io_fs_read estructura = deserializar_kernel_io_fs_read(paquete_reciv);
    paquete_destroy(paquete_reciv);
    log_info(logger, "PID : <%i> | OPERACION: <FS_READ>", estructura.pid);
    log_info(logger, "PID : <%i> | LEER ARCHIVO: <%s> - TAMAÑO A LEER: <%i> - PUNTERO ARCHIVO <%i>", estructura.pid,
             estructura.file_name, estructura.size, estructura.file_pointer);

    sleep(cfg->TIEMPO_UNIDAD_TRABAJO / mil); // todas las operaciones de DIALFS consumen una unidad de trabajo

    // cargamos en MD_archivo la info del archivo
    t_MetaData *MD_archivo = initialize_MD();
    cargarMD(MD_archivo, estructura.file_name);

    // vamos a ver que la lectura sea valida
    if (estructura.file_pointer + estructura.size > (MD_archivo->CANT_BLOQUES * cfg->BLOCK_SIZE))
    {
        // log_error(logger, "El usuario quiere leer mas del tamaño del archivo, Eso NO se puede! Usuario malo castigado
        // "
        //                   "con un delay de 10 minuto");
        // sleep(mil);
        mem_clean_exit();
    }

    char *lectura = (char *)malloc(estructura.size);
    leerMemoria(MD_archivo->BLOQUE_INICIAL * cfg->BLOCK_SIZE + estructura.file_pointer, estructura.size, lectura);

    t_list_iterator *list_iter = list_iterator_create(estructura.info_marcos);
    int offset = 0;
    while (list_iterator_has_next(list_iter))
    {
        frames_element *curr_el = (frames_element *)list_iterator_next(list_iter);

        s_io_memoria_read estructuraMemoria;
        estructuraMemoria.pid = (int)curr_el->pid;
        estructuraMemoria.size = (int)(curr_el->tam_pag - curr_el->offset - curr_el->padding);
        estructuraMemoria.real_mem_dir = (int)(curr_el->tam_pag * curr_el->frame + curr_el->offset);

        uint32_t curr_el_size = curr_el->tam_pag - curr_el->offset - curr_el->padding;
        estructuraMemoria.data = string_substring_until(lectura + offset, (int)curr_el_size);
        offset += (int)curr_el_size;

        t_paquete *paqueteEnviarMemoria = serializar_io_memoria_read(estructuraMemoria);
        send_paquete(get_fd_memoria(), paqueteEnviarMemoria);
        t_paquete *paqueteDeMemoria;
        recv_paquete(get_fd_memoria(), &paqueteDeMemoria);

        if (paqueteDeMemoria->codigo_operacion != OK_GRAL)
        {
            // log_error(logger, "MEMORIA NOS ENVIO CUALQUIER OPCODE");
            mem_clean_exit();
        }
        paquete_destroy(paqueteDeMemoria);
    }
    list_iterator_destroy(list_iter);
    free(lectura);
    destroyMD(MD_archivo);

    free(estructura.file_name);
    list_destroy_and_destroy_elements(estructura.info_marcos, free);
    t_paquete *paqueteEnviarKernel = paquete_new();
    paqueteEnviarKernel->codigo_operacion = IO_KERNEL_FS_READ;
    return paqueteEnviarKernel;
}

t_paquete *resolver_KERNEL_IO_FS_WRITE(t_paquete *paquete_reciv)
{
    t_config_io *cfg = get_config();
    t_log *logger = get_logger();
    const int mil = 1000;

    s_kernel_io_fs_write estructura = deserializar_kernel_io_fs_write(paquete_reciv);
    paquete_destroy(paquete_reciv);
    log_info(logger, "PID : <%i> | OPERACION: <FS_WRITE>", estructura.pid);
    log_info(logger, "PID : <%i> | ESCRIBIR ARCHIVO: <%s> - TAMANIO A ESCRIBIR: <%i> - PUNTERO ARCHIVO <%i>",
             estructura.pid, estructura.file_name, estructura.size, estructura.file_pointer);

    sleep(cfg->TIEMPO_UNIDAD_TRABAJO / mil); // todas las operaciones de DIALFS consumen una unidad de trabajo

    // cargamos en MD_archivo la info del archivo
    t_MetaData *MD_archivo = initialize_MD();
    cargarMD(MD_archivo, estructura.file_name);

    // vamos a ver que la lectura sea valida
    if (estructura.file_pointer + estructura.size > (MD_archivo->CANT_BLOQUES * cfg->BLOCK_SIZE))
    {
        // log_error(logger, "El usuario quiere leer mas del tamaño del archivo, Eso NO se puede! Usuario malo castigado
        // "
        //                   "con un delay de 10 minuto");
        // sleep(mil);
        mem_clean_exit();
    }

    t_list_iterator *listIterrada = list_iterator_create(estructura.info_marcos);
    char *data_de_memoria = string_new();

    // armamos el char* que luego debemos escribir en BLOQUES
    while (list_iterator_has_next(listIterrada))
    {
        frames_element *elemento = list_iterator_next(listIterrada);
        s_io_memoria_write estructuraMemoria;
        estructuraMemoria.pid = (int)elemento->pid;
        estructuraMemoria.size = (int)(elemento->tam_pag - elemento->offset - elemento->padding);
        estructuraMemoria.real_mem_dir = (int)(elemento->tam_pag * elemento->frame + elemento->offset);
        t_paquete *paqueteEnviarMemoria = serializar_io_memoria_write(estructuraMemoria);

        t_paquete *paqueteRecividoMemoria;

        send_paquete(get_fd_memoria(), paqueteEnviarMemoria);
        recv_paquete(get_fd_memoria(), &paqueteRecividoMemoria);
        s_memoria_io_write structRecividoMemoria = deserializar_m_i_write_struct(paqueteRecividoMemoria);
        // log_error(logger, "memoria nos mando para escribir: %s\n", structRecividoMemoria.value);
        // log_error(logger, "el size es: %i y deberia ser %i\n", strlen(structRecividoMemoria.value),
        //   structRecividoMemoria.size);

        string_append(&data_de_memoria, (char *)structRecividoMemoria.value);
        paquete_destroy(paqueteRecividoMemoria);
        free(structRecividoMemoria.value); // libero e void*
    }

    // log_error(logger, "es string concatenado es: %s\n", data_de_memoria);
    // escribimos en BLOQUES
    // log_error(logger, "en el archivo vamos a escribir, %s de tamaño %i", data_de_memoria, estructura.size);

    escribirMemoria(MD_archivo->BLOQUE_INICIAL * cfg->BLOCK_SIZE + estructura.file_pointer, estructura.size,
                    data_de_memoria);

    destroyMD(MD_archivo);
    free(estructura.file_name);
    list_destroy_and_destroy_elements(estructura.info_marcos, free);
    t_paquete *paqueteEnviarKernel = paquete_new();
    paqueteEnviarKernel->codigo_operacion = IO_KERNEL_FS_WRITE;
    return paqueteEnviarKernel;
}