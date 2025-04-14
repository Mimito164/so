#include "gestor_recursos.h"

#define DECIMAL 10
#define KILL_PROCESS true
#define DONT_KILL_PROCESS false

static t_dictionary *recursos;
static pthread_mutex_t mutex_recursos;

// Start: ========= SOLICITUDES ============= //
static t_solicitud *create_solicitud(int pid, s_kernel_data data_instruccion, t_instruccion tipo_instruccion)
{
    t_solicitud *solicitud = malloc(sizeof(t_solicitud));
    solicitud->pid = pid;
    solicitud->procesando = false;
    solicitud->tipo_instruccion = tipo_instruccion;

    solicitud->data_instruccion = data_instruccion;

    return solicitud;
};

/// @brief destruye una solicitud
/// @param solicitud La solicitud a eliminar
/// @param kill si debe o no matar al proceso de la solicitud
static void destroy_solicitud(void *raw_solicitud, bool kill)
{
    t_solicitud *solicitud = (t_solicitud *)raw_solicitud;

#ifndef TEST
    if (kill)
    {
        // mandar a matar procesos con solicitudes encoladas
        blackilst_process(solicitud->pid, (motivo_de_muerte)INVALID_INTERFACE);
        find_and_kill(solicitud->pid); // no sabemos en que lista puede estar
    }
#endif

    // liberar memoria
    free(solicitud);
    solicitud = NULL;
    raw_solicitud = NULL;
}

/// @brief destruye una solicitud y mata al proceso de la misma
/// @param raw_solicitud La solicitud a eliminar
static void destroy_solicitud_and_kill_process(void *raw_solicitud)
{
    destroy_solicitud(raw_solicitud, KILL_PROCESS);
}

/// @brief Pone a procesar una solicitud
/// @attention *NO* se encarga de agregar la solicitud a la lista de solicitudes
/// @param solicitud La solicitud que se quiere procesar
/// @param recurso El recurso que la debe procesar
/// @return true si se pudo procesar false si no se pudo procesar
static bool procesar_solicitud(t_solicitud *solicitud, t_recurso *recurso)
{
    log_trace(get_logger(), "procesar_solicitud del PID: %i del recurso: %s", solicitud->pid, recurso->nombre);
    if (recurso->instancias_disponibles <= 0)
    {
        log_bloqueo((int)solicitud->pid, recurso->nombre);
        return false;
    }

    solicitud->procesando = true;      // cambiar el booleano de la solicitud a true
    recurso->instancias_disponibles--; // restar 1 a instancias_disponibles

    switch (solicitud->tipo_instruccion)
    {
    case INST_KERNEL_IO_GEN_ZZZ:
        log_trace(get_logger(), "procesar_solicitud INST_KERNEL_IO_GEN_ZZZ del PID: %i", solicitud->pid);
        log_bloqueo((int)solicitud->pid, recurso->nombre);

#ifndef TEST
        handle_io_gen_sleep(solicitud, recurso);
#endif
        break;

    case INST_KERNEL_IO_STDIN_READ:
        log_trace(get_logger(), "procesar_solicitud INST_KERNEL_IO_STDIN_READ del PID: %i", solicitud->pid);
        log_bloqueo((int)solicitud->pid, recurso->nombre);

#ifndef TEST
        handle_io_stdin_read(solicitud, recurso);
#endif
        break;

    case INST_KERNEL_IO_STDOUT_WRITE:
        log_trace(get_logger(), "procesar_solicitud INST_KERNEL_IO_STDOUT_WRITE del PID: %i", solicitud->pid);
        log_bloqueo((int)solicitud->pid, recurso->nombre);

#ifndef TEST
        handle_io_stdout_write(solicitud, recurso);
#endif
        break;

    case INST_KERNEL_IO_FS_CREATE:
        log_trace(get_logger(), "procesar_solicitud INST_KERNEL_IO_FS_CREATE del PID: %i", solicitud->pid);
        log_bloqueo((int)solicitud->pid, recurso->nombre);

#ifndef TEST
        handle_io_fs_create(solicitud, recurso);
#endif
        break;

    case INST_KERNEL_IO_FS_DELETE:
        log_trace(get_logger(), "procesar_solicitud INST_KERNEL_IO_FS_DELETE del PID: %i", solicitud->pid);
        log_bloqueo((int)solicitud->pid, recurso->nombre);

#ifndef TEST
        handle_io_fs_delete(solicitud, recurso);
#endif
        break;

    case INST_KERNEL_IO_FS_TRUNCATE:
        log_trace(get_logger(), "procesar_solicitud INST_KERNEL_IO_FS_TRUNCATE del PID: %i", solicitud->pid);
        log_bloqueo((int)solicitud->pid, recurso->nombre);

#ifndef TEST
        handle_io_fs_truncate(solicitud, recurso);
#endif
        break;

    case INST_KERNEL_IO_FS_WRITE:
        log_trace(get_logger(), "procesar_solicitud INST_KERNEL_IO_FS_WRITE del PID: %i", solicitud->pid);
        log_bloqueo((int)solicitud->pid, recurso->nombre);

#ifndef TEST
        handle_io_fs_write(solicitud, recurso);
#endif
        break;

    case INST_KERNEL_IO_FS_READ:
        log_trace(get_logger(), "procesar_solicitud INST_KERNEL_IO_FS_READ del PID: %i", solicitud->pid);
        log_bloqueo((int)solicitud->pid, recurso->nombre);

#ifndef TEST
        handle_io_fs_read(solicitud, recurso);
#endif
        break;

    case INST_CPU_KERNEL_DESALOJO_SIGNAL:
    case INST_CPU_KERNEL_DESALOJO_WAIT:
        // No habría que hacer nada (en teoría)
        break;

    default:
        log_error(get_logger(), "procesar_solicitud del PID: %i recibió una instrucción que no sabe manejar",
                  solicitud->pid);
        break;
    }
    return true;
}
// End: =========== SOLICITUDES ========== //

