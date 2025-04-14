#include "console.h"

static int stdout_fd;

void consola_con_censura()
{
    stdout_fd = dup(STDOUT_FILENO);
    int null_output_fd = open("/dev/null", O_WRONLY); // /dev/null es un archivo especial de linux
    dup2(null_output_fd, STDOUT_FILENO);
    close(null_output_fd);
}

void consola_con_libre_expresion()
{
    dup2(stdout_fd, STDOUT_FILENO);
    close(stdout_fd);
}