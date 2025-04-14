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
| Test de Prueba 2 : Memoria y TLB                         |
------------------------------------------------------------

Actividades

1. Iniciar los módulos.

2. Ejecutar los siguientes comandos en la consola del Kernel
  a. INICIAR_PROCESO /scripts_memoria/MEMORIA_1

3. Esperar a que finalice el proceso.

4. Finalizar todos los módulos, cambiar el algoritmo de reemplazo de TLB por LRU y volver a iniciar todo.

5. Ejecutar los siguientes comandos en la consola del Kernel
  a. INICIAR_PROCESO /scripts_memoria/MEMORIA_1

6. Esperar a que finalice el proceso.

7. Ejecutar los siguientes comandos en la consola del Kernel
  a. INICIAR_PROCESO /scripts_memoria/MEMORIA_2

8. Esperar a que finalice el proceso.

9. Ejecutar los siguientes comandos en la consola del Kernel
  a. INICIAR_PROCESO /scripts_memoria/MEMORIA_3

10. Esperar a que finalice el proceso.


Resultados Esperados
  
  - En las ejecuciones del proceso MEMORIA_1 se observan diferencias en los reemplazos de la TLB.
  
  - En la ejecución del proceso MEMORIA_2 se observa que puede recuperar correctamente el valor de la memoria que se encuentra en 2 páginas diferentes.
  
  - En la ejecución del proceso MEMORIA_3 el mismo debería finalizar por error de Out of Memory.


