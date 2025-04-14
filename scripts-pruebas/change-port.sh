#!/bin/bash 

file="./Memoria/Memoria.config"
file2="./CPU/CPU.config"
file3="./FileSystem/FileSystem.config"
file4="./Kernel/Kernel.config"

# result=$((num + 5))

new_value=$1
new_value2=$((new_value + 1))
new_value3=$((new_value2 + 3))
new_value4=$((new_value3 + 1))


#--------------------- Memoria ------------------------------

attribute="PUERTO_ESCUCHA"     #=8002 = PUERTO_MEMORIA       -> new_value
attribute2="PUERTO_FILESYSTEM" #=8003 = PUERTA_ESCUCHA--> FS -> new_value2

sed -i "s/^$attribute=.*/$attribute=$new_value/" "$file"
sed -i "s/^$attribute2=.*/$attribute2=$new_value2/" "$file"

#-------------------- FileSystem ---------------------------

attribute="PUERTO_MEMORIA"  # PUERTO_MEMORIA=8002
attribute2="PUERTO_ESCUCHA" # PUERTO_ESCUCHA=8003

sed -i "s/^$attribute=.*/$attribute=$new_value/" "$file3"
sed -i "s/^$attribute2=.*/$attribute2=$new_value2/" "$file3"

#-------------------- CPU -------------------------------
attribute2="PUERTO_FILESYSTEM"
attribute3="PUERTO_ESCUCHA_DISPATCH"
attribute4="PUERTO_ESCUCHA_INTERRUPT"

# PUERTO_MEMORIA=8002
# PUERTO_ESCUCHA_DISPATCH=8006
# PUERTO_ESCUCHA_INTERRUPT=8007

sed -i "s/^$attribute=.*/$attribute=$new_value/" "$file2"
sed -i "s/^$attribute2=.*/$attribute2=$new_value2/" "$file2"
sed -i "s/^$attribute3=.*/$attribute3=$new_value3/" "$file2"
sed -i "s/^$attribute4=.*/$attribute4=$new_value4/" "$file2"
#-------------------- KERNEL ----------------------------

# PUERTO_MEMORIA=8002    ---> Attribute 3 
# PUERTO_FILESYSTEM=8003 ---> Attribute 2 
# PUERTO_CPU_DISPATCH=8006
# PUERTO_CPU_INTERRUPT=8007

attribute5="PUERTO_CPU_DISPATCH"
attribute6="PUERTO_CPU_INTERRUPT"

sed -i "s/^$attribute=.*/$attribute=$new_value/" "$file4"
sed -i "s/^$attribute2=.*/$attribute2=$new_value2/" "$file4"
sed -i "s/^$attribute5=.*/$attribute5=$new_value3/" "$file4"
sed -i "s/^$attribute6=.*/$attribute6=$new_value4/" "$file4"

