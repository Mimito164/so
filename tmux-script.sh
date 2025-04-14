#!/bin/bash

clean= false
io=Interfaz1

while getopts :ci: flag
do
    case "${flag}" in
        c) clean=true;;
        i) io=$OPTARG;;
    esac
done

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
tmux send-keys -t 0 'cd memoria' Enter
tmux send-keys -t 0 './bin/memoria memoria.config' Enter
while ! tmux capture-pane -p -t 0 | grep "Escuchando en 127.0.0.1:" ; do
  # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
  echo Esperando MEMORIA:
  echo  $(tmux capture-pane -p -t 0)
  sleep 0.3
done ;

# 2. CPU
tmux send-keys -t 1 'cd cpu' Enter
tmux send-keys -t 1 './bin/cpu cpu.config' Enter
while ! tmux capture-pane -p -t 1 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando CPU:
    echo $(tmux capture-pane -p -t 1)
    sleep 0.3
done ;

# 3. Kernel
tmux send-keys -t 2 'cd kernel' Enter
tmux send-keys -t 2 './bin/kernel kernel.config' Enter
while ! tmux capture-pane -p -t 2 | grep "Escuchando en 127.0.0.1:" ; do
    # Me quedo loopeando imprimiendo lo de abajo hasta que el grep consiga lo que quiere 
    echo Esperando KERNEL:
    echo $(tmux capture-pane -p -t 2)
    sleep 0.3
done ;

# 4. IO
tmux send-keys -t 3 'cd entradasalida' Enter
tmux send-keys -t 3 "./bin/entradasalida entradasalida.config $io" Enter



# Attach to the created session
tmux attach-session -t my_session
