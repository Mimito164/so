# TLB
```
 - TLB: (TBD)
    - Que se pueda habilitar y deshabilitar
    - Estructura inicial:
      - PID | NºPAG | NºMARCO | TIMESTAMP
    - Algoritmos:
      - FIFO
      - LRU
  - hacer la funcion que define la jerarquia de motivos de devolucion (FEDE)
  - logs minimos
```

## Tabla de paginas 

``` c
[ // lista de listas de marcos asignados
    [0, 2,  4 ], // lista de marcos asignados del proceso 0
    [-1], // lista de marcos asignados del proceso 1. En este caso el proceso fue eliminado por eso guardamos -1 para que se respete el indice con el numero del pid de cada proceso
    [6, 9], // lista de marcos asignados del proceso 2
    [], 
    [],  // 0 * tam. bytes de cada marco
    [],
    // las listas comienzan vacias
    // si hay un resize 
]
``` 
1. Funcion para crear lista de listas de marcos asignados , al principio esta vacia.
2. hacer una funcion que cree un sub lista para cada proceso. Automaticamente va a ir incrementando el indice (creo que la propia implementacion de las commons resuelve esto).
3. Resize para cierto pid
   1. hacer la resta del tamaño de la lista de marcos asignados
      1. Puede pasar que tengas que asignar marcos
         1. funcion que obtenga los marcos libres consecutivos
            1. Si no hay suficientes responder out of memory
      2. Puede pasar que tengas que liberar marcos
         1. Eliminar los ultimos marcos de cada lista
         2. marcar como diponibles los marcos.
4. Eliminar un proceso 
   1. liberar marcos
         1. marcar como diponibles los marcos.
         2. Eliminar todos los marcos de la lista