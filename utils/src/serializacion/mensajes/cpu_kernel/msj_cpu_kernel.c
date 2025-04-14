#include "msj_cpu_kernel.h"

void serializar_contexto_ejecucion(t_buffer **buffer, t_contexto_ejecucion ctx)
{
    serializar_uint32_t(buffer, ctx.pid);
    serializar_uint32_t(buffer, ctx.registros_cpu.PC);
    serializar_uint8_t(buffer, ctx.registros_cpu.AX);
    serializar_uint8_t(buffer, ctx.registros_cpu.BX);
    serializar_uint8_t(buffer, ctx.registros_cpu.CX);
    serializar_uint8_t(buffer, ctx.registros_cpu.DX);
    serializar_uint32_t(buffer, ctx.registros_cpu.EAX);
    serializar_uint32_t(buffer, ctx.registros_cpu.EBX);
    serializar_uint32_t(buffer, ctx.registros_cpu.ECX);
    serializar_uint32_t(buffer, ctx.registros_cpu.EDX);
    serializar_uint32_t(buffer, ctx.registros_cpu.SI);
    serializar_uint32_t(buffer, ctx.registros_cpu.DI);
}
t_contexto_ejecucion deserializar_contexto_ejecucion(t_paquete *pkg)
{
    t_contexto_ejecucion temp_contexto;
    deserializar_uint32_t(pkg->buffer, &temp_contexto.pid);
    deserializar_uint32_t(pkg->buffer, &temp_contexto.registros_cpu.PC);
    deserializar_uint8_t(pkg->buffer, &temp_contexto.registros_cpu.AX);
    deserializar_uint8_t(pkg->buffer, &temp_contexto.registros_cpu.BX);
    deserializar_uint8_t(pkg->buffer, &temp_contexto.registros_cpu.CX);
    deserializar_uint8_t(pkg->buffer, &temp_contexto.registros_cpu.DX);
    deserializar_uint32_t(pkg->buffer, &temp_contexto.registros_cpu.EAX);
    deserializar_uint32_t(pkg->buffer, &temp_contexto.registros_cpu.EBX);
    deserializar_uint32_t(pkg->buffer, &temp_contexto.registros_cpu.ECX);
    deserializar_uint32_t(pkg->buffer, &temp_contexto.registros_cpu.EDX);
    deserializar_uint32_t(pkg->buffer, &temp_contexto.registros_cpu.SI);
    deserializar_uint32_t(pkg->buffer, &temp_contexto.registros_cpu.DI);
    return temp_contexto;
}

t_interrupt deserializar_interrupcion(t_paquete *pkg)
{
    t_interrupt temp_interrupcion;
    temp_interrupcion.interrupt_happend = true;

    deserializar_uint32_t(pkg->buffer, (uint32_t *)&temp_interrupcion.pid);

    switch (pkg->codigo_operacion)
    {
    case KERNEL_CPU_INT_KILL:
        temp_interrupcion.type = KILL;
        break;
    case KERNEL_CPU_INT_DESALOJO:
        temp_interrupcion.type = DESALOJO;
        break;
    default:
        assert((pkg->codigo_operacion == KERNEL_CPU_INT_KILL || pkg->codigo_operacion == KERNEL_CPU_INT_DESALOJO) &&
               "Hubo un error con el opcode recibido (cpu Interrupt)");
        break;
    }

    return temp_interrupcion;
}

