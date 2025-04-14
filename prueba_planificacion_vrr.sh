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
| Test de Prueba 1 : PLANIFICACION                         |
------------------------------------------------------------

1- Iniciar los módulos.
2- Ejecutar los siguientes comandos en la consola del Kernel
  a- EJECUTAR_SCRIPT /scripts_kernel/PRUEBA_PLANI
3- Esperar a que empiece a ejecutar PLANI_4 y ejecutar FINALIZAR_PROCESO con el PID de PLANI_4.
4- Esperar que finaliceh los demás procesos y cortar la prueba.
5- Cambiar el algoritmo de planificación a RR y volver a ejecutar en la consola del Kernel
  a- EJECUTAR_SCRIPT /scripts_kernel/PRUEBA_PLANI
8- Esperar a que finalicen 3 procesos y ejecutar FINALIZAR_PROCESO con el PID de PLANI_4.
9- Finalizar la prueba, cambiar el algoritmo de planificación a VRR y volver a ejecutar.
  a- EJECUTAR_SCRIPT /scripts_kernel/PRUEBA_PLANI
11- Esperar a que finalicen 3 procesos y finalizar la prueba.

------------------------------------------------------------
| Resultados Esperados                                     |
------------------------------------------------------------

1- 3 de los 4 procesos finalizan sin problemas
2- En FIFO Para que puedan volver a ejecutar PLANI_2 y PLANI_3 hay que matar el proceso PLANI_4
3- En RR finalizan PLANI_1, luego PLANI_3 (el cual es desalojado 2 veces por fin de quantum) y por último PLANI_2. PLANI_4 continúa ejecutando. 
4- En VRR finalizan en el mismo orden que RR, pero PLANI_3 es desalojado 3 veces por fin de quantum.

