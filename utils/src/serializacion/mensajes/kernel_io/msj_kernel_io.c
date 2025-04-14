#include "msj_kernel_io.h"

// Begin: =============== IO_GEN_SLEEP ================== //

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_kernel_io_sleep create_kernel_io_sleep_struct(uint32_t pid, uint32_t work_unit)
{
    s_kernel_io_sleep s_knel_io_sleep = {.pid = pid, .work_unit = work_unit};
    return s_knel_io_sleep;
}

t_paquete *serializar_kernel_io_sleep_struct(s_kernel_io_sleep s_knel_io_sleep)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = KERNEL_IO_GEN_ZZZ;

    serializar_uint32_t(&paquete->buffer, s_knel_io_sleep.pid);       // 1. PID
    serializar_uint32_t(&paquete->buffer, s_knel_io_sleep.work_unit); // 2. work_unit

    return paquete;
}

s_kernel_io_sleep deserializar_kernel_io_sleep_struct(t_paquete *paquete)
{
    assert(paquete->codigo_operacion == KERNEL_IO_GEN_ZZZ); // esto está para debugear, no es runtime check
    s_kernel_io_sleep s_knel_io_sleep;

    deserializar_uint32_t(paquete->buffer, &s_knel_io_sleep.pid);
    deserializar_uint32_t(paquete->buffer, &s_knel_io_sleep.work_unit);

    return s_knel_io_sleep;
}

// End: =============== IO_GEN_SLEEP ================== //

// Begin: =============== IO_STDIN_READ ================== //

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_kernel_io_stdin_read create_kernel_io_stdin_read(uint32_t pid, t_list *lista_frames, uint32_t size)
{
    s_kernel_io_stdin_read s_knel_io_stdin_read;
    s_knel_io_stdin_read.pid = pid;
    s_knel_io_stdin_read.size = size;
    s_knel_io_stdin_read.lista_frames = lista_frames;

    return s_knel_io_stdin_read;
}

t_paquete *serializar_kernel_io_stdin_read(s_kernel_io_stdin_read s_knel_io_stdin_read)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = KERNEL_IO_STDIN_READ;

    serializar_uint32_t(&paquete->buffer, s_knel_io_stdin_read.pid);  // 1. PID
    serializar_uint32_t(&paquete->buffer, s_knel_io_stdin_read.size); // 2. SIZE
    serializar_t_list(&paquete->buffer, s_knel_io_stdin_read.lista_frames,
                      (void *)serializar_elem_list_frames); // 3. Lista

    return paquete;
}

s_kernel_io_stdin_read deserializar_kernel_io_stdin_read(t_paquete *paquete)
{
    assert(paquete->codigo_operacion == KERNEL_IO_STDIN_READ); // esto está para debugear, no es runtime check

    s_kernel_io_stdin_read s_knel_io_stdin_read;

    deserializar_uint32_t(paquete->buffer, &s_knel_io_stdin_read.pid);  // 1. PID
    deserializar_uint32_t(paquete->buffer, &s_knel_io_stdin_read.size); // 2. SIZE
    s_knel_io_stdin_read.lista_frames = list_create();
    deserializar_t_list(paquete->buffer, &s_knel_io_stdin_read.lista_frames,
                        (void *)deserializar_elem_list_frames); // 3. Lista

    return s_knel_io_stdin_read;
}

// End: =============== IO_STDIN_READ ================== //

// Begin: =============== IO_STDOUT_WRITE ================== //

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_kernel_io_stdout_write create_kernel_io_stdout_write(uint32_t pid, t_list *lista_frames)
{
    s_kernel_io_stdout_write s_knel_io_stdout_write;
    s_knel_io_stdout_write.pid = pid;
    s_knel_io_stdout_write.lista_frames = lista_frames;
    return s_knel_io_stdout_write;
}

t_paquete *serializar_kernel_io_stdout_write(s_kernel_io_stdout_write s_knel_io_stdout_write)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = KERNEL_IO_STDOUT_WRITE;

    serializar_uint32_t(&paquete->buffer, s_knel_io_stdout_write.pid); // 1. PID
    serializar_t_list(&paquete->buffer, s_knel_io_stdout_write.lista_frames,
                      (void *)serializar_elem_list_frames); // 2. LISTA

    return paquete;
}

