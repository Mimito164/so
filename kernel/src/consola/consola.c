#include "consola.h"

#define LINE_MAX 100
#define ES_SCRIPT true
#define NO_ES_SCRIPT false
#define BASE_STROL 10

pthread_t HILO_CONOSOLA;

static bool planificacion_detenida = false;

static void ejec_inst(char *linea, bool isScript);
static bool instrccuion_valida(char **instruccion, bool isScript);
static void ejecutar_script(char *path);
static bool path_correcto(char *path);
static void finalizar_proceso(char *pid);
static void proceso_estado();
static void consola_exit();

void *consola()
{
    char *linea;

    while (1)
    {
        linea = readline("> ");
        if (linea)
        {
            add_history(linea);
        }

        if (linea == NULL)
        {
            continue;
        }

        if (strncmp(linea, "EXIT", 4) == 0)
        {
            consola_exit();
            free(linea);
#ifdef TEST
            sem_post(&semaforo_consola);
#endif
            break;
        }

        ejec_inst(linea, NO_ES_SCRIPT);
        free(linea);
    }

    return NULL;
}

void iniciar_consola()
{
    iniciar_diccionario();
    pthread_create(&HILO_CONOSOLA, NULL, &consola, NULL);
    pthread_detach(HILO_CONOSOLA);
}

// TODO(Fede): implementar la funcion que mata al hilo de la consola
// void finalizar_consola()
// {
//     pthread_kill(HILO_CONOSOLA, SIGKILL);
// }

void ejecutar_script(char *path)
{
    t_log *logger = get_logger();
    FILE *script = NULL;
    char line[LINE_MAX];

    script = fopen(path, "r");

    while (fgets(line, LINE_MAX, script))
    {
        char *clean_line = string_replace(line, "\n", "");
        ejec_inst(clean_line, ES_SCRIPT);
    }

    if (fclose(script) == 0)
    {
        script = NULL;
        return;
    }
    log_error(logger, "no se cerro el archivo de script correctamente");
    script = NULL;
}

static void finalizar_proceso(char *pid)
{
    long int_pid = strtol(pid, NULL, BASE_STROL);
    blackilst_process(int_pid, (motivo_de_muerte)INTERRUPTED_BY_USER);
    find_and_kill(int_pid);
}

void place_holder_iniciar_planificacion()
{
    t_log *logger = get_logger();
    log_info(logger, "Iniciar planificador: Ponele que se ejecuto bien (posta) B)");
}

void proceso_estado()
{
#ifdef TEST
    log_info(get_logger(), "Proceso estado: Ponele que se ejecuto bien (posta) B)");
#else
    print_all_lists();
#endif
}
void consola_exit()
{
    t_log *logger = get_logger();
    log_info(logger, "Finalizo la consola");
}

static bool path_correcto(char *path)
{
    FILE *fptr = fopen(path, "r");

    if (fptr == NULL)
    {
        return false;
    }

    if (fclose(fptr) != 0)
    {
        fptr = NULL;
        return false;
    }
    return true;
}

static bool instrccuion_valida(char **instruccion, bool isScript)
{
    t_log *logger = get_logger();

    if (!dictionary_has_key(argumentos_comandos, instruccion[0]))
    {
        log_error(logger, "No se reconoce ese comando.");
        return false;
    }

    intptr_t argumentos_esperados = (intptr_t)dictionary_get(argumentos_comandos, instruccion[0]);
    intptr_t cantidad_argumentos = string_array_size(instruccion) - 1;

    if (argumentos_esperados != cantidad_argumentos)
    {
        log_error(logger, "La cantidad argumentos es distinta a la esperada.");
        return false;
    }

    if (isScript)
    {
        if (strcmp(instruccion[0], "EJECUTAR_SCRIPT") == 0)
        {
            log_error(logger, "Estas queriendo ejecutar \"EJECUTAR_SCRIPT\" dentro de un script");
            return false;
        }

        if (strcmp(instruccion[0], "EXIT") == 0)
        {
            log_error(logger, "Estas queriendo ejecutar \"EXIT\" dentro de un script");
            return false;
        }
    }

    return true;
}

static void ejec_inst(char *linea, bool isScript)
{
    char **instruccion = string_split(linea, " ");
    t_log *logger = get_logger();

    if (!instrccuion_valida(instruccion, isScript)) // no me gusta el nombre >:/
    {
        return;
    }

    if (strcmp(instruccion[0], "INICIAR_PROCESO") == 0)
    {
        if (!path_correcto(instruccion[1]))
        {
            log_error(logger, "el path indicado no es correcto");
            return;
        }

        iniciar_proceso(instruccion[1]);
    }
    if (strcmp(instruccion[0], "FINALIZAR_PROCESO") == 0)
    {
#ifdef TEST
        log_info(logger, "Finalizar proceso: Ponele que se ejecuto bien B)");
#else
        finalizar_proceso(instruccion[1]);
#endif
    }
    if (strcmp(instruccion[0], "DETENER_PLANIFICACION") == 0)
    {
#ifdef TEST
        log_info(logger, "Detener planificador: Ponele que se ejecuto bien B)");
#else
        if (planificacion_detenida)
        {
            log_warning(logger, "La planificacion ya esta detenida");
        }
        else
        {
            detener_planificacion();
            planificacion_detenida = true;
            log_info(logger, "Planificacion Detenida");
        }

#endif
    }
    if (strcmp(instruccion[0], "INICIAR_PLANIFICACION") == 0)
    {
#ifdef TEST
        log_info(logger, "Iniciar planificador: Ponele que se ejecuto bien B)");
#else
        if (planificacion_detenida)
        {
            reanudar_planificacion();
            planificacion_detenida = false;
            log_info(logger, "Planificacion Iniciada");
        }
        else
        {
            log_warning(logger, "La planificacion ya esta Iniciada");
        }
#endif
    }
    if (strcmp(instruccion[0], "MULTIPROGRAMACION") == 0)
    {
#ifdef TEST
        log_info(logger, "Multiprogramacion: Ponele que se ejecuto bien B)");
#else
        int new_multi = (int)strtol(instruccion[1], NULL, 0);
        update_multiprogramacion(new_multi);
#endif
    }
    if (strcmp(instruccion[0], "PROCESO_ESTADO") == 0)
    {
#ifdef TEST
        log_info(logger, "Proceso estado: Ponele que se ejecuto bien B)");
#else
        proceso_estado();
#endif
    }

    if (strcmp(instruccion[0], "EJECUTAR_SCRIPT") == 0)
    {

        if (!path_correcto(instruccion[1]))
        {
            log_error(logger, "el path indicado no es correcto");
            return;
        }

        ejecutar_script(instruccion[1]);
    }
    string_array_destroy(instruccion);
}