// Start: =========== RECURSOS ============== //

/// @brief Crea un recurso (y crea la lista de solicitudes del mismo)
/// @param instancias Cantidad de instancias del recurso. Si es único, hay una instancia
/// @param tipo El tipo de IO o si es un recurso de sistema
/// @return Puntero al recurso creado
static t_recurso *create_recurso(int instancias, TIPO_RECURSOS tipo, // NOLINT(bugprone-easily-swappable-parameters)
                                 int fd_io, char *nombre)            // NOLINT(bugprone-easily-swappable-parameters)
{
    t_recurso *recurso = malloc(sizeof(t_recurso));
    recurso->nombre = nombre;
    recurso->instancias = instancias;
    recurso->instancias_disponibles = instancias;
    recurso->tipo = tipo;
    recurso->fd_io = fd_io;
    recurso->solicitudes = list_create();
    pthread_mutex_init(&recurso->mutex_recurso, NULL);

    return recurso;
}

void agregar_recurso(char *nombre, int instancias, TIPO_RECURSOS tipo,
                     int fd_io) // NOLINT(bugprone-easily-swappable-parameters)
{
    t_recurso *recurso = create_recurso(instancias, tipo, fd_io, nombre);
    pthread_mutex_lock(&mutex_recursos);
    dictionary_put(recursos, nombre, recurso);
    pthread_mutex_unlock(&mutex_recursos);
}

/// @brief Destruye un recurso y todo su contenido\
/// @brief Manda kill a todos los procesos que tengan solicitudes al recurso
/// @param recurso el recurso para eliminar
static void destroy_recurso(t_recurso *recurso)
{
    pthread_mutex_destroy(&(recurso->mutex_recurso));
    list_destroy_and_destroy_elements(recurso->solicitudes, destroy_solicitud_and_kill_process);
    free(recurso);
    recurso = NULL;
}

