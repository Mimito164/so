#!/bin/bash 


# ----------------------------- MEMORIA ------------------------

cd memoria 
make clean > /dev/null
make > /dev/null
var=$?
#echo "$var"
if [ $var -eq 0 ] 
then 
  echo -e "\033[1;32mMemoria compilo bien \033[m" 
else 
  echo -e "\033[1;31mRIP memoria\033[m" >&2 
fi
a=$var
var=$((var || a))
cd .. 

# ----------------------------- CPU  ------------------------
cd cpu 
make clean > /dev/null
make > /dev/null
if [ $? -eq 0 ] 
then 
  echo -e  "\033[1;32mCPU compilo bien \033[m" 
else 
  echo -e  "\033[1;31mRIP CPU\033[m" >&2 
fi
a=$var
var=$((var || a))
cd .. 

# ----------------------------- ENTRADA-SALIDA  ------------------------
cd entradasalida 
make clean > /dev/null
make > /dev/null
if [ $? -eq 0 ] 
then 
  echo -e  "\033[1;32mENTRADA-SALIDA compilo bien \033[m" 
else 
  echo -e "\033[1;31mRIP ENTRADA-SALIDA\033[m" >&2 
fi
a=$var
var=$((var || a))
cd .. 

# ----------------------------- KERNEL ------------------------

cd kernel 
make clean > /dev/null
make > /dev/null
if [ $? -eq 0 ] 
then 
  echo -e "\033[1;32mKERNEL compilo bien \033[m" 
else 
  echo -e "\033[1;31mRIP KERNEL\033[m" >&2 
fi
a=$var
var=$((var || a))
cd .. 

# ----------------------------- UTILS  ------------------------

cd utils 
make clean > /dev/null
make > /dev/null
if [ $? -eq 0 ] 
then 
  echo -e "\033[1;32mUTILS compilo bien \033[m" 
else 
  echo -e "\033[1;31mRIP UTILS"\033[m >&2 
fi
a=$var
var=$((var || a))
cd .. 

#echo "$var"
exit $var
