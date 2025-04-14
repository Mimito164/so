En todos los casos no se va a tener en cuenta el opcode. Solamente se va a analizar lo que este dentro del buffer.

**El orden de serializacion/deserializacion es de izquieda a derecha como se lee en la tabla.**

# KERNEL ←→ cpu

## Interrupciones

|      <!-- -->       | <!-- --> |
|:-------------------:|:--------:|
|      variable       |   PID    |
| tipo de la variable |  int32   |

* El mismo opcode va a indicar si la interrupcion es por kill o por desalojo (`KERNEL_CPU_INT_KILL`,`KERNEL_CPU_INT_DESALOJO`)...

## Contexto de ejecucion

|      <!-- -->       | <!-- --> | <!-- --> | <!-- --> | <!-- --> | <!-- --> | <!-- --> | <!-- --> | <!-- --> | <!-- --> | <!-- --> | <!-- --> | <!-- --> |
|:-------------------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|
|      variable       |   PID    |    PC    |    AX    |    BX    |    CX    |    DX    |   EAX    |   EBX    |   ECX    |   EDX    |    SI    |    DI    |
| tipo de la variable |  uint32  |  uint32  |  uint8   |  uint8   |  uint8   |  uint8   |  uint32  |  uint32  |  uint32  |  uint32  |  uint32  |  uint32  |


### `CPU_KERNEL_KILL`

| <!-- --> |                 <!-- -->                 |
|:--------:|:----------------------------------------:|
| varable  |          `<CONTEXTO_EJECUCION>`          |
|  tamaño  | los tamaños correspondientes al cotnexto |

### `CPU_KERNEL_DESALOJO_FINQ`

| <!-- --> |                 <!-- -->                 |
|:--------:|:----------------------------------------:|
| varable  |          `<CONTEXTO_EJECUCION>`          |
|  tamaño  | los tamaños correspondientes al cotnexto |

### `CPU_KERNEL_DESALOJO_SIGNAL`

| <!-- --> |                 <!-- -->                 |      <!-- -->      |
|:--------:|:----------------------------------------:|:------------------:|
| varable  |          `<CONTEXTO_EJECUCION>`          | nombre del recurso |
|  tamaño  | los tamaños correspondientes al cotnexto |       char*        |

### `CPU_KERNEL_DESALOJO_WAIT`

| <!-- --> |                 <!-- -->                 |      <!-- -->      |
|:--------:|:----------------------------------------:|:------------------:|
| varable  |          `<CONTEXTO_EJECUCION>`          | nombre del recurso |
|  tamaño  | los tamaños correspondientes al cotnexto |       char*        |

### `CPU_KERNEL_DESALOJO_IO`

| <!-- --> |                 <!-- -->                 |    <!-- -->     |
|:--------:|:----------------------------------------:|:---------------:|
| varable  |          `<CONTEXTO_EJECUCION>`          | nombre de la IO |
|  tamaño  | los tamaños correspondientes al cotnexto |      char*      |