void eliminar_recurso(char *nombre)
{
    pthread_mutex_lock(&mutex_recursos);
    dictionary_remove_and_destroy(recursos, nombre, (void *)&destroy_recurso);
    pthread_mutex_unlock(&mutex_recursos);
}

/// @brief Busca un recurso por su nombre.
/// @param nombre el nombre del recurso
/// @return `t_recurso* | NULL` Retorna el recurso si existe o NULL si no existe
static t_recurso *get_recurso(char *nombre)
{
    t_recurso *recurso = NULL;
    pthread_mutex_lock(&mutex_recursos);
    bool recurso_existe = dictionary_has_key(recursos, nombre);
    if (recurso_existe)
    {
        recurso = (t_recurso *)dictionary_get(recursos, nombre);
    }
    pthread_mutex_unlock(&mutex_recursos);

    return recurso;
}

void init_gestor_recursos()
{
    recursos = dictionary_create();
    pthread_mutex_init(&mutex_recursos, NULL);

    t_config_kernel *config = get_config();
    char **recursos_nombres = string_get_string_as_array(config->RECURSOS);
    char **recursos_instancias = string_get_string_as_array(config->INSTANCIAS_RECURSOS);

    int count = 0;
    while (recursos_nombres[count])
    {
        char *nombre = (char *)malloc(strlen(recursos_nombres[count]) + 1);
        strcpy(nombre, recursos_nombres[count]);
        int instancias = (int)strtol(recursos_instancias[count], NULL, DECIMAL);
        agregar_recurso(nombre, instancias, SISTEMA_SIGNAL_WAIT, -1);
        count++;
    }

    string_array_destroy(recursos_nombres);
    string_array_destroy(recursos_instancias);
}

void destroy_gestor_recursos()
{
    pthread_mutex_lock(&mutex_recursos);
    dictionary_destroy_and_destroy_elements(recursos, (void *)&destroy_recurso);
    pthread_mutex_unlock(&mutex_recursos);
    pthread_mutex_destroy(&mutex_recursos);
}
// End: ============= Recursos ============== //

int get_pid_from_instruccion(t_instruccion tipo_instruccion, s_kernel_data data_instruccion)
{
    uint32_t pid;

    switch (tipo_instruccion)
    {
    case INST_CPU_KERNEL_DESALOJO_SIGNAL:
    case INST_CPU_KERNEL_DESALOJO_WAIT:
        pid = data_instruccion.wati_o_signal.contexto.pid;
        break;

    case INST_KERNEL_IO_GEN_ZZZ:
        pid = data_instruccion.sleep.pid;
        break;
    case INST_KERNEL_IO_STDIN_READ:
        pid = data_instruccion.stdin_read.pid;
        break;
    case INST_KERNEL_IO_STDOUT_WRITE:
        pid = data_instruccion.stdout_write.pid;
        break;
    case INST_KERNEL_IO_FS_CREATE:
        pid = data_instruccion.fs_create.pid;
        break;
    case INST_KERNEL_IO_FS_DELETE:
        pid = data_instruccion.fs_delete.pid;
        break;
    case INST_KERNEL_IO_FS_TRUNCATE:
        pid = data_instruccion.fs_truncate.pid;
        break;
    case INST_KERNEL_IO_FS_WRITE:
        pid = data_instruccion.fs_write.pid;
        break;
    case INST_KERNEL_IO_FS_READ:
        pid = data_instruccion.fs_read.pid;
        break;
    default:
        assert(!"Instruccion no valida");
        break;
    }

    return (int)pid;
}

