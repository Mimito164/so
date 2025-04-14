#!/bin/bash

#------------------- Flags del script ----------------------
clean= false
valgrind= false

while getopts :cv flag
do
    case "${flag}" in
        c) clean=true;;
        v) valgrind=true;;
    esac
done
#----------------- END Flags del script -------------------

echo """
------------------------------------------------------------
| Test de Prueba 5 : FS                                    |
------------------------------------------------------------

Actividades
1. Iniciar los módulos.

2. Ejecutar los siguientes comandos en la consola del Kernel
  - INICIAR_PROCESO /scripts_memoria/FS_1
  - INICIAR_PROCESO /scripts_memoria/FS_2

3. Ingresar para FS_1 el siguiente texto sin comillas “Fallout 1 Fallout 2 Fallout 3 Fallout: New Vegas Fallout 4 Fallout 76”

4. Esperar a que finalicen los 2 procesos y cortar la prueba finalizando la ejecución de todos los módulos.

5. Iniciar nuevamente los módulos.

Ejecutar los siguientes comandos en la consola del Kernel
  - INICIAR_PROCESO /scripts_memoria/FS_3
  - INICIAR_PROCESO /scripts_memoria/FS_4

Esperar a que finalice el proceso y cortar la prueba.

Resultados Esperados:
- El proceso FS_1 genera 2 archivos y escribe en ellos.
- El proceso FS_2 genera 2 archivos de tamaño 10 bytes.
- El proceso FS_3 lee el contenido del primer archivo creado por FS_1.
- El proceso FS_4 crea un archivo de 250 bytes, elimina el primer archivo creado por FS_2 y al querer ampliar el segundo archivo de los creados por FS_2 requiere de compactar

------------------------------------------------------------
| GL HF Preciona una tecla para continuar                  |
------------------------------------------------------------
"""

read -n1 -s -r key

#--------------------- Memoria ------------------------------

# TAM_MEMORIA=1024        
# TAM_PAGINA=16           
# RETARDO_RESPUESTA=100  

file="./memoria/memoria.config"

attribute="TAM_MEMORIA"    
att_TAM_MEMORIA="1024"
attribute2="TAM_PAGINA"
att_TAM_PAGINA="16"
attribute3="RETARDO_RESPUESTA"
att_RETARDO_RESPUESTA="100"

sed -i "s/^$attribute=.*/$attribute=$att_TAM_MEMORIA/" "$file"
sed -i "s/^$attribute2=.*/$attribute2=$att_TAM_PAGINA/" "$file"
sed -i "s/^$attribute3=.*/$attribute3=$att_RETARDO_RESPUESTA/" "$file"

#--------------------- CPU ---------------------------------

# CANTIDAD_ENTRADAS_TLB=0
# ALGORITMO_TLB=FIFO

file2="./cpu/cpu.config"

attribute="CANTIDAD_ENTRADAS_TLB"    
att_CANTIDAD_ENTRADA_TLB="0"
attribute2="ALGORITMO_TLB"
att_ALGORITMO_TLB="FIFO"

sed -i "s/^$attribute=.*/$attribute=$att_CANTIDAD_ENTRADA_TLB/" "$file2"
sed -i "s/^$attribute2=.*/$attribute2=$att_ALGORITMO_TLB/" "$file2"


#---------------------- Kenel -------------------------

file3="./kernel/kernel.config" 

# ALGORITMO_PLANIFICACION=RR
# QUANTUM=5000
# RECURSOS=[REC1]
# INSTANCIAS_RECURSOS=[1]
# GRADO_MULTIPROGRAMACION=10

attribute="ALGORITMO_PLANIFICACION" 
att_ALGORITMO_PLANIFICACION="RR"
attribute2="QUANTUM"   
att_QUANTUM="5000"
attribute3="RECURSOS"
att_RECURSOS="[REC1]"
attribute4="INSTANCIAS_RECURSOS"   
att_INSTANCIAS_RECURSOS="[1]"
attribute5="GRADO_MULTIPROGRAMACION"   
att_GRADO_MULTIPROGRAMACION="10"