s_kernel_io_stdout_write deserializar_kernel_io_stdout_write(t_paquete *paquete)
{
    assert(paquete->codigo_operacion == KERNEL_IO_STDOUT_WRITE); // esto está para debugear, no es runtime check
    s_kernel_io_stdout_write s_knel_io_stdout_write = create_kernel_io_stdout_write(0, list_create());
    deserializar_uint32_t(paquete->buffer, &s_knel_io_stdout_write.pid); // 1. PID
    s_knel_io_stdout_write.lista_frames = list_create();
    deserializar_t_list(paquete->buffer, &s_knel_io_stdout_write.lista_frames,
                        (void *)deserializar_elem_list_frames); // 2. LISTA
    return s_knel_io_stdout_write;
}

// End: =============== IO_STDOUT_WRITE ================== //

// Begin: =============== IO_FS_CREATE ================== //

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_kernel_io_fs_create create_kernel_io_fs_create(uint32_t pid, char *file_name)
{
    s_kernel_io_fs_create s_knel_io_fs_create;
    s_knel_io_fs_create.pid = pid;
    s_knel_io_fs_create.file_name = file_name;
    return s_knel_io_fs_create;
}

t_paquete *serializar_kernel_io_fs_create(s_kernel_io_fs_create s_knel_io_fs_create)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = KERNEL_IO_FS_CREATE;

    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_create.pid);     // 1. PID
    serializar_string(&paquete->buffer, s_knel_io_fs_create.file_name); // 2. Nombre del archivo

    return paquete;
}

s_kernel_io_fs_create deserializar_kernel_io_fs_create(t_paquete *paquete)
{

    assert(paquete->codigo_operacion == KERNEL_IO_FS_CREATE); // esto está para debugear, no es runtime check

    s_kernel_io_fs_create s_knel_io_fs_create;

    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_create.pid);     // 1. PID
    deserializar_string(paquete->buffer, &s_knel_io_fs_create.file_name); // 2. Nombre del archivo

    return s_knel_io_fs_create;
}

// End: =============== IO_FS_CREATE ================== //

// Begin: =============== IO_FS_DELETE ================== //

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_kernel_io_fs_delete create_kernel_io_fs_delete(uint32_t pid, char *file_name)
{
    s_kernel_io_fs_delete s_knel_io_fs_delete;
    s_knel_io_fs_delete.pid = pid;
    s_knel_io_fs_delete.file_name = file_name;

    return s_knel_io_fs_delete;
}

void destroy_kernel_io_fs_delete(s_kernel_io_fs_delete *s_knel_io_fs_delete)
{
    free(s_knel_io_fs_delete->file_name);
    s_knel_io_fs_delete->file_name = NULL;
}

t_paquete *serializar_kernel_io_fs_delete(s_kernel_io_fs_delete s_knel_io_fs_delete)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = KERNEL_IO_FS_DELETE;

    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_delete.pid);     // 1. PID
    serializar_string(&paquete->buffer, s_knel_io_fs_delete.file_name); // 2. Nombre del archivo

    return paquete;
}

s_kernel_io_fs_delete deserializar_kernel_io_fs_delete(t_paquete *paquete)
{

    assert(paquete->codigo_operacion == KERNEL_IO_FS_DELETE); // esto está para debugear, no es runtime check

    s_kernel_io_fs_delete s_knel_io_fs_delete;

    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_delete.pid);     // 1. PID
    deserializar_string(paquete->buffer, &s_knel_io_fs_delete.file_name); // 2. Nombre del archivo

    return s_knel_io_fs_delete;
}

// End: =============== IO_FS_DELETE ================== //

// Begin: =============== IO_FS_TRUNCATE ================== //

s_kernel_io_fs_truncate create_kernel_io_fs_truncate(uint32_t pid, char *file_name, uint32_t new_size)
{
    s_kernel_io_fs_truncate s_knel_io_fs_truncate;
    s_knel_io_fs_truncate.pid = pid;
    s_knel_io_fs_truncate.file_name = file_name;
    s_knel_io_fs_truncate.new_size = new_size;

    return s_knel_io_fs_truncate;
}

t_paquete *serializar_kernel_io_fs_truncate(s_kernel_io_fs_truncate s_knel_io_fs_truncate)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = KERNEL_IO_FS_TRUNCATE;

    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_truncate.pid);      // 1. PID
    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_truncate.new_size); // 2. Nuevo Tamaño
    serializar_string(&paquete->buffer, s_knel_io_fs_truncate.file_name);  // 3. Nombre del archivo

    return paquete;
}

s_kernel_io_fs_truncate deserializar_kernel_io_fs_truncate(t_paquete *paquete)
{
    assert(paquete->codigo_operacion == KERNEL_IO_FS_TRUNCATE); // esto está para debugear, no es runtime check

    s_kernel_io_fs_truncate s_knel_io_fs_truncate;

    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_truncate.pid);      // 1. PID
    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_truncate.new_size); // 2. Nuevo Tamaño
    deserializar_string(paquete->buffer, &s_knel_io_fs_truncate.file_name);  // 3. Nombre del archivo

    return s_knel_io_fs_truncate;
}