resultado_encolar encolar_solicitud(char *nombre_recurso, t_instruccion tipo_instruccion,
                                    s_kernel_data data_instruccion)
{
    int pid = get_pid_from_instruccion(tipo_instruccion, data_instruccion);

    // 1. Buscar el recurso en el diccionario: get_recurso(nombre_recurso);
    t_recurso *recurso = get_recurso(nombre_recurso);
    t_log *logger = get_logger();
    log_trace(logger, "encolar_solicitud del PID: %i", pid);

    // 2. Si no lo encuentra KILL al proceso (KILL se encarga de poner a ejecutar otro proceso, nosotros no)
    if (recurso == NULL)
    {
        return KILL_AL_PROCESO;
    }
    // 3. Crear un t_solicitud create_solicitud()
    t_solicitud *solicitud = create_solicitud(pid, data_instruccion, tipo_instruccion);

    pthread_mutex_lock(&recurso->mutex_recurso);
    // 4. Encolar la t_solicitud en la lista: t_list *solicitudes;
    list_add(recurso->solicitudes, solicitud);

    // 5. procesar_solicitud(): retorna true si la procesa o false si no.
    if (!procesar_solicitud(solicitud, recurso))
    {
        pthread_mutex_unlock(&recurso->mutex_recurso);
        return ESPERANDO_SER_PROCESADO;
    }
    pthread_mutex_unlock(&recurso->mutex_recurso);
    return PROCESANDO_AHORA;
}

static bool procesado(t_solicitud *solicitud)
{
    return solicitud->procesando;
}

static bool sin_procesar(t_solicitud *solicitud)
{
    return !solicitud->procesando;
}

bool desencolar_solicitud_io(char *key_recurso, uint32_t pid) // NOLINT(misc-unused-parameters)
{
    t_log *logger = get_logger();
    log_trace(logger, "desencolar_solicitud_io del PID: %i", pid);

    // 1. Buscar el recurso en el diccionario: get_recurso(nombre_recurso);
    t_recurso *recurso = get_recurso(key_recurso);

    // 2. Si no lo encuentra KILL al proceso
    if (recurso == NULL)
    {
#ifndef TEST
        blackilst_process(pid, (motivo_de_muerte)INVALID_RESOURCE);
        find_and_kill(pid);
#endif
        log_trace(logger, "KILL al PID <%i> por desencolar a un recurso que no existe", pid);
        return false;
    }

    // 3. Buscar en la lista de solicitudes UNA solicitud con el PID que nos pasaron y quitarla de la lista

    // en vez de desbloquear mi solicitud deberia desbloquear la primer solicitud que encuentre.
    bool coincide_pid(t_solicitud * solicitud)
    {
        return solicitud->pid == pid;
    }

    pthread_mutex_lock(&recurso->mutex_recurso);
    t_solicitud *solicitud = list_remove_by_condition(recurso->solicitudes, (void *)coincide_pid);
    if (solicitud && solicitud->procesando)
    {
        // 3.1 Sumar 1 a instancias_disponibles
        recurso->instancias_disponibles++;

        // 3.2: eliminar lo que haga falta para no tener memory leaks
        destroy_solicitud(solicitud, DONT_KILL_PROCESS);
    }

    // 4. Si hay otras solicitudes sin procesar para el recurso procesarlas procesar_solicitud(t_solicitud solicitud)

    if (recurso->instancias_disponibles > 0 && list_any_satisfy(recurso->solicitudes, (void *)sin_procesar))
    {
        // log_trace(logger, "la concha de mi madre, el recurso es: %s, %d", recurso->nombre,
        //           recurso->instancias_disponibles);
        solicitud = list_find(recurso->solicitudes, (void *)sin_procesar);
        int next_pid = get_pid_from_instruccion(solicitud->tipo_instruccion, solicitud->data_instruccion);

        // 4. No importa si procesar_solicitud tira true or false, pero siempre debería tirar true.
        if (!procesar_solicitud(solicitud, recurso))
        {
            log_error(logger, "no se pudo procesar la solicitud siguiente, esto no debería pasar");
            assert(false); // 5. matamos la ejecucion si no encontramos la solicitud
        }

        // Si desencolamos un signal, la próxima solicitud es de un proceso bloqueado por wait
        if (recurso->tipo == SISTEMA_SIGNAL_WAIT)
        {
#ifndef TEST
            s_planificador_corto_args args = {.transicion = BLOCKED_A_READY, .op_pid.PID = next_pid};
            planificador_corto(args);
#endif
        }
    }
    pthread_mutex_unlock(&recurso->mutex_recurso);

    // 5. retornar true si salió todo bien
    return true;
}

