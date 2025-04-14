#include "handlers_multiprogramacion.h"

static int multiprogramacion = 0;
static pthread_mutex_t mutex_multiprogramacion;

void init_mutex_multiprogramacion()
{
    pthread_mutex_init(&mutex_multiprogramacion, NULL);
    set_multiprogramacion(get_config()->GRADO_MULTIPROGRAMACION);
}

void destroy_mutex_multiprogramacion()
{
    pthread_mutex_destroy(&mutex_multiprogramacion);
}

void set_multiprogramacion(int new_value)
{
    pthread_mutex_lock(&mutex_multiprogramacion);
    multiprogramacion = new_value;
    pthread_mutex_unlock(&mutex_multiprogramacion);
}

int get_multiprogramacion()
{
    pthread_mutex_lock(&mutex_multiprogramacion);
    int temp = multiprogramacion;
    pthread_mutex_unlock(&mutex_multiprogramacion);
    return temp;
}
