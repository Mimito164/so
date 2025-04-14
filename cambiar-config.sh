#!/bin/bash

#--------------------- Config Memoria ------------------------------

# TAM_MEMORIA=1024        
# TAM_PAGINA=32           
# RETARDO_RESPUESTA=1000  

file="./memoria/memoria.config"

attribute="TAM_MEMORIA"
att_TAM_MEMORIA="1024"

attribute2="TAM_PAGINA"
att_TAM_PAGINA="4"

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

# ALGORITMO_PLANIFICACION=FIFO
# QUANTUM=2750
# RECURSOS=[RECURSO]
# INSTANCIAS_RECURSOS=[1]
# GRADO_MULTIPROGRAMACION=10

attribute="ALGORITMO_PLANIFICACION"   
att_ALGORITMO_PLANIFICACION="RR"
attribute2="QUANTUM"   
att_QUANTUM="750"
attribute3="RECURSOS"   
att_RECURSOS="[RA,RB,RC]"
attribute4="INSTANCIAS_RECURSOS"   
att_INSTANCIAS_RECURSOS="[1,1,1]"
attribute5="GRADO_MULTIPROGRAMACION"   
att_GRADO_MULTIPROGRAMACION="10"

sed -i "s/^$attribute=.*/$attribute=$att_ALGORITMO_PLANIFICACION/" "$file3"
sed -i "s/^$attribute2=.*/$attribute2=$att_QUANTUM/" "$file3"
sed -i "s/^$attribute3=.*/$attribute3=$att_RECURSOS/" "$file3"
sed -i "s/^$attribute4=.*/$attribute4=$att_INSTANCIAS_RECURSOS/" "$file3"
sed -i "s/^$attribute5=.*/$attribute5=$att_GRADO_MULTIPROGRAMACION/" "$file3"

