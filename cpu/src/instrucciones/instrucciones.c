#include "instrucciones.h"

#define DECIMAL 10
#define MODE_READ 0
#define MODE_WRITE 1

t_paquete *set(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <SET> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    uint32_t valor_entero = (uint32_t)strtol(argumentos_separados[1], NULL, 10);

    void (*set_registro)(uint32_t) = dictionary_get(get_diccionario_registros_set(), argumentos_separados[0]);
    set_registro(valor_entero);

    if (!(strcmp(argumentos_separados[0], "PC") == 0))
    {
        set_pc(get_pc() + 1);
    }

    string_array_destroy(argumentos_separados);
    return NULL;
}

t_paquete *sum(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <SUM> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");

    uint32_t (*get_registro_valor)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[1]);
    uint32_t valor = get_registro_valor();

    uint32_t (*get_registro_destino)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[0]);
    uint32_t valor_registro_destino = get_registro_destino();

    void (*set_registro)(uint32_t) = dictionary_get(get_diccionario_registros_set(), argumentos_separados[0]);
    set_registro(valor_registro_destino + valor);

    if (!(strcmp(argumentos_separados[0], "PC") == 0))
    {
        set_pc(get_pc() + 1);
    }

    string_array_destroy(argumentos_separados);
    return NULL;
}

t_paquete *sub(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <SUB> - <%s>", get_pid(), argumentos);

    char **argumentos_separados = string_split(argumentos, " ");

    uint32_t (*get_registro_valor)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[1]);
    uint32_t valor = get_registro_valor();

    uint32_t (*get_registro_destino)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[0]);
    uint32_t valor_registro_destino = get_registro_destino();

    void (*set_registro)(uint32_t) = dictionary_get(get_diccionario_registros_set(), argumentos_separados[0]);
    set_registro(valor_registro_destino - valor);

    if (!(strcmp(argumentos_separados[0], "PC") == 0))
    {
        set_pc(get_pc() + 1);
    }

    string_array_destroy(argumentos_separados);
    return NULL;
}

t_paquete *jnz(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <JNZ> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");

    uint32_t (*get_registro_valor)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[0]);
    uint32_t valor = get_registro_valor();

    if (valor != 0)
    {
        set_pc((uint32_t)strtol(argumentos_separados[1], NULL, 0));
        return NULL;
    }
    set_pc(get_pc() + 1);
    string_array_destroy(argumentos_separados);
    return NULL;
}

t_paquete *gen_io_sleep(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <IO_GEN_SLEEP> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);
    uint32_t unidades = (uint32_t)strtol(argumentos_separados[1], NULL, 0);
    s_cpu_kernel estructura = create_s_cpu_kernel_io_sleep(get_all(), argumentos_separados[0], unidades);
    t_paquete *paquete = serializar_cpu_kernel_io_sleep(estructura);
    string_array_destroy(argumentos_separados);
    return paquete;
}
t_paquete *nuestro_wait(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <WAIT> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);
    s_cpu_kernel estructura = create_s_cpu_kernel_wait(get_all(), argumentos_separados[0]);
    t_paquete *paquete = serializar_cpu_kernel_wait(estructura);
    string_array_destroy(argumentos_separados);
    return paquete;
}
t_paquete *nuestro_signal(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <SIGNAL> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);
    s_cpu_kernel estructura = create_s_cpu_kernel_signal(get_all(), argumentos_separados[0]);
    t_paquete *paquete = serializar_cpu_kernel_signal(estructura);
    string_array_destroy(argumentos_separados);
    return paquete;
}

t_paquete *resize(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <RESIZE> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    send_resize(get_pid(), (uint32_t)strtol(argumentos_separados[0], NULL, 0));
    bool respuesta = recv_resize();
    set_pc(get_pc() + 1);
    if (respuesta)
    {
        return NULL;
    }
    string_array_destroy(argumentos_separados);
    return crear_paquete_resize();
}

static uint32_t obtener_tamano_registro(char *registro)
{
    if (strcmp(registro, "AX") == 0 || strcmp(registro, "BX") == 0 || strcmp(registro, "CX") == 0 ||
        strcmp(registro, "DX") == 0)
    {
        return 1;
    }
    return 4;
}

