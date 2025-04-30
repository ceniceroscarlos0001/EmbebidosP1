#!/bin/bash

# GPIO utilizados para generar numero (8 pines)
GPIO_0=27
GPIO_1=22
GPIO_2=10
GPIO_3=9
GPIO_4=11
GPIO_5=5
GPIO_6=6
GPIO_7=13

# En caso que no se pase algun valor
if [ $# -ne 1 ]; then   # si no hay argumento
  echo "No hay comando"
  echo "los comandos a utilizar son config, valor, cerrar"
  exit 2     # Numero invalido de argumentos
fi

# Configurar GPIO como entradas
if [ "$1" == "config" ]; then
  echo "Exportando GPIO numero $GPIO_0"
  echo $GPIO_0 >> "/sys/class/gpio/export"
  sleep 1  # para asegurar que se exporto correctamente
  echo "in" >> "/sys/class/gpio/gpio$GPIO_0/direction"
  echo "Exportando GPIO numero $GPIO_1"
  echo $GPIO_1 >> "/sys/class/gpio/export"
  sleep 1  # para asegurar que se exporto correctamente
  echo "in" >> "/sys/class/gpio/gpio$GPIO_1/direction"
  echo "Exportando GPIO numero $GPIO_2"
  echo $GPIO_2 >> "/sys/class/gpio/export"
  sleep 1
  echo "in" >> "/sys/class/gpio/gpio$GPIO_2/direction"
  echo "Exportando GPIO numero $GPIO_3"
  echo $GPIO_3 >> "/sys/class/gpio/export"
  sleep 1
  echo "in" >> "/sys/class/gpio/gpio$GPIO_3/direction"
  echo "Exportando GPIO numero $GPIO_4"
  echo $GPIO_4 >> "/sys/class/gpio/export"
  sleep 1
  echo "in" >> "/sys/class/gpio/gpio$GPIO_4/direction"
  echo "Exportando GPIO numero $GPIO_5"
  echo $GPIO_5 >> "/sys/class/gpio/export"
  sleep 1
  echo "in" >> "/sys/class/gpio/gpio$GPIO_5/direction"
  echo "Exportando GPIO numero $GPIO_6"
  echo $GPIO_6 >> "/sys/class/gpio/export"
  sleep 1
  echo "in" >> "/sys/class/gpio/gpio$GPIO_6/direction"
  echo "Exportando GPIO numero $GPIO_7"
  echo $GPIO_7 >> "/sys/class/gpio/export"
  sleep 1
  echo "in" >> "/sys/class/gpio/gpio$GPIO_7/direction"
fi

if [ "$1" == "cerrar" ]; then
  echo "Cerrando el GPIO $GPIO_0"
  echo $GPIO_0 >> "/sys/class/gpio/unexport"
  echo "Cerrando el GPIO $GPIO_1"
  echo $GPIO_1 >> "/sys/class/gpio/unexport"
  echo "Cerrando el GPIO $GPIO_2"
  echo $GPIO_2 >> "/sys/class/gpio/unexport"
  echo "Cerrando el GPIO $GPIO_3"
  echo $GPIO_3 >> "/sys/class/gpio/unexport"
  echo "Cerrando el GPIO $GPIO_4"
  echo $GPIO_4 >> "/sys/class/gpio/unexport"
  echo "Cerrando el GPIO $GPIO_5"
  echo $GPIO_5 >> "/sys/class/gpio/unexport"
  echo "Cerrando el GPIO $GPIO_6"
  echo $GPIO_6 >> "/sys/class/gpio/unexport"
  echo "Cerrando el GPIO $GPIO_7"
  echo $GPIO_7 >> "/sys/class/gpio/unexport"
fi

if [ "$1" == "valor" ]; then
  contador=1
  timeStamp=$(date +%s%N)
  echo $timeStamp >> tiempoShell.txt
  while [ $contador -le 10000 ]
  do
  # echo "Estampa tiempo: $timeStamp"
  # echo "Contador: $contador"
    contador=$((contador + 1))
    # Obtencion de datos
    bit0=$(cat "/sys/class/gpio/gpio$GPIO_0/value")
    bit1=$(cat "/sys/class/gpio/gpio$GPIO_1/value")
    bit2=$(cat "/sys/class/gpio/gpio$GPIO_2/value")
    bit3=$(cat "/sys/class/gpio/gpio$GPIO_3/value")
    bit4=$(cat "/sys/class/gpio/gpio$GPIO_4/value")
    bit5=$(cat "/sys/class/gpio/gpio$GPIO_5/value")
    bit6=$(cat "/sys/class/gpio/gpio$GPIO_6/value")
    bit7=$(cat "/sys/class/gpio/gpio$GPIO_7/value")	
    let numero=bit0+bit1+bit2+bit3+bit4+bit5+bit6+bit7
    echo $numero >> amplitudShell.txt
    timeStamp=$(date +%s%N)
    echo $timeStamp >> tiempoShell.txt
  done
fi