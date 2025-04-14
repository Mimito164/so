#include "comunicacion_cpu.h"

static int fd_server_dispatch_cpu;
static pthread_mutex_t mutex_fd_server_dispatch_cpu;

static int fd_server_interrupt_cpu;
static pthread_mutex_t mutex_fd_server_interrupt_cpu;

static void delegante(t_paquete *paquete_respuesta_dispatch);
static void *handle_dispatch_recieve();

void iniciar_comunicacion_cpu()
{
    t_log *logger = get_logger();
    t_config_kernel *cfg = get_config();
    if (!generar_conexion(logger, "servidor cpu dispatch", &fd_server_dispatch_cpu, cfg->IP_CPU,
                          cfg->PUERTO_CPU_DISPATCH))
    {
        assert(!"no logramos conectar con el modulo cpu puerto dispatch");
        // TODO(ALL) crear funcion para liberar datos de todo.
    }

    pthread_mutex_init(&mutex_fd_server_dispatch_cpu, NULL);

    if (!generar_conexion(logger, "servidor cpu interrupt", &fd_server_interrupt_cpu, cfg->IP_CPU,
                          cfg->PUERTO_CPU_INTERRUPT))
    {

        assert(!"no logramos conectar con el modulo cpu puerto interrupt");
        // TODO(ALL) crear funcion para liberar datos de todo.
    }

    pthread_mutex_init(&mutex_fd_server_interrupt_cpu, NULL);
}

/// @brief Levanta un hilo y NO es bloqueante.
/// @brief Envía el contexto de ejecución por dispatch y queda a la espera de la respuesta por dispatch de CPU.
/// @brief La espera de la respuesta es bloqueante y justamente bloquea el uso del socket de dispatch.
/// @brief Cuando recibe la respuesta no retorna el valor. Delega tareas en base a lo que nos envíe CPU.
/// @param contexto es el Contexto de ejecución del proceso que será enviado a CPU
void send_dispatch(t_contexto_ejecucion contexto) // NOLINT(bugprone-easily-swappable-parameters)
{

    t_log *logger = get_logger();
    t_paquete *paquete_dispatch_send = serializar_kernel_cpu_execute(contexto);

    pthread_mutex_lock(&mutex_fd_server_dispatch_cpu);

    bool send_status = send_paquete(fd_server_dispatch_cpu, paquete_dispatch_send);
    if (!send_status)
    {
        log_error(logger, "Error en send_dispatch()");
        assert(!"Error en send_dispatch, falló el send"); // esto es solo para debuggear
        pthread_mutex_unlock(&mutex_fd_server_dispatch_cpu);
    }

    log_trace(logger, "KERNEL_CPU_EXECUTE enviado a cpu");
    // TODO(all): send_dispatch podría ser bloqueante, no hace falta crear un hilo
    // hacemos send y nos quedamos bloqueados esperando la respuesta ... (para revisar en el futuro)
    pthread_t recieve_thread;
    pthread_create(&recieve_thread, NULL, handle_dispatch_recieve, NULL /* argumentos del handle */);
    pthread_detach(recieve_thread);
}

