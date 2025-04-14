# tp-C-Commenta

Esta es una plantilla de proyecto diseñada para generar un TP de Sistemas
Operativos de la UTN FRBA.

----

## Modulos 

* [CPU](./cpu/README.md)
* [Memoria](./memoria/README.md)
* [Kernel](./kernel/README.md)
* [I/O](./entradasalida/README.md)

----

## Dependencias

Para poder compilar y ejecutar el proyecto, es necesario tener instalada la
biblioteca [so-commons-library] de la cátedra:

```bash
git clone https://github.com/sisoputnfrba/so-commons-library
cd so-commons-library
make debug
make install
```
## Script 

Para probar los modulos pueden hacer uso del `script` ( Deben tener tmux ) 

El script realiza lo siguiente:

1. Abrir 4 panes de tmux
2. Cambia a una carpeta de un modulo
3. Make clean
4. Un echo de prueba
5. Make 

A los paneles se puede acceder con el mouse y moverse dentro de ellos 


### Forma 1:

```
bash tmux-script.sh
```
### Forma 2: 

Probar 

```
./tmux-script.sh
```

Si no funciona o tira error puede ser por los permisos de ejecucion, hay que hacer un paso mas para que quede y poder usar el comando de arriba 

```
chmod u+x tmux-script.sh
```

Esto lo que hace es al usuario darle permisos de ejecucion al script

## Cerrar el script 

Una forma es cerrando la terminal pero si queres mantenerte dentro de la termina y usarla normal sin tmux podes usar el siguiente comando 

```
tmux kill-server
```

Esto mataria el "server" que se creo de tmux volviendo a la terminal

> Recomendacion agregar un alias ( se agrega en .bashrc ) 

```
alias tks='tmux kill-server'
```

### Forma 2: 

En esta forma vamos a usar el comando `exit`,  n veces, dependiendo cuantos panes tengas 

## Script de testeo de Modulos 

Para correr el modulo hay dos formas: 

```
bash  test-modules.sh

```
La siguiente forma puede ser que falle si no se tiene los permisos adecuados 
```
./test-modules
```

## Compilación

Cada módulo del proyecto se compila de forma independiente a través de un
archivo `makefile`. Para compilar un módulo, es necesario ejecutar el comando
`make` desde la carpeta correspondiente.

El ejecutable resultante se guardará en la carpeta `bin` del módulo.

## Importar desde Visual Studio Code

Para importar el workspace, debemos abrir el archivo `tp.code-workspace` desde
la interfaz o ejecutando el siguiente comando desde la carpeta raíz del
repositorio:

```bash
code tp.code-workspace
```


## Checkpoint

Para cada checkpoint de control obligatorio, se debe crear un tag en el
repositorio con el siguiente formato:

```
checkpoint-{número}
```

Donde `{número}` es el número del checkpoint.

Para crear un tag y subirlo al repositorio, podemos utilizar los siguientes
comandos:

```bash
git tag -a checkpoint-{número} -m "Checkpoint {número}"
git push origin checkpoint-{número}
```

Asegúrense de que el código compila y cumple con los requisitos del checkpoint
antes de subir el tag.

## Entrega

Para desplegar el proyecto en una máquina Ubuntu Server, podemos utilizar el
script [so-deploy] de la cátedra:

```bash
git clone https://github.com/sisoputnfrba/so-deploy.git
cd so-deploy
./deploy.sh -r=release -p=utils -p=kernel -p=cpu -p=memoria -p=entradasalida "tp-{año}-{cuatri}-{grupo}"
```

El mismo se encargará de instalar las Commons, clonar el repositorio del grupo
y compilar el proyecto en la máquina remota.

Ante cualquier duda, podés consultar la documentación en el repositorio de
[so-deploy], o utilizar el comando `./deploy.sh -h`.

[so-commons-library]: https://github.com/sisoputnfrba/so-commons-library
[so-deploy]: https://github.com/sisoputnfrba/so-deploy

# How to run the project

_antes de compilar es preciso tener instaladas todas las dependencias: commons y readline. Luego será posible compilar_

## Orden de despliegue de los modulos

1. Memoria
2. CPU
3. Kernel
4. IO (múltiples)
 
## Orden de cierre

1. IO (todas)
2. Kernel
3. CPU
4. Memoria
   
## Diagrama de flujo
https://excalidraw.com/#json=uh0iwA7Qb7LaVlPJVkMj4,p-9JlQqCHjFpdqbn1ojixQ