t_paquete *mov_in(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <MOV_IN> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    // (Registro Datos, Registro Dirección)

    uint32_t (*get_registro_direccion)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[1]);
    uint32_t registro_direccion = get_registro_direccion();

    uint32_t data_size = obtener_tamano_registro(argumentos_separados[0]);
    assert(data_size > 0);

    void *bytes_leidos = malloc(data_size);
    uint32_t offset_bytes_leidos = 0;

    t_list *info_marcos = traducir_memoria(registro_direccion, data_size, get_pid());
    t_list_iterator *iterator_info_marcos = list_iterator_create(info_marcos);

    while (list_iterator_has_next(iterator_info_marcos))
    {
        frames_element *info_marco = list_iterator_next(iterator_info_marcos);
        uint32_t chunk_size = info_marco->tam_pag - info_marco->offset - info_marco->padding;
        uint32_t offset_en_marco = 0;

        for (; offset_en_marco < chunk_size; offset_en_marco++)
        {
            // uint8_t *data = bytes_escritos + offset_bytes_escritos;
            send_mov_in((info_marco->frame * info_marco->tam_pag) + info_marco->offset + offset_en_marco, 1);
            uint8_t porcion_bytes = recv_mov_in();
            memcpy(bytes_leidos + offset_bytes_leidos, &porcion_bytes, 1);
            log_info(get_logger(), "PID: <%d> - Acción: <LEER> - Dirección Física: <%d> - Valor: <%d>", get_pid(),
                     info_marco->frame + info_marco->offset, porcion_bytes);

            offset_bytes_leidos++;
        }

        // char *string_log = string_duplicate((char *)porcion_bytes);
        // string_log[chunk_size] = '\0';
    }

    list_destroy(info_marcos);

    if (data_size == 4)
    {
        void (*set_registro_datos)(uint32_t) = dictionary_get(get_diccionario_registros_set(), argumentos_separados[0]);
        set_registro_datos(*(uint32_t *)bytes_leidos);
    }
    else
    {
        void (*set_registro_datos)(uint8_t) = dictionary_get(get_diccionario_registros_set(), argumentos_separados[0]);
        set_registro_datos(*(uint8_t *)bytes_leidos);
    }

    free(bytes_leidos);

    list_iterator_destroy(iterator_info_marcos);
    if (!(strcmp(argumentos_separados[0], "PC") == 0))
    {
        set_pc(get_pc() + 1);
    }

    string_array_destroy(argumentos_separados);
    return NULL;
}

static void destroy_info_marco(frames_element *info_marco)
{
    free(info_marco->data);
    info_marco->data = NULL;
}

t_paquete *mov_out(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <MOV_OUT> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");

    // (Registro Dirección, Registro Datos)
    uint32_t (*get_registro_direccion)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[0]);
    uint32_t registro_direccion = get_registro_direccion();

    uint32_t data_size = obtener_tamano_registro(argumentos_separados[1]);
    assert(data_size > 0);

    uint8_t contenido_registro_8t;
    uint32_t contenido_registro_32t;
    void *bytes_escritos;

    if (data_size == 4)
    {
        uint32_t (*get_registro_datos)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[1]);
        uint32_t registro_datos = get_registro_datos();
        contenido_registro_32t = registro_datos;
        bytes_escritos = &contenido_registro_32t;
    }
    else
    {
        uint8_t (*get_registro_datos)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[1]);
        uint8_t registro_datos = get_registro_datos();
        contenido_registro_8t = registro_datos;
        bytes_escritos = &contenido_registro_8t;
    }

    uint32_t offset_bytes_escritos = 0;

    t_list *info_marcos = traducir_memoria(registro_direccion, data_size, get_pid());
    t_list_iterator *iterator_info_marcos = list_iterator_create(info_marcos);

    while (list_iterator_has_next(iterator_info_marcos))
    {
        frames_element *info_marco = list_iterator_next(iterator_info_marcos);
        uint32_t chunk_size = info_marco->tam_pag - info_marco->offset - info_marco->padding;
        // chunk_size += offset_bytes_escritos;
        uint32_t offset_en_marco = 0;

        for (; offset_en_marco < chunk_size; offset_en_marco++)
        {
            uint8_t *data = bytes_escritos + offset_bytes_escritos;

            log_info(get_logger(), "PID: <%d> - Acción: <ESCRIBIR> - Dirección Física: <%d> - Valor: <%d>", get_pid(),
                     info_marco->frame * get_tam_pag() + info_marco->offset + offset_en_marco, *data);

            t_paquete *movout_status;
            send_mov_out((info_marco->frame * info_marco->tam_pag) + info_marco->offset + offset_en_marco, 1, *data);
            recv_paquete(get_fd_memoria(), &movout_status);
            assert(movout_status->codigo_operacion == OK_GRAL);
            paquete_destroy(movout_status);

            offset_bytes_escritos++;
        }

        // info_marco->data = malloc(sizeof(chunk_size));
        // memcpy(info_marco->data, bytes_escritos + offset_bytes_escritos, chunk_size);
    }

    list_iterator_destroy(iterator_info_marcos);
    // list_destroy_and_destroy_elements(info_marcos, (void *)destroy_info_marco);
    list_destroy(info_marcos);

    // free(bytes_escritos);
    string_array_destroy(argumentos_separados);
    set_pc(get_pc() + 1);
    return NULL;
}

t_paquete *io_fs_truncate(char *argumentos)
{
    log_info(get_logger(), "PID: %i - Ejecutando: IO_FS_TRUNCATE - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);
    uint32_t (*getter_registro)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[2]);
    uint32_t tamanio = getter_registro();
    s_cpu_kernel estructura =
        create_s_cpu_kernel_io_fs_truncate(get_all(), argumentos_separados[0], tamanio, argumentos_separados[1]);
    t_paquete *paquete = serializar_cpu_kernel_io_fs_truncate(estructura);
    string_array_destroy(argumentos_separados);
    return paquete;
}