s_cpu_kernel create_s_cpu_kernel_signal(t_contexto_ejecucion ctx, char *recurso)
{
    s_cpu_kernel s_cpu_kernel_signal;
    s_cpu_kernel_signal.contexto = ctx;
    s_cpu_kernel_signal.solicitud_recurso.args_signal = recurso;
    return s_cpu_kernel_signal;
}
t_paquete *serializar_cpu_kernel_signal(s_cpu_kernel s_cpu_kernel_signal)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_SIGNAL;
    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_signal.contexto);
    serializar_string(&pkg->buffer, s_cpu_kernel_signal.solicitud_recurso.args_signal);
    return pkg;
}
s_cpu_kernel deserializar_cpu_kernel_signal(t_paquete *pkg)
{
    s_cpu_kernel s_cpu_kernel_signal;
    s_cpu_kernel_signal.contexto = deserializar_contexto_ejecucion(pkg);
    deserializar_string(pkg->buffer, &s_cpu_kernel_signal.solicitud_recurso.args_signal);
    return s_cpu_kernel_signal;
}

s_cpu_kernel create_s_cpu_kernel_wait(t_contexto_ejecucion ctx, char *recurso)
{
    s_cpu_kernel s_cpu_kernel_wait;
    s_cpu_kernel_wait.contexto = ctx;
    s_cpu_kernel_wait.solicitud_recurso.args_wait = recurso;
    return s_cpu_kernel_wait;
}
t_paquete *serializar_cpu_kernel_wait(s_cpu_kernel s_cpu_kernel_wait)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_WAIT;
    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_wait.contexto);
    serializar_string(&pkg->buffer, s_cpu_kernel_wait.solicitud_recurso.args_wait);
    return pkg;
}
s_cpu_kernel deserializar_cpu_kernel_wait(t_paquete *pkg)
{
    s_cpu_kernel s_cpu_kernel_wait;
    s_cpu_kernel_wait.contexto = deserializar_contexto_ejecucion(pkg);
    deserializar_string(pkg->buffer, &s_cpu_kernel_wait.solicitud_recurso.args_wait);
    return s_cpu_kernel_wait;
}

s_cpu_kernel create_s_cpu_kernel_io_sleep(t_contexto_ejecucion ctx, char *io_name, uint32_t unidades_trabajo)
{
    s_cpu_kernel s_cpu_kernel_io_sleep;
    s_cpu_kernel_io_sleep.contexto = ctx;
    s_cpu_kernel_io_sleep.solicitud_recurso.args_io_sleep.nombre_io = io_name;
    s_cpu_kernel_io_sleep.solicitud_recurso.args_io_sleep.unidades_trabajo = unidades_trabajo;
    return s_cpu_kernel_io_sleep;
}
t_paquete *serializar_cpu_kernel_io_sleep(s_cpu_kernel s_cpu_kernel_io_sleep)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_IO_GEN_ZZZ;
    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_io_sleep.contexto);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_sleep.solicitud_recurso.args_io_sleep.nombre_io);
    serializar_uint32_t(&pkg->buffer, s_cpu_kernel_io_sleep.solicitud_recurso.args_io_sleep.unidades_trabajo);
    return pkg;
}
s_cpu_kernel deserializar_cpu_kernel_io_sleep(t_paquete *pkg)
{
    s_cpu_kernel s_cpu_kernel_io_sleep;
    s_cpu_kernel_io_sleep.contexto = deserializar_contexto_ejecucion(pkg);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_sleep.solicitud_recurso.args_io_sleep.nombre_io);
    deserializar_uint32_t(pkg->buffer, &s_cpu_kernel_io_sleep.solicitud_recurso.args_io_sleep.unidades_trabajo);

    return s_cpu_kernel_io_sleep;
}

s_cpu_kernel create_s_cpu_kernel_io_stdin(t_contexto_ejecucion ctx, char *nombreIO, t_list *lista_frames, uint32_t size)
{
    s_cpu_kernel s_cpu_kernel_io_stdin;
    s_cpu_kernel_io_stdin.contexto = ctx;
    s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.nombre_io = nombreIO;
    s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.size = size;
    s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.lista_frames = lista_frames;
    return s_cpu_kernel_io_stdin;
}

