# Modulo: Entrada Salida ( I/O )

[Flow Diagram in Excalidraw](https://excalidraw.com/#json=r9xG1lKfU4dnrZEWhSt3j,WqC8wAliYNPyGstIXp13Iw)

## Mensajes


## IO → Memoria

- **IO_STDIN_READ**: Le pide a memoria escribir en un valor en una dirección
	- Está asociado a un PID
	- Hay un tamaño fijado por la instrucción
	- El valor del input lo ingresa el usuario en la IO
	- La instrucción original tiene una dir. lógica que el Procesador convirtió a una dir.física
- **IO_STDOUT_WRITE**: Le pide a  memoria el valor en una dirección de mem
	- Está asociado a un PID
	- La instrucción original tiene una dir. lógica que el Procesador convirtió a una dir.física
	- Tiene un tamaño dado por la instrucción
- **IO_FS_WRITE**: Le pide a memoria el valor en una dirección
	- Está asociado a un PID
	- Hay un tamaño fijado por la instrucción
	- La instrucción original tiene una dir. lógica que el Procesador convirtió a una dir. física
- **IO_FS_READ**: Le pide a memoria que escriba el valor en una dirección
	- Está asociado a un PID
	- Hay un tamaño fijado por la instrucción
	- La instrucción original tiene una dir. lógica que el Procesador convirtió a una dir. física

## Memoria → IO

- **IO_STDIN_READ**: OK ⚠️
- **IO_STDOUT_WRITE**: responde el valor en esa dirección
- **IO_FS_WRITE**: responde el valor en esa dirección
- **IO_FS_READ**: OK ⚠️

## Kernel → IO

- **IO_GEN_SLEEP** le pide a la IO que duerma
	- Asociado a un PID
	- Tiene una Unidad de trabajo
	- nombre de interfaz ? 
- **IO_STDIN_READ** le pide a la IO que lea del keeb
	- Asociado a un PID
	- Direccion de mem: Lógica que el CPU tradujo a física. (dónde guardar el input)
	- Tamaño (del input)
- **IO_STDOUT_WRITE** le pide a la IO que lea de mem e imprima por pantalla
	- Asociado a un PID
	- Direccion de mem: Lógica que el CPU tradujo a física. (dónde guardar el input)
	- Tamaño (del input)
- **IO_FS_CREATE:**
	- Crear un archivo en el FS
	- Nombre del file
- **IO_FS_DELETE**
	- Eliminar un archivo en el FS
	- Nombre del file
- **IO_FS_TRUNCATE**
	- Cambiar tamaño de un archivo en el FS
	- Nombre del file
	- Tamaño nuevo
- **IO_FS_WRITE**
	- Tomar de memoria valores y guardarlos en un file a partir del puntero del archivo
	- Nombre del file
	- Puntero del archivo (target)
	- Dir. lógica de mem traducida a física por el CPU (source)
	- Tamaño (cant de bytes a leer)
- **IO_FS_READ**
	- Leer valores de un archivo (en un puntero específico) y guardarlos en una dir de memoria
	- Puntero del archivo (source)
	- Nombre del file
	- Dir. lógica de mem traducida a física por el CPU (target)
	- Tamaño (cant de bytes a leer)

## IO → KERNEL

- **IO_GEN_SLEEP** OK + PID + nombre io ⚠️
- **IO_STDIN_READ** OK + PID + nombre io ⚠️
- **IO_STDOUT_WRITE** OK + PID + nombre io ⚠️
	- Dónde imprime tiene su pantalla ? 
- **IO_FS_CREATE:** OK + PID + nombre io ⚠️
- **IO_FS_DELETE** OK + PID + nombre io ⚠️
- **IO_FS_TRUNCATE** OK + PID + nombre io
- **IO_FS_TRUNCATE** Out of mem + PID + nombre io (not consigna)
- **IO_FS_WRITE** OK + PID + nombre io ⚠️
- **IO_FS_READ** OK + PID + nombre io ⚠️