/// @brief Envía un interrupt de Kill o de Desalojo al CPU
/// @param pid es el Process ID
/// @param codigo KERNEL_CPU_INT_DESALOJO o KERNEL_CPU_INT_KILL
void send_interrupt(uint32_t pid, op_code codigo) // NOLINT(bugprone-easily-swappable-parameters)
{
    assert((codigo == KERNEL_CPU_INT_DESALOJO) | (codigo == KERNEL_CPU_INT_KILL) |
           (codigo == KERNEL_CPU_INT_KILL_ALREADY_KILLED));

    t_log *logger = get_logger();

    t_paquete *paquete_interrupt = serializar_kernel_cpu_interrupt(pid, codigo);

    pthread_mutex_lock(&mutex_fd_server_interrupt_cpu);
    bool send_status = send_paquete(fd_server_interrupt_cpu, paquete_interrupt);

    if (!send_status)
    {
        log_error(logger, "Error en send_interrupt()");
        assert(!"Error en send_interrupt, falló el send");
        pthread_mutex_unlock(&mutex_fd_server_interrupt_cpu);
    }

    if (codigo == KERNEL_CPU_INT_DESALOJO)
    {
        log_trace(logger, "KERNEL_CPU_INT_DESALOJO enviado a cpu");
    }

    if (codigo == KERNEL_CPU_INT_KILL)
    {
        log_trace(logger, "KERNEL_CPU_INT_KILL enviado a cpu");
    }

    if (codigo == KERNEL_CPU_INT_KILL_ALREADY_KILLED)
    {
        log_trace(logger, "KERNEL_CPU_INT_KILL_ALREADY_KILLED enviado a cpu");
    }

    pthread_mutex_unlock(&mutex_fd_server_interrupt_cpu);
}

/// @brief Es la función para el hilo que espera la respuesta por dispatch.
/// @return no retorna nada
static void *handle_dispatch_recieve() // puede recibir args
{
    t_log *logger = get_logger();

    t_paquete *paquete_respuesta_dispatch = paquete_new();

    bool recv_status = recv_paquete(fd_server_dispatch_cpu, &paquete_respuesta_dispatch);
    pthread_mutex_unlock(&mutex_fd_server_dispatch_cpu);

    if (!recv_status)
    {
        log_error(logger, "Error en send_dispatch()");
        assert(!"Error en send_dispatch, falló el send");
        pthread_exit(NULL);
    }

    delegante(paquete_respuesta_dispatch);

    return 0;
}