------------------------------------------------------------
| GL HF  Presiona una tecla para arrancar                  |
------------------------------------------------------------
"""

read -n1 -s -r key


#--------------------- Memoria ------------------------------

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

#--------------------- CPU ---------------------------------

# CANTIDAD_ENTRADAS_TLB=4
# ALGORITMO_TLB=FIFO

file2="./cpu/cpu.config"

attribute="CANTIDAD_ENTRADAS_TLB"    
att_CANTIDAD_ENTRADA_TLB="4"
attribute2="ALGORITMO_TLB"
att_ALGORITMO_TLB="FIFO"

sed -i "s/^$attribute=.*/$attribute=$att_CANTIDAD_ENTRADA_TLB/" "$file2"
sed -i "s/^$attribute2=.*/$attribute2=$att_ALGORITMO_TLB/" "$file2"


#---------------------- Kenel -------------------------

file3="./kernel/kernel.config" 

# ALGORITMO_PLANIFICACION=FIFO
# QUANTUM=2000
# RECURSOS=[RECURSO]
# INSTANCIAS_RECURSOS=[1]
# GRADO_MULTIPROGRAMACION=10

attribute="ALGORITMO_PLANIFICACION"   
att_ALGORITMO_PLANIFICACION="FIFO"
attribute2="QUANTUM"   
att_QUANTUM="2000"
attribute3="RECURSOS"   
att_RECURSOS="[RECURSOS]"
attribute4="INSTANCIAS_RECURSOS"   
att_INSTANCIAS_RECURSOS="[1]"
attribute5="GRADO_MULTIPROGRAMACION"   
att_GRADO_MULTIPROGRAMACION="10"

sed -i "s/^$attribute=.*/$attribute=$att_ALGORITMO_PLANIFICACION/" "$file3"
sed -i "s/^$attribute2=.*/$attribute2=$att_QUANTUM/" "$file3"
sed -i "s/^$attribute3=.*/$attribute3=$att_RECURSOS/" "$file3"
sed -i "s/^$attribute4=.*/$attribute4=$att_INSTANCIAS_RECURSOS/" "$file3"
sed -i "s/^$attribute5=.*/$attribute5=$att_GRADO_MULTIPROGRAMACION/" "$file3"

#---------------------- Entrada Saluda  -------------------------
#---------------------- IO_GEN_SLEEP ----------------------------

file4="./entradasalida/entradasalida.config"

# TIPO_INTERFAZ=GENERICA
# TIEMPO_UNIDAD_TRABAJO=250


attribute="TIPO_INTERFAZ"   
att_TIPO_INTERFAZ="GENERICA"
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

while ! tmux capture-pane -p -t 0 | grep "Escuchando en" ; do
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

while ! tmux capture-pane -p -t 1 | grep "Escuchando en" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando CPU:
    echo $(tmux capture-pane -p -t 1)
    sleep 0.3
done ;
terminator -e "tail -f ./cpu/logs/main_logger.log" &&

# 3. Kernel
rm ./kernel/logs/main_logger.log
tmux send-keys -t 2 'cd kernel' Enter

if [ $valgrind ]
  then
  tmux send-keys -t 2 'valgrind --leak-check=full --show-leak-kinds=all ./bin/kernel kernel.config' Enter
  else
  tmux send-keys -t 2 './bin/kernel kernel.config' Enter
fi

while ! tmux capture-pane -p -t 2 | grep "Escuchando en:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando KERNEL:
    echo $(tmux capture-pane -p -t 2)
    sleep 0.3
done ;
terminator -e "tail -f ./kernel/logs/main_logger.log" &&


# 4. IO
rm ./entradasalida/logs/main_logger.log
tmux send-keys -t 3 'cd entradasalida' Enter

if [ $valgrind ]
  then
  tmux send-keys -t 3 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida entradasalida.config ESPERA" Enter
  else
  tmux send-keys -t 3 "./bin/entradasalida entradasalida.config ESPERA" Enter
fi

while ! tmux capture-pane -p -t 3 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel 3;
    echo $(tmux capture-pane -p -t 3)
    sleep 0.3
done ;

# ----------------------------- Arrancar el script -------------------------------
tmux send-keys -t 2 'INICIAR_PROCESO ../scripts_memoria/MEMORIA_1' Enter


echo """----------
Esperamos a que finalize el PID 0 ( MEMORIA_1 )
---------- """

fin_p_0=false
while ! $fin_p_0; do

  if ! $fin_p_0 && tmux capture-pane -p -t 2 -S -50 -J | grep "Finaliza el proceso <0>"; then
    fin_p_0=true
  fi  
  sleep 0.3
done

echo """----------
Matamos todos los modulos
---------- """


tmux send-keys -t 3 C-c # Matamos IO
tmux send-keys -t 2 'EXIT' Enter # Matamos la consola de Kernel
sleep 1
tmux send-keys -t 2 C-c # Matamos Kernel
tmux send-keys -t 1 C-c
tmux send-keys -t 0 C-c


echo """----------
Cambiamos el algoritmo de reemplazo de la TLB por LRU
---------- """

file2="./cpu/cpu.config"

# ALGORITMO_TLB=LRU
attribute="ALGORITMO_TLB"
att_ALGORITMO_TLB="LRU"

sed -i "s/^$attribute=.*/$attribute=$att_ALGORITMO_TLB/" "$file2"


echo """----------
Volvemos a iniciar todo
---------- """

# 1. Memoria
if [ $valgrind ]
  then
  tmux send-keys -t 0 'valgrind --leak-check=full --show-leak-kinds=all ./bin/memoria memoria.config' Enter
  else
  tmux send-keys -t 0 './bin/memoria memoria.config' Enter
fi

while ! tmux capture-pane -p -t 0 | grep "Escuchando en" ; do
  # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
  echo Esperando MEMORIA:
  echo  $(tmux capture-pane -p -t 0)
  sleep 0.3
done ;

# 2. CPU
if [ $valgrind ]
  then
  tmux send-keys -t 1 'valgrind --leak-check=full --show-leak-kinds=all ./bin/cpu cpu.config' Enter
  else
  tmux send-keys -t 1 './bin/cpu cpu.config' Enter
fi

while ! tmux capture-pane -p -t 1 | grep "Escuchando en" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando CPU:
    echo $(tmux capture-pane -p -t 1)
    sleep 0.3
done ;

# 3. Kernel
if [ $valgrind ]
  then
  tmux send-keys -t 2 'valgrind --leak-check=full --show-leak-kinds=all ./bin/kernel kernel.config' Enter
  else
  tmux send-keys -t 2 './bin/kernel kernel.config' Enter
fi


while ! tmux capture-pane -p -t 2 | grep "Escuchando en:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando KERNEL:
    echo $(tmux capture-pane -p -t 2)
    sleep 0.3
done ;

# 4. IO
if [ $valgrind ]
  then
  tmux send-keys -t 3 "valgrind --leak-check=full --show-leak-kinds=all ./bin/entradasalida entradasalida.config ESPERA" Enter
  else
  tmux send-keys -t 3 "./bin/entradasalida entradasalida.config ESPERA" Enter
fi

while ! tmux capture-pane -p -t 3 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel 3;
    echo $(tmux capture-pane -p -t 3)
    sleep 0.3
done ;


echo """----------
Ejecutamos MEMORIA_1
---------- """

tmux send-keys -t 2 'INICIAR_PROCESO ../scripts_memoria/MEMORIA_1' Enter


echo """----------
Esperar a que finalice el proceso.
---------- """
fin_p_0=false
while ! $fin_p_0; do

  if ! $fin_p_0 && tmux capture-pane -p -t 2 -S -50 | grep "Finaliza el proceso <0>"; then
    fin_p_0=true
  fi

done


echo """----------
Ejecutamos MEMORIA_2
---------- """

tmux send-keys -t 2 'INICIAR_PROCESO ../scripts_memoria/MEMORIA_2' Enter

echo """----------
Esperar a que finalice el proceso.
---------- """
fin_p_0=false
while ! $fin_p_0; do

  if ! $fin_p_0 && tmux capture-pane -p -t 2 -S -50  | grep "Finaliza el proceso <1>"; then
    fin_p_0=true
  fi

done


echo """----------
Ejecutamos MEMORIA_3
---------- """

tmux send-keys -t 2 'INICIAR_PROCESO ../scripts_memoria/MEMORIA_3' Enter

echo """----------
Esperar a que finalice el proceso.
---------- """
fin_p_0=false
while ! $fin_p_0; do

  if ! $fin_p_0 && tmux capture-pane -p -t 2 -S -50 | grep "Finaliza el proceso <2>"; then
    fin_p_0=true
  fi

done


# Attach to the created session
tmux attach-session -t my_session