t_paquete *serializar_cpu_kernel_io_stdin(s_cpu_kernel s_cpu_kernel_io_stdin)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_IO_STDIN_READ;

    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_io_stdin.contexto);                        // 1. contexto
    serializar_string(&pkg->buffer, s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.nombre_io);   // 2. nombre
    serializar_uint32_t(&pkg->buffer, s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.size);      // 3. size
    serializar_t_list(&pkg->buffer, s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.lista_frames, // 4. frame list
                      (void *)serializar_elem_list_frames);

    return pkg;
}

s_cpu_kernel deserializar_cpu_kernel_io_stdin(t_paquete *pkg)
{
    assert(pkg->codigo_operacion == CPU_KERNEL_DESALOJO_IO_STDIN_READ);

    s_cpu_kernel s_cpu_kernel_io_stdin;

    s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.lista_frames = list_create();

    s_cpu_kernel_io_stdin.contexto = deserializar_contexto_ejecucion(pkg);                              // 1. contexto
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.nombre_io); // 2. nombre
    deserializar_uint32_t(pkg->buffer, &s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.size);    // 3. size
    deserializar_t_list(pkg->buffer,                                                                    // 4. frame list
                        &s_cpu_kernel_io_stdin.solicitud_recurso.args_io_stdin.lista_frames,
                        (void *)deserializar_elem_list_frames);
    return s_cpu_kernel_io_stdin;
}

// s_cpu_kernel create_s_cpu_kernel_io_stdout(t_contexto_ejecucion ctx, uint32_t direccion_traducida, char *io_name,
//                                            uint32_t tamano)
// {
//     s_cpu_kernel s_cpu_kernel_io_stdout;
//     s_cpu_kernel_io_stdout.contexto = ctx;
//     s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdout.nombre_io = io_name;
//     s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdout.direccion_traducida = direccion_traducida;
//     s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdout.tamano = tamano;
//     return s_cpu_kernel_io_stdout;
// }

t_paquete *serializar_cpu_kernel_io_stdout(s_cpu_kernel s_cpu_kernel_io_stdout)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_IO_STDOUT_WRITE;

    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_io_stdout.contexto);                       // 1. contexto
    serializar_string(&pkg->buffer, s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdout.nombre_io); // 2. nombre
    serializar_uint32_t(&pkg->buffer, s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdout.size);    // 3. size
    serializar_t_list(&pkg->buffer,                                                                     // 4. frame list
                      s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdout.lista_frames,
                      (void *)serializar_elem_list_frames);

    return pkg;
}
s_cpu_kernel deserializar_cpu_kernel_io_stdout(t_paquete *pkg)
{

    assert(pkg->codigo_operacion == CPU_KERNEL_DESALOJO_IO_STDOUT_WRITE);

    s_cpu_kernel s_cpu_kernel_io_stdout;
    s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdin.lista_frames = list_create();

    s_cpu_kernel_io_stdout.contexto = deserializar_contexto_ejecucion(pkg);                               // 1. contexto
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdout.nombre_io); // 2. nombre
    deserializar_uint32_t(pkg->buffer, &s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdout.size);    // 3. size
    deserializar_t_list(pkg->buffer, // 4. frame list
                        &s_cpu_kernel_io_stdout.solicitud_recurso.args_io_stdout.lista_frames,
                        (void *)deserializar_elem_list_frames);

    return s_cpu_kernel_io_stdout;
}

s_cpu_kernel create_s_cpu_kernel_io_fs_create(char *io_name, t_contexto_ejecucion ctx, char *nombre_archivo)
{
    s_cpu_kernel s_cpu_kernel_io_fs_create;
    s_cpu_kernel_io_fs_create.contexto = ctx;
    s_cpu_kernel_io_fs_create.solicitud_recurso.args_io_fs_create.nombre_io = io_name;
    s_cpu_kernel_io_fs_create.solicitud_recurso.args_io_fs_create.nombre_archivo = nombre_archivo;
    return s_cpu_kernel_io_fs_create;
}
t_paquete *serializar_cpu_kernel_io_fs_create(s_cpu_kernel s_cpu_kernel_io_fs_create)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_IO_FS_CREATE;
    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_io_fs_create.contexto);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_create.solicitud_recurso.args_io_fs_create.nombre_io);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_create.solicitud_recurso.args_io_fs_create.nombre_archivo);
    return pkg;
}
s_cpu_kernel deserializar_cpu_kernel_io_fs_create(t_paquete *pkg)
{
    s_cpu_kernel s_cpu_kernel_io_fs_create;
    s_cpu_kernel_io_fs_create.contexto = deserializar_contexto_ejecucion(pkg);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_create.solicitud_recurso.args_io_fs_create.nombre_io);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_create.solicitud_recurso.args_io_fs_create.nombre_archivo);
    return s_cpu_kernel_io_fs_create;
}