sed -i "s/^$attribute=.*/$attribute=$att_ALGORITMO_PLANIFICACION/" "$file3"
sed -i "s/^$attribute2=.*/$attribute2=$att_QUANTUM/" "$file3"
sed -i "s/^$attribute3=.*/$attribute3=$att_RECURSOS/" "$file3"
sed -i "s/^$attribute4=.*/$attribute4=$att_INSTANCIAS_RECURSOS/" "$file3"
sed -i "s/^$attribute5=.*/$attribute5=$att_GRADO_MULTIPROGRAMACION/" "$file3"

#---------------------- Entrada Saluda 0 -------------------------
#---------------------- DIALFS           -------------------------

file4="./entradasalida/FS.config"

# TIPO_INTERFAZ=DIALFS
# TIEMPO_UNIDAD_TRABAJO=2000
# BLOCK_SIZE=16
# BLOCK_COUNT=32
# RETRASO_COMPACTACION=7500

attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="DIALFS"
attribute2="TIEMPO_UNIDAD_TRABAJO"   
att_TIEMPO_UNIDAD_TRABAJO="2000"
attribute3="BLOCK_SIZE"   
att_BLOCK_SIZE="16"
attribute4="BLOCK_COUNT"
att_BLOCK_COUNT="32"
attribute5="RETRASO_COMPACTACION"   
att_RETRASO_COMPACTACION="7500"

sed -i "s/^$attribute=.*/$attribute=$att_TIPO_INTERFAZ/" "$file4"
sed -i "s/^$attribute2=.*/$attribute2=$att_TIEMPO_UNIDAD_TRABAJO/" "$file4"
sed -i "s/^$attribute3=.*/$attribute3=$att_BLOCK_SIZE/" "$file4"
sed -i "s/^$attribute4=.*/$attribute4=$att_BLOCK_COUNT/" "$file4"
sed -i "s/^$attribute5=.*/$attribute5=$att_RETRASO_COMPACTACION/" "$file4"

# Limpiar los archivos si hay creados
rm -rf ./entradasalida/estructuras/FS/*


#---------------------- Entrada Saluda 1 -------------------------
#---------------------- TECLADO       ----------------------------

file4="./entradasalida/TECLADO.config"

# TIPO_INTERFAZ=STDIN
# TIEMPO_UNIDAD_TRABAJO=250


attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="STDIN"
attribute2="TIEMPO_UNIDAD_TRABAJO"   
att_TIEMPO_UNIDAD_TRABAJO="250"


sed -i "s/^$attribute=.*/$attribute=$att_TIPO_INTERFAZ/" "$file4"
sed -i "s/^$attribute2=.*/$attribute2=$att_TIEMPO_UNIDAD_TRABAJO/" "$file4"


#---------------------- Entrada Saluda 3 -------------------------
#---------------------- MONITOR       ----------------------------

file4="./entradasalida/MONITOR.config"

# TIPO_INTERFAZ=STDOUT
# TIEMPO_UNIDAD_TRABAJO=250

attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="STDOUT"
attribute2="TIEMPO_UNIDAD_TRABAJO"   
att_TIEMPO_UNIDAD_TRABAJO="250"


sed -i "s/^$attribute=.*/$attribute=$att_TIPO_INTERFAZ/" "$file4"
sed -i "s/^$attribute2=.*/$attribute2=$att_TIEMPO_UNIDAD_TRABAJO/" "$file4"


# ------------------------- Compilando todo ----------------------

if [ $clean ]
    then
    echo CLEAN
    # Comilando todo en paralelo
    (cd utils && make clean && make) &
    (cd memoria && make clean && make) &
    (cd cpu && make clean && make) &
    (cd kernel && make clean && make) &
    (cd entradasalida && make clean && make) &

    else
    echo NO CLEAN
    # Comilando todo en paralelo
    (cd utils && make) &
    (cd memoria && make) &
    (cd cpu && make) &
    (cd kernel && make) &
    (cd entradasalida && make) &

fi
wait # Esperando a que todo comile bien


# -------------------- Levantando la sesión de TMUX --------------------------

touch  ~/.tmux.conf # archivo de config de tmux
echo "setw -g mouse on" > ~/.tmux.conf # en la config de tmux habilito el mouse 

tmux kill-session -t my_session # elimino la vieja por las dudas
tmux new-session -d -s my_session # nueva sesion de tmux

tmux source-file ~/.tmux.conf # uso la config que armé arriba

