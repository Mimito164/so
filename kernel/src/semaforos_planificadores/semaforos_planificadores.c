#include "semaforos_planificadores.h"

#define TRANSICIONES_MAXIMAS 3

sem_t semaforo_planificador_corto;
sem_t semaforo_transicion_new_ready;
sem_t semaforo_estado_exec;

void inicializar_semaforos_pausa_planificador_corto()
{
    sem_init(&semaforo_planificador_corto, 1, TRANSICIONES_MAXIMAS);
    sem_init(&semaforo_transicion_new_ready, 1, 1);
    sem_init(&semaforo_estado_exec, 1, 1);
}
void destruir_semaforos_pausa_planificador_corto()
{
    sem_destroy(&semaforo_planificador_corto);
    sem_destroy(&semaforo_transicion_new_ready);
    sem_destroy(&semaforo_estado_exec);
}

void detener_planificacion()
{
    for (int i = 0; i < TRANSICIONES_MAXIMAS; i++)
    {
        sem_wait(&semaforo_planificador_corto);
    }
    sem_wait(&semaforo_transicion_new_ready);
    sem_wait(&semaforo_estado_exec);
}

void reanudar_planificacion()
{
    for (int i = 0; i < TRANSICIONES_MAXIMAS; i++)
    {
        sem_post(&semaforo_planificador_corto);
    }
    sem_post(&semaforo_transicion_new_ready);
    sem_post(&semaforo_estado_exec);
}