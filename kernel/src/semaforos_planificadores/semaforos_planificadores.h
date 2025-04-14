#ifndef SEMAFOROS_PLANIFICADORES
#define SEMAFOROS_PLANIFICADORES

#include <stdlib.h>

#include <semaphore.h>

// planificador corto plazo
extern sem_t semaforo_planificador_corto;
extern sem_t semaforo_transicion_new_ready;
extern sem_t semaforo_estado_exec;

void inicializar_semaforos_pausa_planificador_corto();
void destruir_semaforos_pausa_planificador_corto();

void detener_planificacion();

void reanudar_planificacion();

#endif