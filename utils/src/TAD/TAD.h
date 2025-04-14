#ifndef TAD_H_
#define TAD_H_

#include <commons/collections/list.h>
#include <stdint.h>

typedef enum
{
    // Kernel a MEMORIA
    KERNEL_MEMORIA_CREAR_PROCESO,
    KERNEL_MEMORIA_FINALIZAR_PROCESO,
    // MEMORIA A KERNEL
    // NADA
    // KERNEL A CPU
    KERNEL_CPU_EXECUTE,
    KERNEL_CPU_INT_KILL,
    KERNEL_CPU_INT_DESALOJO,
    KERNEL_CPU_INT_KILL_ALREADY_KILLED,
    // CPU A KERNEL
    CPU_KERNEL_KILL_ALREADY_KILLED,
    CPU_KERNEL_KILL,
    CPU_KERNEL_DESALOJO_SIGNAL,          // Exec->BLocked
    CPU_KERNEL_DESALOJO_WAIT,            // Exec->BLocked
    CPU_KERNEL_DESALOJO_IO_GEN_ZZZ,      // Exec->BLocked
    CPU_KERNEL_DESALOJO_IO_STDIN_READ,   // Exec->BLocked
    CPU_KERNEL_DESALOJO_IO_STDOUT_WRITE, // Exec->BLocked
    CPU_KERNEL_DESALOJO_IO_FS_CREATE,    // Exec->BLocked
    CPU_KERNEL_DESALOJO_IO_FS_DELETE,    // Exec->BLocked
    CPU_KERNEL_DESALOJO_IO_FS_TRUNCATE,  // Exec->BLocked
    CPU_KERNEL_DESALOJO_IO_FS_WRITE,     // Exec->BLocked
    CPU_KERNEL_DESALOJO_IO_FS_READ,      // Exec->BLocked
    CPU_KERNEL_DESALOJO_OUT_OF_MEMORY,   // Exec->BLocked
    CPU_KERNEL_DESALOJO_FINQ,            // Exec->BLocked
    // KERNEL A IO
    KERNEL_IO_GEN_ZZZ,
    KERNEL_IO_STDIN_READ,
    KERNEL_IO_STDOUT_WRITE,
    KERNEL_IO_FS_CREATE,
    KERNEL_IO_FS_DELETE,
    KERNEL_IO_FS_TRUNCATE,
    KERNEL_IO_FS_WRITE,
    KERNEL_IO_FS_READ,
    // IO_KERNEL
    IO_KERNEL_INICIAR_CONEXION,
    IO_KERNEL_GEN_ZZZ,
    IO_KERNEL_STDIN_READ,
    IO_KERNEL_STDOUT_WRITE,
    IO_KERNEL_FS_CREATE,
    IO_KERNEL_FS_DELETE,
    IO_KERNEL_FS_TRUNCATE,
    IO_KERNEL_FS_WRITE,
    IO_KERNEL_FS_READ,
    // IO_MEMORIA
    IO_MEMORIA_READ,  // IO le pide a MEMORIA que lea y MEMORIA devuelve la info (io ni se fija el op code)
    IO_MEMORIA_WRITE, // aca IO no espera ninguna respuesta
    // MEMORIA a IO
    MEMORIA_IO_READ,  // ok
    MEMORIA_IO_WRITE, // memoria devuelve la info
    // CPU a MEMORIA
    CPU_MEMORIA_OBTENER_MARCO,
    CPU_MEMORIA_OBTENER_INSTRUCCIONES,
    CPU_MEMORIA_RESIZE,
    CPU_MEMORIA_COPIAR_STRING,
    CPU_MEMORIA_MOV_IN,  // le pido a memoria un valor
    CPU_MEMORIA_MOV_OUT, // guardo en memoria un valor
    CPU_MEMORIA_OBTENER_TAM_PAG,
    // MEMORIA_CPU
    MEMORIA_CPU_OBTENER_MARCO,
    MEMORIA_CPU_OBTENER_INSTRUCCIONES,
    MEMORIA_CPU_RESIZE,
    MEMORIA_CPU_COPIAR_STRING,
    MEMORIA_CPU_MOV_IN,
    MEMORIA_CPU_MOV_OUT,
    MEMORIA_CPU_OUT_OF_MEMORY,
    MEMORIA_CPU_TAM_PAG,
    // MSJ GENERAL
    OK_GRAL
} op_code;

typedef struct
{
    uint32_t PC;
    uint8_t AX;
    uint8_t BX;
    uint8_t CX;
    uint8_t DX;
    uint32_t EAX;
    uint32_t EBX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t SI;
    uint32_t DI;
} t_registros_cpu;

typedef struct
{
    uint32_t pid;
    t_registros_cpu registros_cpu;
} t_contexto_ejecucion;

typedef enum
{
    DialFS,
    STDOUT,
    STDIN,
    GENERICA
} tipo_IO;

typedef struct
{
    uint32_t pid;
    uint32_t offset;
    uint32_t padding;
    uint32_t tam_escritura;
    uint32_t frame;
    uint32_t tam_pag;
    void *data;
} frames_element;

#endif