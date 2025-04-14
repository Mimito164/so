#include "globals/globals.h"
#include <signal.h>
#include <unistd.h>
#define FINAL_MSG_LENGTH 13

static t_log *logger = NULL;
static uint32_t tam_pag;

static void initialize_logger()
{
    logger = log_create("./logs/main_logger.log", "modulo CPU", true, LOG_LEVEL_DEBUG);
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
        write(STDOUT_FILENO, "\nCerrando CPU\n", FINAL_MSG_LENGTH);
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
        log_error(logger, "Se esperaba: %s cpu.config\n", argv[0]);
        destroy_logger();
        exit(EXIT_SUCCESS); // NOLINT TODO: cleanup all threads and all other strctures.
    }
}

void mem_clean_exit()
{
    destroy_logger();
    destroy_config_cpu();
    exit(EXIT_SUCCESS); // NOLINT TODO: cleanup all threads and all other strctures.
}

uint32_t get_tam_pag()
{
    return tam_pag;
}

void set_tam_pag(uint32_t tam)
{
    tam_pag = tam;
}