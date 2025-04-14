#ifndef TRANSICIONES_H_
#define TRANSICIONES_H_

typedef enum
{
    BLOCKED_A_EXIT,
    BLOCKED_A_READY,
    READY_A_EXEC,
    EXEC_A_READY,
    EXEC_A_BLOCKED,
    EXEC_A_EXIT,
    NEW_A_READY,
} t_transicion;

#endif