bool desencolar_solicitud_sistema(char *key_recurso, uint32_t pid, bool es_exterminio) // NOLINT(misc-unused-parameters)
{
    t_log *logger = get_logger();
    log_trace(logger, "desencolar_solicitud_sistema del PID: %i", pid);

    // 1. Buscar el recurso en el diccionario: get_recurso(nombre_recurso);
    t_recurso *recurso = get_recurso(key_recurso);

    // 2. Si no lo encuentra KILL al proceso
    if (recurso == NULL)
    {
        blackilst_process(pid, (motivo_de_muerte)INVALID_RESOURCE);
        find_and_kill(pid);
        log_trace(logger, "KILL al PID <%i> por desencolar a un recurso que no existe", pid);
        return false;
    }
    // si estamos liberando los recursos de un proceso debemos eliminar la solicitud de dicho proceso
    t_solicitud *solicitud = NULL;
    bool coincide_pid_y_no_esta_procesando(t_solicitud * solicitud)
    {
        return solicitud->pid == pid && !solicitud->procesando;
    }

    bool coincide_pid_y_esta_procesando(t_solicitud * solicitud)
    {
        return solicitud->pid == pid && solicitud->procesando;
    }
    pthread_mutex_lock(&recurso->mutex_recurso);

    if (es_exterminio)
    {
        // primero eliminamos todas las solicitudes del proceso que no se hayan procesado
        while (1)
        {
            solicitud = list_remove_by_condition(recurso->solicitudes, (void *)coincide_pid_y_no_esta_procesando);
            if (!solicitud)
            {
                break;
            }
            destroy_solicitud(solicitud, DONT_KILL_PROCESS);
        }
        // eliminamos todas las solicitudes del proceso que se esten procesando
        solicitud = list_remove_by_condition(recurso->solicitudes, (void *)coincide_pid_y_esta_procesando);

        if (solicitud)
        {
            recurso->instancias_disponibles++;
        }
        else
        {
            pthread_mutex_unlock(&recurso->mutex_recurso);
            return false;
        }
    }

    // en vez de desbloquear mi solicitud deberia desbloquear la primer solicitud que encuentre.

    else
    {
        solicitud = list_remove_by_condition(recurso->solicitudes, (void *)procesado);

        recurso->instancias_disponibles++;
    }
    // 3.2: eliminar lo que haga falta para no tener memory leaks
    if (solicitud != NULL)
    {
        destroy_solicitud(solicitud, DONT_KILL_PROCESS);
    }

    // 4. Si hay otras solicitudes sin procesar para el recurso procesarlas procesar_solicitud(t_solicitud solicitud)

    if (recurso->instancias_disponibles > 0 && list_any_satisfy(recurso->solicitudes, (void *)sin_procesar))
    {
        solicitud = list_find(recurso->solicitudes, (void *)sin_procesar);
        int next_pid = get_pid_from_instruccion(solicitud->tipo_instruccion, solicitud->data_instruccion);

        // 4. No importa si procesar_solicitud tira true or false, pero siempre debería tirar true.
        if (!procesar_solicitud(solicitud, recurso))
        {
            log_error(logger, "no se pudo procesar la solicitud siguiente, esto no debería pasar");
            assert(false); // 5. matamos la ejecucion si no encontramos la solicitud
        }

        // Si desencolamos un signal, la próxima solicitud es de un proceso bloqueado por wait
#ifndef TEST
        s_planificador_corto_args args = {.transicion = BLOCKED_A_READY, .op_pid.PID = next_pid};
        planificador_corto(args);
#endif
    }
    pthread_mutex_unlock(&recurso->mutex_recurso);

    // 5. retornar true si salió todo bien
    return true;
}