/// @brief Recibe la respuesta de CPU y delega el manejo de la misma
/// @param paquete_respuesta_dispatch
static void delegante(t_paquete *paquete_respuesta_dispatch)
{
    t_log *logger = get_logger();
    sem_wait(&semaforo_estado_exec);

    switch (paquete_respuesta_dispatch->codigo_operacion)
    {
    case CPU_KERNEL_KILL: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_KILL recibido");
        t_pcb *executing = get_executing();
        blackilst_process(executing->pid, (motivo_de_muerte)SUCCESS);
        check_blacklist(EXEC_A_EXIT, executing->pid, NULL);

        disable_last_quantum_args();

        s_planificador_corto_args args = {.transicion = READY_A_EXEC};
        planificador_corto(args);

        break;
    }

    case CPU_KERNEL_KILL_ALREADY_KILLED: {

        disable_last_quantum_args();
        s_planificador_corto_args args = {.transicion = READY_A_EXEC};
        planificador_corto(args);

        break;
    }
    case CPU_KERNEL_DESALOJO_FINQ: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_FINQ recibido");
        t_pcb *executing = get_executing();
        log_fin_de_q((int)executing->pid);
        t_contexto_ejecucion contexto = deserializar_contexto_ejecucion(paquete_respuesta_dispatch);
        update_pcb(get_executing(), contexto);
        s_planificador_corto_args args = {.transicion = EXEC_A_READY};
        planificador_corto(args);

        break;
    }

    case CPU_KERNEL_DESALOJO_SIGNAL: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_SIGNAL recibido");
        s_cpu_kernel desalojo = deserializar_cpu_kernel_signal(paquete_respuesta_dispatch);
        update_pcb(get_executing(), desalojo.contexto);

        bool result =
            desencolar_solicitud_sistema(desalojo.solicitud_recurso.args_signal, desalojo.contexto.pid, false);

        // el recurso no existe y matamos al proceso (no esta mas en EXEC), llamamos al plani
        if (!result)
        {
            s_planificador_corto_args args = {.transicion = READY_A_EXEC};
            planificador_corto(args);
        }
        else
        {
            // El PCB está en exec, pero cpu lo desalojó
            if (!hay_fin_q)
            {
                send_dispatch(desalojo.contexto);
            }
            else
            {
                disable_last_quantum_args();
                s_planificador_corto_args args = {.transicion = EXEC_A_READY};
                planificador_corto(args);
            }
        }
        break;
    }

    case CPU_KERNEL_DESALOJO_WAIT: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_WAIT recibido");

        s_cpu_kernel desalojo = deserializar_cpu_kernel_wait(paquete_respuesta_dispatch);
        update_pcb(get_executing(), desalojo.contexto);

        resultado_encolar result = encolar_solicitud(desalojo.solicitud_recurso.args_wait,
                                                     (t_instruccion)CPU_KERNEL_DESALOJO_WAIT, (s_kernel_data)desalojo);

        if (result == PROCESANDO_AHORA)
        {
            if (!hay_fin_q)
            {
                // El PCB está en exec, pero cpu lo desalojó
                send_dispatch(desalojo.contexto); // esto se lo manda a cpu para que siga
            }
            else
            {
                disable_last_quantum_args();
                s_planificador_corto_args args = {.transicion = EXEC_A_READY};
                planificador_corto(args);
            }
        }
        else
        {
            disable_last_quantum_args();
            s_planificador_corto_args args = {.transicion = EXEC_A_BLOCKED, .op_pid.PID = (int)desalojo.contexto.pid};
            planificador_corto(args);
        }
        break;
    }
    case CPU_KERNEL_DESALOJO_IO_GEN_ZZZ: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_IO_GEN_ZZZ recibido");

        s_cpu_kernel desalojo_io_gen = deserializar_cpu_kernel_io_sleep(paquete_respuesta_dispatch);
        update_pcb(get_executing(), desalojo_io_gen.contexto);

        disable_last_quantum_args();

        s_kernel_io_sleep msj_kernel_io = {.pid = desalojo_io_gen.contexto.pid,
                                           .work_unit =
                                               desalojo_io_gen.solicitud_recurso.args_io_sleep.unidades_trabajo};

        s_planificador_corto_args args = {.transicion = EXEC_A_BLOCKED,
                                          .op_pid.PID = (int)desalojo_io_gen.contexto.pid};
        planificador_corto(args);

        resultado_encolar result = encolar_solicitud(desalojo_io_gen.solicitud_recurso.args_io_sleep.nombre_io,
                                                     (t_instruccion)KERNEL_IO_GEN_ZZZ, (s_kernel_data)msj_kernel_io);

        if (result == KILL_AL_PROCESO)
        {
            blackilst_process(desalojo_io_gen.contexto.pid, (motivo_de_muerte)INVALID_RESOURCE);
            check_blacklist(BLOCKED_A_EXIT, desalojo_io_gen.contexto.pid, NULL);
            find_and_kill(desalojo_io_gen.contexto.pid); // porlas

            log_trace(logger, "KILL al PID <%i> por encolar un recurso que no existe", desalojo_io_gen.contexto.pid);
        }

        break;
    }
    case CPU_KERNEL_DESALOJO_IO_STDIN_READ: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_IO_STDIN_READ recibido");

        s_cpu_kernel desalojo_io_stdin = deserializar_cpu_kernel_io_stdin(paquete_respuesta_dispatch);
        update_pcb(get_executing(), desalojo_io_stdin.contexto);

        disable_last_quantum_args();
        s_kernel_io_stdin_read msj_kernel_io = {.pid = desalojo_io_stdin.contexto.pid,
                                                .size = desalojo_io_stdin.solicitud_recurso.args_io_stdin.size,

                                                .lista_frames =
                                                    desalojo_io_stdin.solicitud_recurso.args_io_stdin.lista_frames};

        s_planificador_corto_args args = {.transicion = EXEC_A_BLOCKED,
                                          .op_pid.PID = (int)desalojo_io_stdin.contexto.pid};
        planificador_corto(args);

        resultado_encolar result = encolar_solicitud(desalojo_io_stdin.solicitud_recurso.args_io_stdin.nombre_io,
                                                     (t_instruccion)KERNEL_IO_STDIN_READ, (s_kernel_data)msj_kernel_io);

        if (result == KILL_AL_PROCESO)
        {
            blackilst_process(desalojo_io_stdin.contexto.pid, (motivo_de_muerte)INVALID_RESOURCE);
            check_blacklist(BLOCKED_A_EXIT, desalojo_io_stdin.contexto.pid, NULL);
            find_and_kill(desalojo_io_stdin.contexto.pid); // porlas

            log_trace(logger, "KILL al PID <%i> por encolar un recurso que no existe", desalojo_io_stdin.contexto.pid);
        }
        break;
    }
    case CPU_KERNEL_DESALOJO_IO_STDOUT_WRITE: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_IO_STDOUT_WRITE recibido");
        s_cpu_kernel desalojo_io_stdout = deserializar_cpu_kernel_io_stdout(paquete_respuesta_dispatch);
        update_pcb(get_executing(), desalojo_io_stdout.contexto);

        disable_last_quantum_args();

        s_kernel_io_stdout_write msj_kernel_io = {.pid = desalojo_io_stdout.contexto.pid,
                                                  .lista_frames =
                                                      desalojo_io_stdout.solicitud_recurso.args_io_stdin.lista_frames};

        s_planificador_corto_args args = {.transicion = EXEC_A_BLOCKED,
                                          .op_pid.PID = (int)desalojo_io_stdout.contexto.pid};
        planificador_corto(args);

        resultado_encolar result =
            encolar_solicitud(desalojo_io_stdout.solicitud_recurso.args_io_stdin.nombre_io,
                              (t_instruccion)KERNEL_IO_STDOUT_WRITE, (s_kernel_data)msj_kernel_io);

        if (result == KILL_AL_PROCESO)
        {
            blackilst_process(desalojo_io_stdout.contexto.pid, (motivo_de_muerte)INVALID_RESOURCE);
            check_blacklist(BLOCKED_A_EXIT, desalojo_io_stdout.contexto.pid, NULL);
            find_and_kill(desalojo_io_stdout.contexto.pid); // porlas

            log_trace(logger, "KILL al PID <%i> por encolar un recurso que no existe", desalojo_io_stdout.contexto.pid);
        }
        break;
    }
    case CPU_KERNEL_DESALOJO_IO_FS_CREATE: {

        log_trace(logger, "CPU_KERNEL_DESALOJO_IO_FS_CREATE recibido");
        s_cpu_kernel s_io_fs_create = deserializar_cpu_kernel_io_fs_create(paquete_respuesta_dispatch);
        update_pcb(get_executing(), s_io_fs_create.contexto);
        disable_last_quantum_args();

        s_kernel_io_fs_create msj_kernel_io = create_kernel_io_fs_create(
            s_io_fs_create.contexto.pid, s_io_fs_create.solicitud_recurso.args_io_fs_create.nombre_archivo);

        s_planificador_corto_args args = {.transicion = EXEC_A_BLOCKED};
        planificador_corto(args);

        resultado_encolar result = encolar_solicitud(s_io_fs_create.solicitud_recurso.args_io_fs_create.nombre_io,
                                                     (t_instruccion)KERNEL_IO_FS_CREATE, (s_kernel_data)msj_kernel_io);

        if (result == KILL_AL_PROCESO)
        {
            blackilst_process(s_io_fs_create.contexto.pid, (motivo_de_muerte)INVALID_RESOURCE);
            check_blacklist(BLOCKED_A_EXIT, s_io_fs_create.contexto.pid, NULL);
            find_and_kill(s_io_fs_create.contexto.pid); // porlas

            log_trace(logger, "KILL al PID %i por encolar un recurso que no existe", s_io_fs_create.contexto.pid);
        }
        break;
    }

    case CPU_KERNEL_DESALOJO_IO_FS_DELETE: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_IO_FS_DELETE recibido");
        s_cpu_kernel s_io_fs_delete = deserializar_cpu_kernel_io_fs_delete(paquete_respuesta_dispatch);
        update_pcb(get_executing(), s_io_fs_delete.contexto);
        disable_last_quantum_args();

        s_kernel_io_fs_delete msj_kernel_io = create_kernel_io_fs_delete(
            s_io_fs_delete.contexto.pid, s_io_fs_delete.solicitud_recurso.args_io_fs_create.nombre_archivo);

        s_planificador_corto_args args = {.transicion = EXEC_A_BLOCKED};
        planificador_corto(args);

        resultado_encolar result = encolar_solicitud(s_io_fs_delete.solicitud_recurso.args_io_fs_create.nombre_io,
                                                     (t_instruccion)KERNEL_IO_FS_DELETE, (s_kernel_data)msj_kernel_io);

        if (result == KILL_AL_PROCESO)
        {
            blackilst_process(s_io_fs_delete.contexto.pid, (motivo_de_muerte)INVALID_RESOURCE);
            check_blacklist(BLOCKED_A_EXIT, s_io_fs_delete.contexto.pid, NULL);
            find_and_kill(s_io_fs_delete.contexto.pid); // porlas

            log_trace(logger, "KILL al PID %i por encolar un recurso que no existe", s_io_fs_delete.contexto.pid);
        }
        break;
    }

    case CPU_KERNEL_DESALOJO_IO_FS_TRUNCATE: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_IO_FS_TRUNCATE recibido");
        s_cpu_kernel s_io_fs_truncate = deserialziar_cpu_kernel_io_fs_truncate(paquete_respuesta_dispatch);
        update_pcb(get_executing(), s_io_fs_truncate.contexto);
        disable_last_quantum_args();

        s_kernel_io_fs_truncate msj_kernel_io = create_kernel_io_fs_truncate(
            s_io_fs_truncate.contexto.pid, s_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.nombre_archivo,
            s_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.tamano);

        s_planificador_corto_args args = {.transicion = EXEC_A_BLOCKED};
        planificador_corto(args);

        resultado_encolar result =
            encolar_solicitud(s_io_fs_truncate.solicitud_recurso.args_io_fs_truncate.nombre_io,
                              (t_instruccion)KERNEL_IO_FS_TRUNCATE, (s_kernel_data)msj_kernel_io);

        if (result == KILL_AL_PROCESO)
        {
            blackilst_process(s_io_fs_truncate.contexto.pid, (motivo_de_muerte)INVALID_RESOURCE);
            check_blacklist(BLOCKED_A_EXIT, s_io_fs_truncate.contexto.pid, NULL);
            find_and_kill(s_io_fs_truncate.contexto.pid); // porlas

            log_trace(logger, "KILL al PID %i por encolar un recurso que no existe", s_io_fs_truncate.contexto.pid);
        }
        break;
    }

    case CPU_KERNEL_DESALOJO_IO_FS_WRITE: {
        log_trace(logger, "CPU_KERNEL_DESALOJO_IO_FS_WRITE recibido");
        s_cpu_kernel desalojo_io_fs_write = deserializar_cpu_kernel_io_fs_write(paquete_respuesta_dispatch);
        update_pcb(get_executing(), desalojo_io_fs_write.contexto);
        disable_last_quantum_args();

        s_kernel_io_fs_write msj_kernel_io = create_kernel_io_fs_write(
            desalojo_io_fs_write.contexto.pid, desalojo_io_fs_write.solicitud_recurso.args_io_fs_read.nombre_archivo,
            desalojo_io_fs_write.solicitud_recurso.args_io_fs_write.puntero_archivo,
            desalojo_io_fs_write.solicitud_recurso.args_io_fs_write.info_marcos,
            desalojo_io_fs_write.solicitud_recurso.args_io_fs_write.tamano);
        s_planificador_corto_args args = {.transicion = EXEC_A_BLOCKED};
        planificador_corto(args);

        resultado_encolar result = encolar_solicitud(desalojo_io_fs_write.solicitud_recurso.args_io_fs_write.nombre_io,
                                                     (t_instruccion)KERNEL_IO_FS_WRITE, (s_kernel_data)msj_kernel_io);

        if (result == KILL_AL_PROCESO)
        {
            blackilst_process(desalojo_io_fs_write.contexto.pid, (motivo_de_muerte)INVALID_RESOURCE);
            check_blacklist(BLOCKED_A_EXIT, desalojo_io_fs_write.contexto.pid, NULL);
            find_and_kill(desalojo_io_fs_write.contexto.pid); // porlas

            log_trace(logger, "KILL al PID %i por encolar un recurso que no existe", desalojo_io_fs_write.contexto.pid);
        }
        break;
    }

    case CPU_KERNEL_DESALOJO_IO_FS_READ: {

        log_trace(logger, "CPU_KERNEL_DESALOJO_IO_FS_READ recibido");

        s_cpu_kernel desalojo_io_fs_read = deserializar_cpu_kernel_io_fs_read(paquete_respuesta_dispatch);
        update_pcb(get_executing(), desalojo_io_fs_read.contexto);
        disable_last_quantum_args();

        s_kernel_io_fs_read msj_kernel_io = create_kernel_io_fs_read(
            desalojo_io_fs_read.contexto.pid, desalojo_io_fs_read.solicitud_recurso.args_io_fs_read.nombre_archivo,
            desalojo_io_fs_read.solicitud_recurso.args_io_fs_read.puntero_archivo,
            desalojo_io_fs_read.solicitud_recurso.args_io_fs_read.info_marcos,
            desalojo_io_fs_read.solicitud_recurso.args_io_fs_read.tamano);

        s_planificador_corto_args args = {.transicion = EXEC_A_BLOCKED};
        planificador_corto(args);

        resultado_encolar result = encolar_solicitud(desalojo_io_fs_read.solicitud_recurso.args_io_fs_read.nombre_io,
                                                     (t_instruccion)KERNEL_IO_FS_READ, (s_kernel_data)msj_kernel_io);

        if (result == KILL_AL_PROCESO)
        {
            blackilst_process(desalojo_io_fs_read.contexto.pid, (motivo_de_muerte)INVALID_RESOURCE);
            check_blacklist(BLOCKED_A_EXIT, desalojo_io_fs_read.contexto.pid, NULL);
            find_and_kill(desalojo_io_fs_read.contexto.pid); // porlas

            log_trace(logger, "KILL al PID %i por encolar un recurso que no existe", desalojo_io_fs_read.contexto.pid);
        }

        break;
    }

    case CPU_KERNEL_DESALOJO_OUT_OF_MEMORY:
        log_trace(logger, "CPU_KERNEL_DESALOJO_OUT_OF_MEMORY recibido");
        t_pcb *executing = get_executing();
        blackilst_process(executing->pid, (motivo_de_muerte)OUT_OF_MEMORY);
        check_blacklist(EXEC_A_EXIT, executing->pid, NULL);

        disable_last_quantum_args();

        s_planificador_corto_args args = {.transicion = READY_A_EXEC};
        planificador_corto(args);

        break;

    default:
        assert(!"CPU nos envió un op_code desconocido en handle_dispatch_recieve()"); // esto es solo para debuggear
        log_error(logger, "CPU nos envió un op_code desconocido en handle_dispatch_recieve()");
        break;
    }
    hay_fin_q = false;
    sem_post(&semaforo_estado_exec);
    paquete_destroy(paquete_respuesta_dispatch);
}