------------------------------------------------------------
| GL HF Preciones una tecla para continuar                 |
------------------------------------------------------------
"""

read -n1 -s -r key


file="./memoria/memoria.config"
file2="./cpu/cpu.config"
file3="./Kernel/Kernel.config"  
file4="./entradasalida/entraadasalida.config"


#--------------------- Config Memoria ------------------------------

# TAM_MEMORIA=1024        
# TAM_PAGINA=32           
# RETARDO_RESPUESTA=1000  

file="./memoria/memoria.config"

attribute="TAM_MEMORIA"
att_TAM_MEMORIA="1024"

attribute2="TAM_PAGINA"
att_TAM_PAGINA="32"

attribute3="RETARDO_RESPUESTA"
att_RETARDO_RESPUESTA="1000"

sed -i "s/^$attribute=.*/$attribute=$att_TAM_MEMORIA/" "$file"
sed -i "s/^$attribute2=.*/$attribute2=$att_TAM_PAGINA/" "$file"
sed -i "s/^$attribute3=.*/$attribute3=$att_RETARDO_RESPUESTA/" "$file"

#--------------------- Config CPU ---------------------------------

# CANTIDAD_ENTRADAS_TLB=32
# ALGORITMO_TLB=FIFO

file2="./cpu/cpu.config"

attribute="CANTIDAD_ENTRADAS_TLB"    
att_CANTIDAD_ENTRADA_TLB="32"
attribute2="ALGORITMO_TLB"
att_ALGORITMO_TLB="FIFO"

sed -i "s/^$attribute=.*/$attribute=$att_CANTIDAD_ENTRADA_TLB/" "$file2"
sed -i "s/^$attribute2=.*/$attribute2=$att_ALGORITMO_TLB/" "$file2"


#---------------------- Config Kenel -------------------------

file3="./kernel/kernel.config" 

# ALGORITMO_PLANIFICACION=VRR
# QUANTUM=2750
# RECURSOS=[RECURSO]
# INSTANCIAS_RECURSOS=[1]
# GRADO_MULTIPROGRAMACION=10

attribute="ALGORITMO_PLANIFICACION"   
att_ALGORITMO_PLANIFICACION="VRR"
attribute2="QUANTUM"   
att_QUANTUM="2750"
attribute3="RECURSOS"   
att_RECURSOS="[RECURSO]"
attribute4="INSTANCIAS_RECURSOS"   
att_INSTANCIAS_RECURSOS="[1]"
attribute5="GRADO_MULTIPROGRAMACION"   
att_GRADO_MULTIPROGRAMACION="10"

sed -i "s/^$attribute=.*/$attribute=$att_ALGORITMO_PLANIFICACION/" "$file3"
sed -i "s/^$attribute2=.*/$attribute2=$att_QUANTUM/" "$file3"
sed -i "s/^$attribute3=.*/$attribute3=$att_RECURSOS/" "$file3"
sed -i "s/^$attribute4=.*/$attribute4=$att_INSTANCIAS_RECURSOS/" "$file3"
sed -i "s/^$attribute5=.*/$attribute5=$att_GRADO_MULTIPROGRAMACION/" "$file3"



#---------------- Config Entrada Saluda  ------------------------
#---------------------- SLP1 ------------------------------------

file4="./entradasalida/entradasalida.config"

# TIPO_INTERFAZ=GENERICA
# TIEMPO_UNIDAD_TRABAJO=50


attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="GENERICA"
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

tmux split-window -h # Split window horizontally
tmux select-pane -t 0 # Switch al primer pane
tmux split-window -v  # Split the second pane vertically

tmux select-pane -t 2 # Switch al segundo pane (que quedó en la derecha)
tmux split-window -v # Split the second pane vertically


# 1. Memoria
rm ./memoria/logs/main_logger.log
tmux send-keys -t 0 'cd memoria' Enter

if [ $valgrind ]
    then
    tmux send-keys -t 0 'valgrind --leak-check=full --show-leak-kinds=all ./bin/memoria memoria.config' Enter
    else
    tmux send-keys -t 0 './bin/memoria memoria.config' Enter
fi

while ! tmux capture-pane -p -t 0 | grep "Escuchando en 127.0.0.1:" ; do
  # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
  echo Esperando MEMORIA:
  echo  $(tmux capture-pane -p -t 0)
  sleep 0.3
done ;

# 2. CPU
rm ./cpu/logs/main_logger.log
tmux send-keys -t 1 'cd cpu' Enter

if [ $valgrind ]
    then
    tmux send-keys -t 1 'valgrind --leak-check=full --show-leak-kinds=all ./bin/cpu cpu.config' Enter
    else
    tmux send-keys -t 1 './bin/cpu cpu.config' Enter
fi

while ! tmux capture-pane -p -t 1 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando CPU:
    echo $(tmux capture-pane -p -t 1)
    sleep 0.3
done ;

# 3. Kernel
rm ./kernel/logs/main_logger.log
tmux send-keys -t 2 'cd kernel' Enter

if [ $valgrind ]
    then
    tmux send-keys -t 2 'valgrind --leak-check=full --show-leak-kinds=all ./bin/kernel kernel.config' Enter
    else
    tmux send-keys -t 2 './bin/kernel kernel.config' Enter
fi

while ! tmux capture-pane -p -t 2 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando KERNEL:
    echo $(tmux capture-pane -p -t 2)
    sleep 0.3
done ;

# 4. IO
rm ./kernel/entradasalida/main_logger.log
tmux send-keys -t 3 'cd entradasalida' Enter

if [ $valgrind ]
    then
    tmux send-keys -t 3 'valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida entradasalida.config SLP1' Enter
    else
    tmux send-keys -t 3 './bin/entradasalida entradasalida.config SLP1' Enter
fi

while ! tmux capture-pane -p -t 3 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel 3;
    echo $(tmux capture-pane -p -t 3)
    sleep 0.3
done ;

# ----------------------------- Arrancar el script -------------------------------
tmux send-keys -t 2 'EJECUTAR_SCRIPT ../scripts_kernel/PRUEBA_PLANI' Enter

# --------------- Test: Esperar que finalicen los demás procesos --------------------
echo """----------
Esperamos a que termine PID 0 y PID 2 (PID 1 y 3 siguen ejecutando)
---------- """
fin_p_0=false
fin_p_2=false
while ! $fin_p_0 || ! $fin_p_2 ; do

  if ! $fin_p_0 && tmux capture-pane -p -t 2 -S -50 | grep "Finaliza el proceso <0>"; then
    fin_p_0=true
  fi
  
  if ! $fin_p_2 && tmux capture-pane -p -t 2 -S -50 | grep "Finaliza el proceso <2>"; then
    fin_p_2=true
  fi

done

# Attach to the created session
tmux attach-session -t my_session