// bool solo_desencolar_solicitud_sistema(char *key_recurso, uint32_t pid,
//                                        bool es_exterminio) // NOLINT(misc-unused-parameters)
// {
//     t_log *logger = get_logger();
//     log_trace(logger, "desencolar_solicitud_sistema del PID: %i", pid);

//     // 1. Buscar el recurso en el diccionario: get_recurso(nombre_recurso);
//     t_recurso *recurso = get_recurso(key_recurso);

//     // 2. Si no lo encuentra KILL al proceso
//     if (recurso == NULL)
//     {
//         blackilst_process(pid, (motivo_de_muerte)INVALID_RESOURCE);
//         find_and_kill(pid);
//         log_trace(logger, "KILL al PID <%i> por desencolar a un recurso que no existe", pid);
//         return false;
//     }
//     // si estamos liberando los recursos de un proceso debemos eliminar la solicitud de dicho proceso
//     t_solicitud *solicitud = NULL;
//     bool coincide_pid_y_no_esta_procesando(t_solicitud * solicitud)
//     {
//         return solicitud->pid == pid && !solicitud->procesando;
//     }

//     bool coincide_pid_y_esta_procesando(t_solicitud * solicitud)
//     {
//         return solicitud->pid == pid && solicitud->procesando;
//     }
//     pthread_mutex_lock(&recurso->mutex_recurso);

//     if (es_exterminio)
//     {
//         // primero eliminamos todas las solicitudes del proceso que no se hayan procesado
//         while (1)
//         {
//             solicitud = list_remove_by_condition(recurso->solicitudes, (void *)coincide_pid_y_no_esta_procesando);
//             if (!solicitud)
//             {
//                 break;
//             }
//             destroy_solicitud(solicitud, DONT_KILL_PROCESS);
//         }
//         // eliminamos todas las solicitudes del proceso que se esten procesando
//         solicitud = list_remove_by_condition(recurso->solicitudes, (void *)coincide_pid_y_esta_procesando);

//         if (solicitud)
//         {
//             recurso->instancias_disponibles++;
//         }
//         else
//         {
//             pthread_mutex_unlock(&recurso->mutex_recurso);
//             return false;
//         }
//     }

//     // en vez de desbloquear mi solicitud deberia desbloquear la primer solicitud que encuentre.

//     else
//     {
//         solicitud = list_remove_by_condition(recurso->solicitudes, (void *)procesado);

//         recurso->instancias_disponibles++;
//     }
//     // 3.2: eliminar lo que haga falta para no tener memory leaks
//     if (solicitud != NULL)
//     {
//         destroy_solicitud(solicitud, DONT_KILL_PROCESS);
//     }

//     pthread_mutex_unlock(&recurso->mutex_recurso);
//     return true;
// }

gestor_recursos_statics get_gestor_recursos_statics()
{
    gestor_recursos_statics statics;
    statics.recursos = &recursos;
    statics.mutex_recursos = mutex_recursos;
    statics.create_solicitud = &create_solicitud;
    statics.destroy_solicitud_and_kill_process = &destroy_solicitud_and_kill_process;
    statics.procesar_solicitud = &procesar_solicitud;
    statics.create_recurso = &create_recurso;
    statics.get_recurso = &get_recurso;

    return statics;
}

void liberar_recursos(uint32_t pid)
{
    void _liberar_recurso_de_ser_necesario(char *key_recurso, t_recurso *recurso)
    {
        if (recurso->tipo == SISTEMA_SIGNAL_WAIT)
        {
            while (desencolar_solicitud_sistema(key_recurso, pid, true))
                ;
        }
        else
        {
            log_trace(get_logger(), "Tamos desencolando por liberar recursos del PID %i", pid);
            desencolar_solicitud_io(key_recurso, pid);
        }
    }
    dictionary_iterator(recursos, (void *)_liberar_recurso_de_ser_necesario);
}

