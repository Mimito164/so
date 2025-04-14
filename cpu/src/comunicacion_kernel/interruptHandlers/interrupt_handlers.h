#ifndef INTERRUPT_HANDLERS_H_
#define INTERRUPT_HANDLERS_H_

#include <pthread.h>
#include <stdbool.h>

typedef enum
{
    KILL_KILLED,
    KILL,
    DESALOJO,
    NONE
} interrupt_type;
typedef struct Interruption
{
    bool interrupt_happend;
    int pid;
    interrupt_type type;
} t_interrupt;

/// @brief Inicializa el mutex de la variable interrupt
void init_mutex_interrupt();

/// @brief Finaliza el mutex de la variable interrupt
void destroy_mutex_interrupt();

/// @brief A partir de un `pid` y `type` que llega desde el puerto de interrupt modifica la variable de interrupt.
/// @param pid El pid correspondiente al proceso que se desea interrumpir. Viene desde el puerto interrupt.
/// @param type EL tipo de interrupcion. Dependiendo del opcode puede ser `KILL` o `DESALOJO`.
void set_interrupt(int pid, interrupt_type type);

/// @brief Indica si llego una interrupcion por el puerto interrupt.
/// @return `true` si hubo una interrupcion. `false` si no hubo una interrupcion.
bool get_interrupt_state();

/// @brief getter para el pid del proceso que se desea interrumpir
/// @return pid del proceso que se desea interrumpir
int get_interrupt_pid();

/// @brief getter del tipo de interrupcion.
/// @return KILL, DESALOJO o NONE (si se llama a este getter cuando no hay una interrupcion).
interrupt_type get_interrupt_type();

/**
 * @brief Resetea la variable de interrupcion\
 * @brief Puntualmente setea:
 * @brief - `interrupt_happend` en `false`
 * @brief - `pid` en `-1`
 * @brief - `type` en `NONE`
 **/
void clear_interrupt();

#endif