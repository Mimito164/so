#!/bin/bash

#------------------- Flags del script ----------------------
clean= false

while getopts :c flag
do
    case "${flag}" in
        c) clean=true;;
    esac
done
#----------------- END Flags del script -------------------

echo """
------------------------------------------------------------
| Test de Prueba 4 : IO                              |
------------------------------------------------------------

Actividades
1 Iniciar los módulos.

2 Ejecutar los siguientes comandos en la consola del Kernel:
- EJECUTAR_SCRIPT /scripts_kernel/PRUEBA_IO

3 Esperar que los procesos IO_A e IO_C estén bloqueados esperando a la interfaz TECLADO.

4 Ingresar para IO_A el texto:  “WAR NEVER CHANGES...” (sin las comillas)

5 Ingresar para IO_C el texto: “Sistemas Operativos 2c2023” (sin las comillas)

6 Esperar a que finalicen 3 procesos.

Resultados Esperados
- El proceso IO_A debería imprimir la frase: “WAR, WAR NEVER CHANGES…”
- El proceso IO_B debería imprimir la frase: “I don't want to set the world on fire”
- El proceso IO_C debería imprimir la frase: “Sistemas Operativos 1c2024”


------------------------------------------------------------
| GL HF Preciones una tecla para continuar                 |
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

# ALGORITMO_PLANIFICACION=FIFO
# QUANTUM=750
# RECURSOS=[REC1]
# INSTANCIAS_RECURSOS=[1]
# GRADO_MULTIPROGRAMACION=10

attribute="ALGORITMO_PLANIFICACION"   
att_ALGORITMO_PLANIFICACION="RR"
attribute2="QUANTUM"   
att_QUANTUM="750"
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
#---------------------- GENERICA     ----------------------------

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

tmux split-window -v # Split window horizontally
tmux select-pane -t 1 # Switch al primer pane
tmux split-window -v  # Split the second pane vertically
tmux select-pane -t 2 # Switch al segundo pane (que quedó en la derecha)
tmux split-window -v # Split the second pane vertically
tmux select-pane -t 3 # Switch al segundo pane (que quedó en la derecha)
tmux split-window -v # Split the second pane vertically
tmux select-pane -t 4 # Switch al segundo pane (que quedó en la derecha)
tmux split-window -v # Split the second pane vertically
tmux select-pane -t 5 # Switch al segundo pane (que quedó en la derecha)
tmux split-window -v # Split the second pane vertically
tmux select-layout even-vertical

# 1. Memoria
rm ./memoria/logs/main_logger.log
tmux send-keys -t 0 'cd memoria' Enter
tmux send-keys -t 0 'valgrind --leak-check=full ./bin/memoria memoria.config' Enter
while ! tmux capture-pane -p -t 0 | grep "Escuchando en 127.0.0.1:" ; do
  # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
  echo Esperando MEMORIA:
  echo  $(tmux capture-pane -p -t 0)
  sleep 0.3
done ;

# 2. CPU
rm ./cpu/logs/main_logger.log
tmux send-keys -t 1 'cd cpu' Enter
tmux send-keys -t 1 'valgrind --leak-check=full ./bin/cpu cpu.config' Enter
while ! tmux capture-pane -p -t 1 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando CPU:
    echo $(tmux capture-pane -p -t 1)
    sleep 0.3
done ;

# 3. Kernel
rm ./kernel/logs/main_logger.log
tmux send-keys -t 2 'cd kernel' Enter
tmux send-keys -t 2 'valgrind --leak-check=full ./bin/kernel kernel.config' Enter
while ! tmux capture-pane -p -t 2 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando KERNEL:
    echo $(tmux capture-pane -p -t 2)
    sleep 0.3
done ;

# 4. IO Gen
rm ./kernel/entradasalida/main_logger.log
tmux send-keys -t 3 'cd entradasalida' Enter
tmux send-keys -t 3 "valgrind --leak-check=full ./bin/entradasalida GENERICA.config GENERICA" Enter
while ! tmux capture-pane -p -t 3 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel 3;
    echo $(tmux capture-pane -p -t 3)
    sleep 0.3
done ;

# 5. IO Stdin
rm ./kernel/entradasalida/main_logger.log
tmux send-keys -t 4 'cd entradasalida' Enter
tmux send-keys -t 4 "valgrind --leak-check=full ./bin/entradasalida TECLADO.config TECLADO" Enter
while ! tmux capture-pane -p -t 4 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel 4;
    echo $(tmux capture-pane -p -t 4)
    sleep 0.3
done ;

# 4. IO Stdout
rm ./kernel/entradasalida/main_logger.log
tmux send-keys -t 5 'cd entradasalida' Enter
tmux send-keys -t 5 "valgrind --leak-check=full ./bin/entradasalida MONITOR.config MONITOR" Enter
while ! tmux capture-pane -p -t 5 | grep "Se envio la presentacion a K" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando IO panel 5;
    echo $(tmux capture-pane -p -t 5)
    sleep 0.3
done ;

# ----------------------------- Arrancar el script -------------------------------
tmux send-keys -t 2 'EJECUTAR_SCRIPT ../scripts_kernel/PRUEBA_IO' Enter

echo """----------
Esperamos que PID 0, PID 2 sean bloqueados
---------- """
block_p_0=false
block_p_1=false
block_p_2=false
while ! $block_p_0 || ! $block_p_1 || ! $block_p_2; do

  if ! $block_p_0 && tmux capture-pane -p -t 2 | grep "PID: <0> - Bloqueado"; then
    block_p_0=true
  fi
  
  if ! $block_p_2 && tmux capture-pane -p -t 2 | grep "PID: <2> - Bloqueado"; then
    block_p_2=true
  fi

  sleep 0.1
done

# esperamos para que levante las ios por las duds. TODO: revisar si podemos evitar este sleep
sleep 1

# --------------- Test: Matar algúno de los procesos --------------------
echo """----------
Escribimos en la IO TECLADO WAR, WAR NEVER CHANGES...
---------- """
tmux send-keys -t 4 'WAR, WAR NEVER CHANGES...' Enter



# --------------- Test: Matar algúno de los procesos --------------------
echo """----------
Matamos al PID 2 para resolver el deadlock
---------- """
tmux send-keys -t 2 'FINALIZAR_PROCESO 3' Enter

# Attach to the created session
tmux attach-session -t my_session

# ACAAAAAAAAAAAAAAAAAAA ---------------------ACAAAAAAAAAAAAAAAAAAAAAAAAAAaa 