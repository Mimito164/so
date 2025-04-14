#!/bin/bash 

# Obtener IP 

system_IP=$(./obtain-ip.sh)

echo "$system_IP"

read -p "Escribi IP de Kernel: " ip_kernel
read -p "Escribi IP de Memoria: " ip_memoria
read -p "Escribi IP de CPU: " ip_cpu
read -p "Escribi IP de Entrada Salida: " ip_entrada_Salida

#---------------------------- Cambia la ip por la que es propia del sistema ---------------------

if [[ -z "$ip_kernel" ]];then 
  ip_kernel=$system_IP
fi 
if [[ -z "$ip_memoria" ]];then 
  ip_memoria=$system_IP
fi 
if [[ -z "$ip_cpu" ]];then 
  ip_cpu=$system_IP
fi 
if [[ -z "$ip_entrada_Salida" ]];then 
  ip_entrada_Salida=$system_IP
fi 

echo "IP-KERNEL: $ip_kernel"
echo "IP-MEMORIA: $ip_memoria"
echo "IP-CPU: $ip_cpu"
echo "IP-ENTRADASALIDA: $ip_entrada_Salida"

echo " --------------- Se editaron los config -------------------------"


attribute_ip_modulo="IP_MODULO"
attribute_ip_memoria="IP_MEMORIA"
attribute_ip_cpu="IP_CPU"
attribute_ip_kernel="IP_KERNEL"

for archivo in ./kernel/configs/*.config; do
  sed -i "s/^$attribute_ip_modulo=.*/$attribute_ip_modulo=$ip_kernel/" "$archivo"
  sed -i "s/^$attribute_ip_memoria=.*/$attribute_ip_memoria=$ip_memoria/" "$archivo"
  sed -i "s/^$attribute_ip_cpu=.*/$attribute_ip_cpu=$ip_cpu/" "$archivo"
done

for archivo in ./cpu/configs/*.config; do
  sed -i "s/^$attribute_ip_modulo=.*/$attribute_ip_modulo=$ip_cpu/" "$archivo"
  sed -i "s/^$attribute_ip_memoria=.*/$attribute_ip_memoria=$ip_memoria/" "$archivo"
done

for archivo in ./memoria/configs/*.config; do
  sed -i "s/^$attribute_ip_modulo=.*/$attribute_ip_modulo=$ip_memoria/" "$archivo"
done

for archivo in ./entradasalida/configs/*.config; do
  sed -i "s/^$attribute_ip_memoria=.*/$attribute_ip_memoria=$ip_memoria/" "$archivo"
  sed -i "s/^$attribute_ip_kernel=.*/$attribute_ip_kernel=$ip_kernel/" "$archivo"
done
