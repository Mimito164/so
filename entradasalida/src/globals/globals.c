#include "globals/globals.h"
#include <signal.h>
#include <unistd.h>
#define FINAL_MSG_LENGTH 12

static t_log *logger = NULL;
static int fd_memoria = 0;
static char *nombre_io = NULL;
static char *ruta_metadata = NULL;
static char *ruta_bloques = NULL;
static char *ruta_bitmap = NULL;

static void initialize_logger()
{
    logger = log_create("./logs/main_logger.log", "modulo IO", true, LOG_LEVEL_TRACE);
}

t_log *get_logger()
{
    if (logger == NULL)
    {
        initialize_logger();
    }
    return logger;
}

void set_fd_memoria(int valor)
{
    fd_memoria = valor;
}

int get_fd_memoria()
{
    return fd_memoria;
}

void destroy_logger()
{
    log_destroy(logger);
    logger = NULL;
}

void sighandler(int signum)
{
    if (signum == SIGINT)
    {
        write(STDOUT_FILENO, "\nCerrando IO\n", FINAL_MSG_LENGTH);
        mem_clean_exit();
    }
}

void setup_signal_handler()
{
    // Set up the sigaction structure
    struct sigaction sigact;
    sigact.sa_handler = sighandler;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);

    // Register the signal handler for SIGINT (Ctrl+C)
    if (sigaction(SIGINT, &sigact, NULL) == -1)
    {
        perror("sigaction");
        _Exit(EXIT_FAILURE);
    }
}

void check_args(int argc, char *argv[])
{
    if (argc < 2)
    {
        log_error(logger, "Se esperaba: %s entradasalida.config\n", argv[0]);
        destroy_logger();
        exit(EXIT_SUCCESS); // NOLINT
    }
}

void mem_clean_exit()
{
    destroy_logger();
    destroy_config_io();
    exit(EXIT_SUCCESS); // NOLINT TODO: cleanup all threads and all other strctures.
}

char *get_nombre_io()
{
    return nombre_io;
}
void set_nombre_io(char *nombre)
{
    nombre_io = nombre;
}

char *get_rutaMetadata()
{
    return ruta_metadata;
}
void set_rutaMetadata(char *rutaMetadata)
{
    ruta_metadata = rutaMetadata;
}

char *get_rutaBloques()
{
    return ruta_bloques;
}
void set_rutaBloques(char *rutaBloques)
{
    ruta_bloques = rutaBloques;
}

char *get_rutaBitmap()
{
    return ruta_bitmap;
}
void set_rutaBitmap(char *rutaBitmap)
{
    ruta_bitmap = rutaBitmap;
}