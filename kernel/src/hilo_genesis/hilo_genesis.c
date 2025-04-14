#include "hilo_genesis.h"

bool send_Kernel_memoria_creacion_call = false;
bool funcion_hilo_admision_call = false;

void iniciar_proceso(char *path_pseudocodigo)
{
    t_pcb *new_pcb = create_pcb();
#ifdef TEST
    send_Kernel_memoria_creacion_call = true;
#else
    send_Kernel_memoria_creacion(new_pcb->pid, path_pseudocodigo);
#endif

    add_pcb("NEW", new_pcb);
    log_proceso_creado((int)new_pcb->pid);
    // pthread_t HILO_GENESIS;
    // pthread_create(&HILO_GENESIS, NULL, hilo_genesis, new_pcb);
    // pthread_join(HILO_GENESIS);
#ifdef TEST
    funcion_hilo_admision_call = true;
#else
    pthread_t HILO_ADMIT;
    pthread_create(&HILO_ADMIT, NULL, (void *)admit, NULL);
#endif
}

// void *hilo_genesis(void *pcb)
// {
//     t_pcb *new_pcb = pcb;
//     add_pcb("NEW", new_pcb);

//     return NULL;
// }