/*
Vienes hasta aquí a pedirme un recurso ...
Sin embargo, no gestiono los recursos.
Decido el destíno de los procesos.
⠄⠄⠄⠄⠄⠄⠄⠄⠄⡀⠄⡀⠄⡀⠄⡀⢀⠄⡀⢀⠄⡀⠄⡀⠄⠄⠄⠄⢀⠄⠄⡀⢀⠄⠄⡀⠄⠄⠄⠄
⠄⠄⠄⠈⠄⠁⠈⠄⠂⠄⡀⠄⠄⡀⢀⠄⢀⠄⢀⠄⡀⠠⠄⠄⠂⠁⠈⡀⠄⠄⠁⠄⠄⠄⠂⠄⡀⠁⠄⠄
⠄⠄⠄⠁⠄⠁⠄⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄⢀⠄⡀⡀⠄⠄⠁⢀⢁⠄⡀⠠⠄⠁⡈⢀⠈⢀⠠⠄⢀⠄⠄
⠄⠄⠄⠄⠁⠄⠁⠄⠂⠄⡠⣲⢧⣳⡳⡯⣟⣼⢽⣺⣜⡵⣝⢜⢔⠔⡅⢂⠄⠄⠁⠄⢀⠄⡀⠄⡀⠄⠄⠄
⠄⠄⠄⠄⠈⠄⠈⠄⢀⡇⡯⡺⢵⣳⢿⣻⣟⣿⣿⣽⢾⣝⢮⡳⣣⢣⠣⡃⢅⠂⠐⠈⠄⠄⢀⠄⡀⠄⠄⠄
⠄⠄⠄⠄⠈⠄⠐⢀⠇⡪⡸⡸⣝⣾⣻⣯⣿⣿⡿⣟⣿⡽⣗⡯⣞⢜⢌⠢⡡⢈⠈⠄⠁⠈⠄⠄⠄⠄⠄⠄
⠄⠄⠄⠄⠐⠄⠈⠆⠕⢔⠡⣓⣕⢷⣻⣽⣝⢷⣻⣻⣝⢯⢿⠹⠸⡑⡅⠕⠠⠠⠄⠅⠄⠂⠄⠂⠈⠄⠄⠄
⠄⠄⠄⠄⠄⠂⠡⡑⢍⠌⡊⢢⢢⢱⠼⣺⢿⢝⠮⢪⣪⡺⣘⡜⣑⢤⢐⠅⠡⢂⠡⠐⡀⢀⠠⠐⠄⠐⠄⠄
⠄⠄⠄⠄⢈⢀⠡⠨⡢⡑⡌⡔⡮⡷⣭⢧⣳⠭⣪⣲⣼⣾⣟⣻⣽⣺⣸⣜⢌⢆⢌⠐⠄⡀⠄⡀⠄⠄⠄⠄
⠄⠄⠄⠄⠄⠠⠄⠌⡢⡵⠺⠞⠟⠛⠯⠟⠟⠝⡫⢗⠟⠝⠙⠉⠊⠑⠉⠉⠉⠑⢒⠠⠁⠄⡀⠠⠄⠄⠄⠄
⠄⠄⠄⠐⡀⠄⠄⠅⡪⠄⠂⠄⠄⠄⠄⠄⠄⠄⢀⢕⢔⠄⠄⠄⠄⡀⠠⠐⠈⢀⠄⠠⠄⡁⠄⡀⠂⠠⠄⠄
⠄⠄⠄⠠⠄⠄⠂⡑⠄⠄⠠⠐⠄⠁⠄⠁⠄⠄⢸⣿⣿⡂⠄⠄⢀⠄⡀⠄⠂⠠⠐⠄⡐⡀⠂⢀⠐⠄⠄⠄
⠄⠄⠄⠄⢐⠄⠂⢕⢅⢄⠄⣀⡀⢄⠄⠁⣀⣔⡵⣿⣯⠧⡣⣢⡠⢀⢀⡠⠐⢀⢐⠠⢀⠐⠄⠄⠄⠄⠄⠄
⠄⠄⠄⠄⠐⡔⢀⠘⢽⣻⣶⣥⣉⠥⡣⣱⣷⠻⣪⣻⣷⡣⡣⢫⣞⣗⡦⡵⢻⠺⡸⠐⡀⠐⠄⠂⡀⠄⠄⠄
⠄⠄⠄⠄⠂⠘⡀⠔⢀⠑⠍⠍⡽⣽⣿⣻⠂⡷⣯⡿⣟⡿⠌⡆⠘⣾⣻⢵⢕⠔⢀⠁⠠⠈⡀⠁⠄⡀⠄⠄
⠄⠄⠄⠄⠠⠄⠄⡐⢰⢈⢄⠱⢽⣺⢳⠁⣈⠄⠄⠈⠊⠈⠄⠄⢡⣐⢫⢯⡢⢊⢄⢪⠨⠠⠄⡀⠁⠄⠄⠄
⠄⠄⠄⠄⠂⠄⠂⠠⠱⣕⡣⡇⡏⢮⢕⣸⣾⠠⠄⠄⠄⠂⠄⠄⠌⢟⣜⡵⣯⢷⡴⡅⠅⡂⠠⠄⢈⠄⠄⠄
⠄⠄⠄⠄⠂⠁⢀⠈⠌⡪⢝⢾⣝⣎⠒⠏⠙⠠⠑⠁⠆⠒⠐⠐⠉⢀⠑⣍⡿⣽⡽⡂⠕⠄⠄⠂⢀⠠⠄⠄
⠄⠄⠄⠐⠄⡈⠄⢀⠄⠊⠍⢯⣷⣏⢊⢀⣈⣠⣤⣤⣤⣴⣶⢶⣴⢤⢬⣌⢻⡺⡻⠈⠄⠂⠄⠂⡀⠄⠄⠄
⠄⠄⠄⠄⠂⢀⠐⠄⠄⠂⠡⠑⠕⠅⡕⡽⡑⡁⠉⠉⠉⠉⠁⠁⠁⠠⢊⠊⠢⠈⠄⠨⠄⠄⠁⠐⢀⠈⠄⠄
⠄⠄⠄⠈⢀⠄⠄⠈⡀⢂⠐⠄⠂⠁⠠⠁⡢⡪⣢⣲⣦⣖⡔⡤⡨⡐⢄⠌⠠⠈⠐⠄⠂⠠⠁⢈⠠⠄⠄⠄
⠄⠄⠄⠄⠄⠄⠄⢂⠄⠢⠂⠈⡀⠈⡀⠈⠰⠹⡨⠑⡑⠕⠕⠊⠌⠌⠄⠐⠄⠂⠁⢈⠄⡁⠐⠄⡐⢀⠂⠄
⠄⠄⠄⠄⡐⢄⠑⠄⠄⡇⡁⠄⠄⠄⠄⡈⠄⠄⠄⠄⢀⠠⠄⠂⢀⠐⠄⡈⠠⠈⠄⠄⠠⠐⠄⠁⠠⠄⠄⠄
⠄⠄⡀⢊⠨⢀⢊⠄⠨⡂⡂⠄⠂⠄⢀⠄⠠⠄⠂⠄⠄⡀⠠⠄⠄⠄⠐⠄⠄⡀⠁⡀⠂⠄⠂⠁⠨⠄⠅⠄
⠄⠄⠐⠄⢂⠢⡀⠄⠬⠄⠂⠅⡀⠈⠄⠄⠄⠄⠄⠄⠄⠄⠄⡀⠂⠄⠂⠄⢀⠄⠄⠄⠄⠂⠄⠂⢈⠐⠄⠄
⠄⠄⠈⡀⠄⠄⠄⠄⠅⠅⠐⠄⠄⡀⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠁⠄⠄⠄⠂⢐⠄⠐⠄
⠄⠄⠄⠄⠄⠂⠄⠄⠕⠈⡂⡀⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠈⠄⠄⠂⠄
*/