t_paquete *io_fs_delete(char *argumentos)
{
    log_info(get_logger(), "PID: %i - Ejecutando: IO_FS_DELETE - %s", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);
    s_cpu_kernel estructura =
        create_s_cpu_kernel_io_fs_delete(argumentos_separados[0], get_all(), argumentos_separados[1]);
    t_paquete *paquete = serializar_cpu_kernel_io_fs_delete(estructura);
    string_array_destroy(argumentos_separados);
    return paquete;
}

t_paquete *io_fs_create(char *argumentos)
{
    log_info(get_logger(), "PID: %i - Ejecutando: IO_FS_CREATE - %s", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);
    s_cpu_kernel estructura =
        create_s_cpu_kernel_io_fs_create(argumentos_separados[0], get_all(), argumentos_separados[1]);
    t_paquete *paquete = serializar_cpu_kernel_io_fs_create(estructura);
    string_array_destroy(argumentos_separados);
    return paquete;
}

t_paquete *io_stdin_read(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <IO_STDIN_READ> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);

    uint32_t (*get_registro_direccion)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[1]);
    uint32_t direccion_logica = get_registro_direccion();
    uint32_t (*get_registro_tamano)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[2]);
    uint32_t tamano_read = get_registro_tamano();

    t_list *listado_frames = traducir_memoria(direccion_logica, tamano_read, get_pid());

    s_cpu_kernel estructura =
        create_s_cpu_kernel_io_stdin(get_all(), argumentos_separados[0], listado_frames, tamano_read);
    t_paquete *paquete = serializar_cpu_kernel_io_stdin(estructura);
    return paquete;
}

t_paquete *io_stdout_write(char *argumentos)
{
    log_info(get_logger(), "PID: <%i> - Ejecutando: <IO_STDOUT_WRITE> - <%s>", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);

    uint32_t (*get_registro_direccion)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[1]);
    uint32_t direccion_logica = get_registro_direccion();
    uint32_t (*get_registro_tamano)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[2]);
    uint32_t tamano_read = get_registro_tamano();

    t_list *listado_frames = traducir_memoria(direccion_logica, tamano_read, get_pid());

    s_cpu_kernel estructura =
        create_s_cpu_kernel_io_stdin(get_all(), argumentos_separados[0], listado_frames, tamano_read);
    t_paquete *paquete = serializar_cpu_kernel_io_stdin(estructura);
    paquete->codigo_operacion = CPU_KERNEL_DESALOJO_IO_STDOUT_WRITE;
    return paquete;
}

t_paquete *copy_string(char *argumentos)
{
    log_info(get_logger(), "PID: %i - Ejecutando: COPY_STRING - %s", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");

    uint32_t size = (uint32_t)strtol(argumentos_separados[0], NULL, DECIMAL);

    uint32_t (*get_SI)() = dictionary_get(get_diccionario_registros_get(), "SI");
    t_list *dir_reales_si = traducir_memoria(get_SI(), size, get_pid());

    uint32_t (*get_DI)() = dictionary_get(get_diccionario_registros_get(), "DI");
    t_list *dir_reales_di = traducir_memoria(get_DI(), size, get_pid());

    send_copy_string(get_pid(), dir_reales_si, dir_reales_di, size); // TODO(all): que pasa si rompe pq memoria murió?

    set_pc(get_pc() + 1);
    return NULL;
}

static t_paquete *armar_paquete_fs(char **argumentos_separados, int modo)
{
    // (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo)
    uint32_t (*get_registro_tamanio)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[3]);
    uint32_t tamanio = get_registro_tamanio();

    uint32_t (*get_registro_puntero)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[4]);
    uint32_t puntero = get_registro_puntero();

    uint32_t (*get_registro_direccion)() = dictionary_get(get_diccionario_registros_get(), argumentos_separados[2]);
    uint32_t registro_direccion = get_registro_direccion();

    t_list *info_marcos = traducir_memoria(registro_direccion, tamanio, get_pid());

    if (modo == MODE_WRITE)
    {
        s_cpu_kernel estructura = create_s_cpu_kernel_io_fs_write(argumentos_separados[0], tamanio, get_all(),
                                                                  info_marcos, argumentos_separados[1], puntero);
        return serializar_cpu_kernel_io_fs_write(estructura);
    }

    s_cpu_kernel estructura = create_s_cpu_kernel_io_fs_read(argumentos_separados[0], tamanio, get_all(), info_marcos,
                                                             argumentos_separados[1], puntero);
    return serializar_cpu_kernel_io_fs_read(estructura);

    // ver donde tendria q hacer los list destroy
}

t_paquete *io_fs_read(char *argumentos)
{
    log_info(get_logger(), "PID: %i - Ejecutando: IO_FS_READ - %s", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);

    return armar_paquete_fs(argumentos_separados, MODE_READ);
}

t_paquete *io_fs_write(char *argumentos)
{
    log_info(get_logger(), "PID: %i - Ejecutando: IO_FS_WRITE - %s", get_pid(), argumentos);
    char **argumentos_separados = string_split(argumentos, " ");
    set_pc(get_pc() + 1);

    return armar_paquete_fs(argumentos_separados, MODE_WRITE);
}