# %0 panel 0, el default
tmux split-window -v # Split window horizontally → Ahora tenemos panel %0 y %1
tmux split-window -v  # Split the second pane vertically → Ahora tenemos panel %0 %1 %2
tmux select-layout even-vertical


tmux new-window # Create a new tmux window → Ahora tenemos panel %0 %1 %2 %3
tmux split-window -v # Split window horizontally → Ahora tenemos panel %0 %1 %2 %3 %4
tmux split-window -v  # Split the second pane vertically → Ahora tenemos panel %0 %1 %2 %3 %4 %5
tmux select-layout even-vertical


tmux select-window -t 0 # Switch a la primer window

# 1. Memoria
rm ./memoria/logs/main_logger.log
tmux send-keys -t %0 'cd memoria' Enter
if [ $valgrind ]
    then
    tmux send-keys -t %0 'valgrind --leak-check=full --show-leak-kinds=all ./bin/memoria memoria.config' Enter
    else
    tmux send-keys -t %0 './bin/memoria memoria.config' Enter
fi

while ! tmux capture-pane -p -t %0 | grep "Escuchando en 127.0.0.1:" ; do
  # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
  echo Esperando MEMORIA:
  echo  $(tmux capture-pane -p -t %0)
  sleep 0.3
done ;

# 2. CPU
rm ./cpu/logs/main_logger.log

tmux send-keys -t %1 'cd cpu' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %1 'valgrind --leak-check=full --show-leak-kinds=all ./bin/cpu cpu.config' Enter
    else
    tmux send-keys -t %1 './bin/cpu cpu.config' Enter
fi

while ! tmux capture-pane -p -t %1 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando CPU:
    echo $(tmux capture-pane -p -t %1)
    sleep 0.3
done ;

# 3. Kernel
rm ./kernel/logs/main_logger.log

tmux send-keys -t %2 'cd kernel' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %2 'valgrind --leak-check=full --show-leak-kinds=all ./bin/kernel kernel.config' Enter
    else
    tmux send-keys -t %2 './bin/kernel kernel.config' Enter
fi

while ! tmux capture-pane -p -t %2 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando KERNEL:
    echo $(tmux capture-pane -p -t %2)
    sleep 0.3
done ;

# 4. IO FS
rm ./kernel/entradasalida/main_logger.log
tmux send-keys -t %3 'cd entradasalida' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %3 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida FS.config FS" Enter
    else
    tmux send-keys -t %3 "./bin/entradasalida FS.config FS" Enter
fi

while ! tmux capture-pane -p -t %3 | grep "" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %3;
    echo $(tmux capture-pane -p -t %3)
    sleep 0.3
done ;

# 5. IO Stdin
rm ./kernel/entradasalida/main_logger.log
tmux send-keys -t %4 'cd entradasalida' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %4 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida TECLADO.config TECLADO" Enter
    else
    tmux send-keys -t %4 "./bin/entradasalida TECLADO.config TECLADO" Enter
fi

while ! tmux capture-pane -p -t %4 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %4;
    echo $(tmux capture-pane -p -t %4)
    sleep 0.3
done ;

# 6. IO Stdout
rm ./kernel/entradasalida/main_logger.log
tmux send-keys -t %5 'cd entradasalida' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %5 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida MONITOR.config MONITOR" Enter
    else
    tmux send-keys -t %5 "./bin/entradasalida MONITOR.config MONITOR" Enter
fi

while ! tmux capture-pane -p -t %5 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %5;
    echo $(tmux capture-pane -p -t %5)
    sleep 0.3
done ;

# terminator -e "tail -f ./kernel/logs/main_logger.log"
# terminator -e "tail -f ./cpu/logs/main_logger.log"
# terminator -e "tail -f ./memoria/logs/main_logger.log"

# ----------------------------- Arrancar el script -------------------------------
tmux send-keys -t %2 'INICIAR_PROCESO ../scripts_memoria/FS_1' Enter
sleep 0.3
tmux send-keys -t %2 'INICIAR_PROCESO ../scripts_memoria/FS_2' Enter

echo """----------
Esperamos a que TECLADO pida un input para el FS_1
---------- """
while ! tmux capture-pane -p -t %4 | grep "INGRESAR UNA CADENA" ; do
    sleep 0.3