s_cpu_kernel create_s_cpu_kernel_io_fs_delete(char *io_name, t_contexto_ejecucion ctx, char *nombre_archivo)
{
    s_cpu_kernel s_cpu_kernel_io_fs_delete;
    s_cpu_kernel_io_fs_delete.contexto = ctx;
    s_cpu_kernel_io_fs_delete.solicitud_recurso.args_io_fs_delete.nombre_io = io_name;
    s_cpu_kernel_io_fs_delete.solicitud_recurso.args_io_fs_delete.nombre_archivo = nombre_archivo;
    return s_cpu_kernel_io_fs_delete;
}
t_paquete *serializar_cpu_kernel_io_fs_delete(s_cpu_kernel s_cpu_kernel_io_fs_delete)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_IO_FS_DELETE;
    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_io_fs_delete.contexto);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_delete.solicitud_recurso.args_io_fs_delete.nombre_io);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_delete.solicitud_recurso.args_io_fs_delete.nombre_archivo);
    return pkg;
}
s_cpu_kernel deserializar_cpu_kernel_io_fs_delete(t_paquete *pkg)
{
    s_cpu_kernel s_cpu_kernel_io_fs_delete;
    s_cpu_kernel_io_fs_delete.contexto = deserializar_contexto_ejecucion(pkg);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_delete.solicitud_recurso.args_io_fs_delete.nombre_io);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_delete.solicitud_recurso.args_io_fs_delete.nombre_archivo);
    return s_cpu_kernel_io_fs_delete;
}

s_cpu_kernel create_s_cpu_kernel_io_fs_truncate(t_contexto_ejecucion ctx, char *io_name, uint32_t tamano,
                                                char *nombre_archivo)
{
    s_cpu_kernel s_cpu_kernel_io_fs_truncate;
    s_cpu_kernel_io_fs_truncate.contexto = ctx;
    s_cpu_kernel_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.nombre_io = io_name;
    s_cpu_kernel_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.nombre_archivo = nombre_archivo;
    s_cpu_kernel_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.tamano = tamano;
    return s_cpu_kernel_io_fs_truncate;
}
t_paquete *serializar_cpu_kernel_io_fs_truncate(s_cpu_kernel s_cpu_kernel_io_fs_truncate)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_IO_FS_TRUNCATE;
    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_io_fs_truncate.contexto);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.nombre_io);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.nombre_archivo);
    serializar_uint32_t(&pkg->buffer, s_cpu_kernel_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.tamano);
    return pkg;
}
s_cpu_kernel deserialziar_cpu_kernel_io_fs_truncate(t_paquete *pkg)
{
    s_cpu_kernel s_cpu_kernel_io_fs_truncate;
    s_cpu_kernel_io_fs_truncate.contexto = deserializar_contexto_ejecucion(pkg);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.nombre_io);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.nombre_archivo);
    deserializar_uint32_t(pkg->buffer, &s_cpu_kernel_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.tamano);
    return s_cpu_kernel_io_fs_truncate;
}

