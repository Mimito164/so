#include "interrupt_handlers.h"

static pthread_mutex_t mutex_interrupt;

static t_interrupt interruption;

void init_mutex_interrupt()
{
    pthread_mutex_init(&mutex_interrupt, NULL);
}

void destroy_mutex_interrupt()
{
    pthread_mutex_destroy(&mutex_interrupt);
}

void set_interrupt(int pid, interrupt_type type) // NOLINT(bugprone-easily-swappable-parameters)
{
    pthread_mutex_lock(&mutex_interrupt);
    interruption.pid = pid;
    interruption.type = type;
    interruption.interrupt_happend = true;
    pthread_mutex_unlock(&mutex_interrupt);
}
bool get_interrupt_state()
{
    pthread_mutex_lock(&mutex_interrupt);
    bool state = interruption.interrupt_happend;
    pthread_mutex_unlock(&mutex_interrupt);
    return state;
}
int get_interrupt_pid()
{
    pthread_mutex_lock(&mutex_interrupt);
    int pid = interruption.pid;
    pthread_mutex_unlock(&mutex_interrupt);
    return pid;
}
interrupt_type get_interrupt_type()
{
    pthread_mutex_lock(&mutex_interrupt);
    interrupt_type type = interruption.type;
    pthread_mutex_unlock(&mutex_interrupt);
    return type;
}

void clear_interrupt()
{
    pthread_mutex_lock(&mutex_interrupt);
    interruption.pid = -1;
    interruption.type = NONE;
    interruption.interrupt_happend = false;
    pthread_mutex_unlock(&mutex_interrupt);
}