done ;

tmux send-keys -t %4 'Fallout 1 Fallout 2 Fallout 3 Fallout: New Vegas Fallout 4 Fallout 76' Enter


echo """----------
Esperamos a que terminen todos los procesos y luego cerramos los modulos
---------- """

block_p_0=false
block_p_1=false
while ! $block_p_0 || ! $block_p_1 ; do

  if ! $block_p_0 && tmux capture-pane -p -t %2 -S -50 -J | grep "Finaliza el proceso <0> - Motivo: SUCCESS"; then
    block_p_0=true
    echo "PID: <0> - Finalizado"
  fi
  
  if ! $block_p_1 && tmux capture-pane -p -t %2 -S -50 -J | grep "Finaliza el proceso <1> - Motivo: SUCCESS"; then
    block_p_1=true
    echo "PID: <1> - Finalizado"
  fi

done

tmux send-keys -t %0 C-c
tmux send-keys -t %1 C-c
tmux send-keys -t %2 'EXIT' Enter
sleep 0.3
tmux send-keys -t %2 C-c
tmux send-keys -t %3 C-c
tmux send-keys -t %4 C-c
tmux send-keys -t %5 C-c

echo """----------
Ahora volvemos a iniciar los modulos
---------- """

# 1. Memoria
if [ $valgrind ]
    then
    tmux send-keys -t %0 'valgrind --leak-check=full --show-leak-kinds=all ./bin/memoria memoria.config' Enter
    else
    tmux send-keys -t %0 './bin/memoria memoria.config' Enter
fi

while ! tmux capture-pane -p -t %0 | grep "Escuchando en 127.0.0.1:" ; do
  # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
  echo Esperando MEMORIA:
  echo  $(tmux capture-pane -p -t %0)
  sleep 0.3
done ;

# 2. CPU
if [ $valgrind ]
    then
    tmux send-keys -t %1 'valgrind --leak-check=full --show-leak-kinds=all ./bin/cpu cpu.config' Enter
    else
    tmux send-keys -t %1 './bin/cpu cpu.config' Enter
fi

while ! tmux capture-pane -p -t %1 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando CPU:
    echo $(tmux capture-pane -p -t %1)
    sleep 0.3
done ;

# 3. Kernel
if [ $valgrind ]
    then
    tmux send-keys -t %2 'valgrind --leak-check=full --show-leak-kinds=all ./bin/kernel kernel.config' Enter
    else
    tmux send-keys -t %2 './bin/kernel kernel.config' Enter
fi

while ! tmux capture-pane -p -t %2 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando KERNEL:
    echo $(tmux capture-pane -p -t %2)
    sleep 0.3
done ;

# 4. IO FS
if [ $valgrind ]
    then
    tmux send-keys -t %3 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida FS.config FS" Enter
    else
    tmux send-keys -t %3 "./bin/entradasalida FS.config FS" Enter
fi

while ! tmux capture-pane -p -t %3 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %3;
    echo $(tmux capture-pane -p -t %3)
    sleep 0.3
done ;

# 5. IO Stdin

if [ $valgrind ]
    then
    tmux send-keys -t %4 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida TECLADO.config TECLADO" Enter
    else
    tmux send-keys -t %4 "./bin/entradasalida TECLADO.config TECLADO" Enter
fi

while ! tmux capture-pane -p -t %4 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %4;
    echo $(tmux capture-pane -p -t %4)
    sleep 0.3
done ;

# 6. IO Stdout
if [ $valgrind ]
    then
    tmux send-keys -t %5 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida MONITOR.config MONITOR" Enter
    else
    tmux send-keys -t %5 "./bin/entradasalida MONITOR.config MONITOR" Enter
fi

while ! tmux capture-pane -p -t %5 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %5;
    echo $(tmux capture-pane -p -t %5)
    sleep 0.3
done ;

echo """----------
Ahora corremos los programas FS_3 y FS_4
---------- """
tmux send-keys -t %2 'INICIAR_PROCESO ../scripts_memoria/FS_3' Enter
sleep 0.3
tmux send-keys -t %2 'INICIAR_PROCESO ../scripts_memoria/FS_4' Enter


# Attach to the created session
tmux attach-session -t my_session
