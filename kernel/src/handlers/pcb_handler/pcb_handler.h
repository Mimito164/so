#ifndef PCB_HANDLER_H_
#define PCB_HANDLER_H_

#include <TAD/TAD.h>
#include <globals/globals.h>
#include <t_config/t_config.h>

typedef struct
{
    uint32_t pid;
    int quantum;
    t_registros_cpu *registros_cpu;
} t_pcb;

t_pcb *create_pcb();
void destroy_pcb(t_pcb *pcb_to_destroy);
t_contexto_ejecucion create_contexto(t_pcb *temp);
// de esta funcion para abajo veo innecesario pasarles un pcb ya que el 99% de los casos en los que las usemos
// de seguro va a ser sobre el pcb en executing, que de seguro va a ser una variable global que no va a cambiar
// hasta que cambiemos de proceso, entonces nada, al ser global no habria que pasarla por parametro
int update_pcb(t_pcb *pcb_to_update, t_contexto_ejecucion context);
void gh_lpm();

#endif