// El lintern me obligo a esto :(
s_cpu_kernel create_s_cpu_kernel_io_fs_write(char *io_name, uint32_t tamano, t_contexto_ejecucion ctx,
                                             t_list *info_marcos, char *nombre_archivo, uint32_t puntero_archivo)
{
    s_cpu_kernel s_cpu_kernel_io_fs_write;
    s_cpu_kernel_io_fs_write.contexto = ctx;
    s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.nombre_io = io_name;
    s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.nombre_archivo = nombre_archivo;
    s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.info_marcos = info_marcos;
    s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.tamano = tamano;
    s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.puntero_archivo = puntero_archivo;
    return s_cpu_kernel_io_fs_write;
}

t_paquete *serializar_cpu_kernel_io_fs_write(s_cpu_kernel s_cpu_kernel_io_fs_write)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_IO_FS_WRITE;
    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_io_fs_write.contexto);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.nombre_io);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.nombre_archivo);
    serializar_t_list(&pkg->buffer, s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.info_marcos,
                      serializar_frames_element_fs);
    serializar_uint32_t(&pkg->buffer, s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.tamano);
    serializar_uint32_t(&pkg->buffer, s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.puntero_archivo);
    return pkg;
}

s_cpu_kernel deserializar_cpu_kernel_io_fs_write(t_paquete *pkg)
{
    s_cpu_kernel s_cpu_kernel_io_fs_write;
    s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.info_marcos = list_create();

    s_cpu_kernel_io_fs_write.contexto = deserializar_contexto_ejecucion(pkg);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.nombre_io);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.nombre_archivo);
    deserializar_t_list(pkg->buffer, &s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.info_marcos,
                        deserializar_frames_element_fs);
    deserializar_uint32_t(pkg->buffer, &s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.tamano);
    deserializar_uint32_t(pkg->buffer, &s_cpu_kernel_io_fs_write.solicitud_recurso.args_io_fs_write.puntero_archivo);
    return s_cpu_kernel_io_fs_write;
}

s_cpu_kernel create_s_cpu_kernel_io_fs_read(char *io_name, uint32_t tamano, t_contexto_ejecucion ctx,
                                            t_list *info_marcos, char *nombre_archivo, uint32_t puntero_archivo)
{
    s_cpu_kernel s_cpu_kernel_io_fs_read;
    s_cpu_kernel_io_fs_read.contexto = ctx;
    s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.nombre_io = io_name;
    s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.nombre_archivo = nombre_archivo;
    s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.info_marcos = info_marcos;
    s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.tamano = tamano;
    s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.puntero_archivo = puntero_archivo;
    return s_cpu_kernel_io_fs_read;
}

t_paquete *serializar_cpu_kernel_io_fs_read(s_cpu_kernel s_cpu_kernel_io_fs_read)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_IO_FS_READ;
    serializar_contexto_ejecucion(&pkg->buffer, s_cpu_kernel_io_fs_read.contexto);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.nombre_io);
    serializar_string(&pkg->buffer, s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.nombre_archivo);
    serializar_t_list(&(pkg->buffer), s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.info_marcos,
                      serializar_frames_element_fs);
    serializar_uint32_t(&pkg->buffer, s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.tamano);
    serializar_uint32_t(&pkg->buffer, s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.puntero_archivo);
    return pkg;
}

s_cpu_kernel deserializar_cpu_kernel_io_fs_read(t_paquete *pkg)
{
    s_cpu_kernel s_cpu_kernel_io_fs_read;
    s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.info_marcos = list_create();

    s_cpu_kernel_io_fs_read.contexto = deserializar_contexto_ejecucion(pkg);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.nombre_io);
    deserializar_string(pkg->buffer, &s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.nombre_archivo);
    deserializar_t_list(pkg->buffer, &s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.info_marcos,
                        deserializar_frames_element_fs);
    deserializar_uint32_t(pkg->buffer, &s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.tamano);
    deserializar_uint32_t(pkg->buffer, &s_cpu_kernel_io_fs_read.solicitud_recurso.args_io_fs_read.puntero_archivo);
    return s_cpu_kernel_io_fs_read;
}