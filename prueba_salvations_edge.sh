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
| Test de Prueba 6 : Salvations Edge                       |
------------------------------------------------------------
Actividades
1. Iniciar los módulos.
2. Ejecutar los siguientes comandos en la consola del Kernel
    - EJECUTAR_SCRIPT /scripts_kernel/PRUEBA_SALVATIONS_EDGE
3. Esperar que empiecen a ejecutar los 2 primeros scripts.
4. Ejecutar en la consola del Kernel el comando: MULTIPROGRAMACION 100
5. Esperar a que finalicen todos los procesos salvo aquellos que están en un loop infinito.
Aclaraciones
    - Ir ejecutando los comandos que el ayudante evaluador indique.
    - A intervalos regulares ejecutar los siguientes comandos en la consola del Kernel
        -   DETENER_PLANIFICACION
        -   PROCESO_ESTADO
        -   INICIAR_PLANIFICACION
Resultados Esperados
    - No se observan esperas activas y/o memory leaks
------------------------------------------------------------
| GL HF Preciona una tecla para continuar                  |
------------------------------------------------------------
"""

read -n1 -s -r key

#--------------------- Memoria ------------------------------

# TAM_MEMORIA=4096
# TAM_PAGINA=32
# RETARDO_RESPUESTA=100

file="./memoria/memoria.config"

attribute="TAM_MEMORIA"    
att_TAM_MEMORIA="4096"
attribute2="TAM_PAGINA"
att_TAM_PAGINA="32"
attribute3="RETARDO_RESPUESTA"
att_RETARDO_RESPUESTA="100"

sed -i "s/^$attribute=.*/$attribute=$att_TAM_MEMORIA/" "$file"
sed -i "s/^$attribute2=.*/$attribute2=$att_TAM_PAGINA/" "$file"
sed -i "s/^$attribute3=.*/$attribute3=$att_RETARDO_RESPUESTA/" "$file"

#--------------------- CPU ---------------------------------

# CANTIDAD_ENTRADAS_TLB=16
# ALGORITMO_TLB=LRU

file2="./cpu/cpu.config"

attribute="CANTIDAD_ENTRADAS_TLB"    
att_CANTIDAD_ENTRADA_TLB="16"
attribute2="ALGORITMO_TLB"
att_ALGORITMO_TLB="LRU"

sed -i "s/^$attribute=.*/$attribute=$att_CANTIDAD_ENTRADA_TLB/" "$file2"
sed -i "s/^$attribute2=.*/$attribute2=$att_ALGORITMO_TLB/" "$file2"


#---------------------- Kenel -------------------------

file3="./kernel/kernel.config" 

# ALGORITMO_PLANIFICACION=VRR
# QUANTUM=500
# RECURSOS=[RA, RB, RC, RD]
# INSTANCIAS_RECURSOS=[1,1,1,1]
# GRADO_MULTIPROGRAMACION=3

attribute="ALGORITMO_PLANIFICACION" 
att_ALGORITMO_PLANIFICACION="VRR"
attribute2="QUANTUM"   
att_QUANTUM="500"
attribute3="RECURSOS"
att_RECURSOS="[RA, RB, RC, RD]"
attribute4="INSTANCIAS_RECURSOS"   
att_INSTANCIAS_RECURSOS="[1,1,1,1]"
attribute5="GRADO_MULTIPROGRAMACION"   
att_GRADO_MULTIPROGRAMACION="3"

sed -i "s/^$attribute=.*/$attribute=$att_ALGORITMO_PLANIFICACION/" "$file3"
sed -i "s/^$attribute2=.*/$attribute2=$att_QUANTUM/" "$file3"
sed -i "s/^$attribute3=.*/$attribute3=$att_RECURSOS/" "$file3"
sed -i "s/^$attribute4=.*/$attribute4=$att_INSTANCIAS_RECURSOS/" "$file3"
sed -i "s/^$attribute5=.*/$attribute5=$att_GRADO_MULTIPROGRAMACION/" "$file3"

#---------------------- Entrada Saluda 0 -------------------------
#---------------------- GENERICA         -------------------------

file4="./entradasalida/GENERICA.config"

# TIPO_INTERFAZ=GENERICA
# TIEMPO_UNIDAD_TRABAJO=250

attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="GENERICA"
attribute2="TIEMPO_UNIDAD_TRABAJO"   
att_TIEMPO_UNIDAD_TRABAJO="250"

sed -i "s/^$attribute=.*/$attribute=$att_TIPO_INTERFAZ/" "$file4"
sed -i "s/^$attribute2=.*/$attribute2=$att_TIEMPO_UNIDAD_TRABAJO/" "$file4"

#---------------------- Entrada Saluda 1 -------------------------
#---------------------- SLP1         -------------------------

file4="./entradasalida/SLP1.config"

# TIPO_INTERFAZ=GENERICA
# TIEMPO_UNIDAD_TRABAJO=50

attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="GENERICA"
attribute2="TIEMPO_UNIDAD_TRABAJO"   
att_TIEMPO_UNIDAD_TRABAJO="50"

sed -i "s/^$attribute=.*/$attribute=$att_TIPO_INTERFAZ/" "$file4"
sed -i "s/^$attribute2=.*/$attribute2=$att_TIEMPO_UNIDAD_TRABAJO/" "$file4"

#---------------------- Entrada Saluda 2 -------------------------
#---------------------- Espera           -------------------------

file4="./entradasalida/SLP1.config"

# TIPO_INTERFAZ=GENERICA
# TIEMPO_UNIDAD_TRABAJO=500

attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="GENERICA"
attribute2="TIEMPO_UNIDAD_TRABAJO"   
att_TIEMPO_UNIDAD_TRABAJO="500"

sed -i "s/^$attribute=.*/$attribute=$att_TIPO_INTERFAZ/" "$file4"
sed -i "s/^$attribute2=.*/$attribute2=$att_TIEMPO_UNIDAD_TRABAJO/" "$file4"



#---------------------- Entrada Saluda 3 -------------------------
#---------------------- TECLADO       ----------------------------

file4="./entradasalida/TECLADO.config"

# TIPO_INTERFAZ=STDIN
# TIEMPO_UNIDAD_TRABAJO=50

attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="STDIN"
attribute2="TIEMPO_UNIDAD_TRABAJO"   
att_TIEMPO_UNIDAD_TRABAJO="50"

sed -i "s/^$attribute=.*/$attribute=$att_TIPO_INTERFAZ/" "$file4"
sed -i "s/^$attribute2=.*/$attribute2=$att_TIEMPO_UNIDAD_TRABAJO/" "$file4"

#---------------------- Entrada Saluda 4 -------------------------
#---------------------- MONITOR       ----------------------------

file4="./entradasalida/MONITOR.config"

# TIPO_INTERFAZ=STDOUT
# TIEMPO_UNIDAD_TRABAJO=50

attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="STDOUT"
attribute2="TIEMPO_UNIDAD_TRABAJO"   
att_TIEMPO_UNIDAD_TRABAJO="50"

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

tmux new-window # Create a new tmux window → Ahora tenemos panel %0 %1 %2 %3
tmux split-window -v # Split window horizontally → Ahora tenemos panel %0 %1 %2 %3 %4
tmux select-layout even-vertical

tmux select-window -t 0 # Switch a la primer window


# 0. Memoria
rm ./memoria/logs/main_logger.log
tmux send-keys -t %0 'cd memoria' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %0 'valgrind --leak-check=full --show-leak-kinds=all ./bin/memoria memoria.config' Enter
    else
    tmux send-keys -t %0 './bin/memoria memoria.config' Enter
fi

while ! tmux capture-pane -p -t %0 | grep "Escuchando en" ; do
  # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
  echo Esperando MEMORIA:
  echo  $(tmux capture-pane -p -t %0)
done ;
#terminator -e "tail -f ./memoria/logs/main_logger.log"

# 1. CPU
rm ./cpu/logs/main_logger.log
tmux send-keys -t %1 'cd cpu' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %1 'valgrind --leak-check=full --show-leak-kinds=all ./bin/cpu cpu.config' Enter
    else
    tmux send-keys -t %1 './bin/cpu cpu.config' Enter
fi

while ! tmux capture-pane -p -t %1 | grep "Escuchando en " ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando CPU:
    echo $(tmux capture-pane -p -t %1)
done ;
#terminator -e "tail -f ./cpu/logs/main_logger.log"

# 2. Kernel
rm ./kernel/logs/main_logger.log
tmux send-keys -t %2 'cd kernel' Enter


if [ $valgrind ]
    then
    tmux send-keys -t %2 'valgrind --leak-check=full --show-leak-kinds=all ./bin/kernel kernel.config' Enter
    else
    tmux send-keys -t %2 './bin/kernel kernel.config' Enter
fi

while ! tmux capture-pane -p -t %2 | grep "Escuchando en " ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando KERNEL:
    echo $(tmux capture-pane -p -t %2)
done ;
#terminator -e "tail -f ./kernel/logs/main_logger.log"

# 3. IO 0 GENERICA
rm ./entradasalida/logs/main_logger.log
tmux send-keys -t %3 'cd entradasalida' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %3 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida GENERICA.config GENERICA" Enter
    else
    tmux send-keys -t %3 "./bin/entradasalida FS.config FS" Enter
fi

while ! tmux capture-pane -p -t %3 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %3;
    echo $(tmux capture-pane -p -t %3)
done ;

# 4. IO 1 SLP1
tmux send-keys -t %4 'cd entradasalida' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %4 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida SLP1.config SLP1" Enter
    else
    tmux send-keys -t %4 "./bin/entradasalida SLP1.config SLP1" Enter
fi

while ! tmux capture-pane -p -t %4 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %4;
    echo $(tmux capture-pane -p -t %4)
done ;

# 5. IO 1 ESPERA

tmux send-keys -t %5 'cd entradasalida' Enter
tmux send-keys -t %5 "valgrind --leak-check=full " Enter

if [ $valgrind ]
    then
    tmux send-keys -t %5 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida ESPERA.config ESPERA" Enter
    else
    tmux send-keys -t %5 "./bin/entradasalida ESPERA.config ESPERA" Enter
fi

while ! tmux capture-pane -p -t %5 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %5;
    echo $(tmux capture-pane -p -t %5)
done ;


# 6. IO Stdin TECLADO
tmux send-keys -t %6 'cd entradasalida' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %6 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida TECLADO.config TECLADO" Enter
    else
    tmux send-keys -t %6 "./bin/entradasalida TECLADO.config TECLADO" Enter
fi

while ! tmux capture-pane -p -t %6 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %6;
    echo $(tmux capture-pane -p -t %6)
done ;

# 7. IO Stdout MONITOR
tmux send-keys -t %7 'cd entradasalida' Enter

if [ $valgrind ]
    then
    tmux send-keys -t %7 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida MONITOR.config MONITOR" Enter
    else
    tmux send-keys -t %7 "./bin/entradasalida MONITOR.config MONITOR" Enter
fi


while ! tmux capture-pane -p -t %7 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel %7;
    echo $(tmux capture-pane -p -t %7)
done ;

# ----------------------------- Arrancar el script -------------------------------
tmux send-keys -t %2 'EJECUTAR_SCRIPT ../scripts_kernel/PRUEBA_SALVATIONS_EDGE' Enter

# tmux send-keys -t %2 'INICIAR_PROCESO ../scripts_memoria/TRIANGLE' Enter
# tmux send-keys -t %2 'INICIAR_PROCESO ../scripts_memoria/SQUARE' Enter
# tmux attach-session -t my_session

echo """----------
Esperamos a que se ejecuten los primeros dos procesos 
para cambiar la multiprogramación
---------- """

block_p_0=false
block_p_1=false
while ! $block_p_0 || ! $block_p_1 ; do

  if ! $block_p_0 && tmux capture-pane -p -t %2 -S -50 -J | grep "PID: <1> - Estado Anterior: <READY> - Estado Actual: <EXEC>"; then
    block_p_0=true
    echo "PID: <0> - Estado Anterior: <READY> - Estado Actual: <EXEC>"
  fi

  if ! $block_p_1 && tmux capture-pane -p -t %2 -S -50 -J | grep "PID: <1> - Estado Anterior: <READY> - Estado Actual: <EXEC>"; then
    block_p_1=true
    echo "PID: <1> - Estado Anterior: <READY> - Estado Actual: <EXEC>"
  fi

done

tmux send-keys -t %2 'MULTIPROGRAMACION 100' Enter


# Attach to the created session
tmux attach-session -t my_session
