#include "globals/globals.h"
#define FINAL_MSG_LENGTH 16

static t_log *logger = NULL;

sem_t semaforo_consola;
sem_t executing_mutex;

static void initialize_logger()
{
    logger = log_create("./logs/main_logger.log", "modulo kernel", true, LOG_LEVEL_DEBUG);
}

t_log *get_logger()
{
    if (logger == NULL)
    {
        initialize_logger();
    }
    return logger;
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
        write(STDOUT_FILENO, "\nCerrando Kernel\n", FINAL_MSG_LENGTH);
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
        log_error(get_logger(), "Se esperaba: %s kernel.config\n", argv[0]);
        mem_clean_exit();
    }
}

void mem_clean_exit()
{
    destroy_logger();
    destroy_config_kernel();
    exit(EXIT_SUCCESS); // NOLINT TODO: cleanup all threads and all other strctures.
}