// End: =============== IO_FS_TRUNCATE ================== //

// Begin: =============== IO_FS_WRITE ================== //

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_kernel_io_fs_write create_kernel_io_fs_write(uint32_t pid, char *file_name, uint32_t file_pointer,
                                               t_list *info_marcos, uint32_t size)
{
    s_kernel_io_fs_write s_knel_io_fs_write;
    s_knel_io_fs_write.pid = pid;
    s_knel_io_fs_write.file_name = file_name;
    s_knel_io_fs_write.file_pointer = file_pointer;
    s_knel_io_fs_write.info_marcos = info_marcos;
    s_knel_io_fs_write.size = size;

    return s_knel_io_fs_write;
}

t_paquete *serializar_kernel_io_fs_write(s_kernel_io_fs_write s_knel_io_fs_write)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = KERNEL_IO_FS_WRITE;

    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_write.pid);          // 1. PID
    serializar_string(&paquete->buffer, s_knel_io_fs_write.file_name);      // 2. Nombre del archivo
    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_write.file_pointer); // 3. Direccion dentro del archivo
    serializar_t_list(&paquete->buffer, s_knel_io_fs_write.info_marcos, serializar_frames_element_fs); // 4. Info marcos
    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_write.size); // 5. Tamaño del valor a escribir

    return paquete;
}

s_kernel_io_fs_write deserializar_kernel_io_fs_write(t_paquete *paquete)
{
    assert(paquete->codigo_operacion == KERNEL_IO_FS_WRITE); // esto está para debugear, no es runtime check

    s_kernel_io_fs_write s_knel_io_fs_write;
    s_knel_io_fs_write.info_marcos = list_create();

    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_write.pid);          // 1. PID
    deserializar_string(paquete->buffer, &s_knel_io_fs_write.file_name);      // 2. Nombre del archivo
    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_write.file_pointer); // 3. Direccion dentro del archivo
    deserializar_t_list(paquete->buffer, &s_knel_io_fs_write.info_marcos,
                        deserializar_frames_element_fs);              // 4. Info marcos
    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_write.size); // 5. Tamaño del valor a escribir

    return s_knel_io_fs_write;
}

// End: =============== IO_FS_WRITE ================== //

// Begin: =============== IO_FS_READ ================== //

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
s_kernel_io_fs_read create_kernel_io_fs_read(uint32_t pid, char *file_name, uint32_t file_pointer, t_list *info_marcos,
                                             uint32_t size)
{
    s_kernel_io_fs_read s_knel_io_fs_read;
    s_knel_io_fs_read.pid = pid;
    s_knel_io_fs_read.file_name = file_name;
    s_knel_io_fs_read.file_pointer = file_pointer;
    s_knel_io_fs_read.info_marcos = info_marcos;
    s_knel_io_fs_read.size = size;

    return s_knel_io_fs_read;
}

t_paquete *serializar_kernel_io_fs_read(s_kernel_io_fs_read s_knel_io_fs_read)
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = KERNEL_IO_FS_READ;

    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_read.pid);          // 1. PID
    serializar_string(&paquete->buffer, s_knel_io_fs_read.file_name);      // 2. Nombre del archivo
    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_read.file_pointer); // 3. Direccion dentro del archivo
    serializar_t_list(&paquete->buffer, s_knel_io_fs_read.info_marcos,
                      serializar_frames_element_fs);               // 4. Info marcos
    serializar_uint32_t(&paquete->buffer, s_knel_io_fs_read.size); // 5. Tamaño del valor a escribir

    return paquete;
}

s_kernel_io_fs_read deserializar_kernel_io_fs_read(t_paquete *paquete)
{

    assert(paquete->codigo_operacion == KERNEL_IO_FS_READ); // esto está para debugear, no es runtime check
    s_kernel_io_fs_read s_knel_io_fs_read;
    s_knel_io_fs_read.info_marcos = list_create();

    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_read.pid);          // 1. PID
    deserializar_string(paquete->buffer, &s_knel_io_fs_read.file_name);      // 2. Nombre del archivo
    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_read.file_pointer); // 3. Direccion dentro del archivo
    deserializar_t_list(paquete->buffer, &s_knel_io_fs_read.info_marcos,
                        deserializar_frames_element_fs);             // 4. Info marcos
    deserializar_uint32_t(paquete->buffer, &s_knel_io_fs_read.size); // 5. Tamaño del valor a escribir

    return s_knel_io_fs_read;
}

// End: =============== IO_FS_READ ================== //
