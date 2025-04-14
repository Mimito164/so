#include "mensajes.h"
#include <serializacion/mensajes/cpu_kernel/msj_cpu_kernel.h>

void cargar_contexto_ejecucion(t_contexto_ejecucion temp_contexto)
{
    set_pid(temp_contexto.pid);
    set_pc(temp_contexto.registros_cpu.PC);
    set_ax(temp_contexto.registros_cpu.AX);
    set_bx(temp_contexto.registros_cpu.BX);
    set_cx(temp_contexto.registros_cpu.CX);
    set_dx(temp_contexto.registros_cpu.DX);
    set_eax(temp_contexto.registros_cpu.EAX);
    set_ebx(temp_contexto.registros_cpu.EBX);
    set_ecx(temp_contexto.registros_cpu.ECX);
    set_edx(temp_contexto.registros_cpu.EDX);
    set_si(temp_contexto.registros_cpu.SI);
    set_di(temp_contexto.registros_cpu.DI);
}

void cargar_interrupcion(t_interrupt temp_interrupt)
{
    set_interrupt(temp_interrupt.pid, temp_interrupt.type);
}
// no se deserializa nada, solo llega el opcode y se mata al proceso.

void send_kill_proceso(int fd_kernel)
{
    t_log *logger = get_logger();
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_KILL;

    bool send_status = send_paquete(fd_kernel, pkg);

    if (!send_status)
    {
        log_error(logger, "send_kill_proceso ha fallado");
    }
    else
    {

        log_debug(logger, "send_kill_proceso enviado");
    }
}

void send_desalojo_finq(int fd_kernel, t_contexto_ejecucion ctx)
{
    t_paquete *pkg = paquete_new();
    pkg->codigo_operacion = CPU_KERNEL_DESALOJO_FINQ;
    serializar_contexto_ejecucion(&pkg->buffer, ctx);
    send_paquete(fd_kernel, pkg);
}

void send_desalojo_signal(int fd_kernel, t_contexto_ejecucion ctx, char *recurso)
{
    // t_paquete *pkg = serializar_desalojo_signal(ctx, recurso);
    s_cpu_kernel s_cpu_kernel_signal = create_s_cpu_kernel_signal(ctx, recurso);
    t_paquete *pkg = serializar_cpu_kernel_signal(s_cpu_kernel_signal);
    send_paquete(fd_kernel, pkg);
}

void send_desalojo_wait(int fd_kernel, t_contexto_ejecucion ctx, char *recurso)
{
    s_cpu_kernel s_cpu_kernel_wait = create_s_cpu_kernel_wait(ctx, recurso);
    t_paquete *pkg = serializar_cpu_kernel_wait(s_cpu_kernel_wait);
    send_paquete(fd_kernel, pkg);
}

void send_desalojo_io_gen_zzz(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, uint32_t unidades_trabajo)
{
    s_cpu_kernel s_cpu_kernel_io_sleep = create_s_cpu_kernel_io_sleep(ctx, io_name, unidades_trabajo);
    t_paquete *pkg = serializar_cpu_kernel_io_sleep(s_cpu_kernel_io_sleep);
    send_paquete(fd_kernel, pkg);
}

// void send_desalojo_io_stdin_read(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, uint32_t
// direccion_traducida,
//                                  uint32_t tamano)
//{
//     s_cpu_kernel s_cpu_kernel_io_stdin = create_s_cpu_kernel_io_stdin(ctx, direccion_traducida, io_name, tamano);
//     t_paquete *pkg = serializar_cpu_kernel_io_stdin(s_cpu_kernel_io_stdin);
//     send_paquete(fd_kernel, pkg);
// }

// void send_desalojo_io_stdout_write(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, uint32_t
// direccion_traducida,
//                                    uint32_t tamano)
//{
//     s_cpu_kernel s_cpu_kernel_io_stdout = create_s_cpu_kernel_io_stdout(ctx, direccion_traducida, io_name, tamano);
//     t_paquete *pkg = serializar_cpu_kernel_io_stdout(s_cpu_kernel_io_stdout);
//     send_paquete(fd_kernel, pkg);
// }

void send_desalojo_io_fs_create(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo)
{
    s_cpu_kernel s_cpu_kernel_io_fs_create = create_s_cpu_kernel_io_fs_create(io_name, ctx, nombre_archivo);
    t_paquete *pkg = serializar_cpu_kernel_io_fs_create(s_cpu_kernel_io_fs_create);
    send_paquete(fd_kernel, pkg);
}

void send_desalojo_io_fs_delete(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo)
{
    s_cpu_kernel s_cpu_kernel_io_fs_delete = create_s_cpu_kernel_io_fs_delete(io_name, ctx, nombre_archivo);
    t_paquete *pkg = serializar_cpu_kernel_io_fs_delete(s_cpu_kernel_io_fs_delete);
    send_paquete(fd_kernel, pkg);
}

void send_desalojo_io_fs_truncate(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo,
                                  uint32_t tamano)
{
    s_cpu_kernel s_cpu_kernel_io_fs_truncate = create_s_cpu_kernel_io_fs_truncate(ctx, io_name, tamano, nombre_archivo);
    t_paquete *pkg = serializar_cpu_kernel_io_fs_truncate(s_cpu_kernel_io_fs_truncate);
    send_paquete(fd_kernel, pkg);
}

void send_desalojo_io_fs_write(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo,
                               t_list *direccion_traducida, uint32_t tamano, uint32_t puntero_archivo)
{
    s_cpu_kernel s_cpu_kernel_io_fs_write =
        create_s_cpu_kernel_io_fs_write(io_name, tamano, ctx, direccion_traducida, nombre_archivo, puntero_archivo);
    t_paquete *pkg = serializar_cpu_kernel_io_fs_write(s_cpu_kernel_io_fs_write);
    send_paquete(fd_kernel, pkg);
}

void send_desalojo_io_fs_read(int fd_kernel, t_contexto_ejecucion ctx, char *io_name, char *nombre_archivo,
                              t_list *direccion_traducida, uint32_t tamano, uint32_t puntero_archivo)
{
    s_cpu_kernel s_cpu_kernel_io_fs_read =
        create_s_cpu_kernel_io_fs_read(io_name, tamano, ctx, direccion_traducida, nombre_archivo, puntero_archivo);
    t_paquete *pkg = serializar_cpu_kernel_io_fs_read(s_cpu_kernel_io_fs_read);
    send_paquete(fd_kernel, pkg);
}

t_paquete *crear_paquete_resize()
{
    t_paquete *paquete = paquete_new();
    paquete->codigo_operacion = CPU_KERNEL_DESALOJO_OUT_OF_MEMORY;
    serializar_contexto_ejecucion(&paquete->buffer, get_all());
    return paquete;
}