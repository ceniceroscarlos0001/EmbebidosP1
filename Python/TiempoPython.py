import RPi.GPIO as GPIO
import time
import sys

GPIO.setmode(GPIO.BCM)
pins = [27, 22, 10, 9, 11, 5, 6, 13]

if len(sys.argv) != 2:
    print("Usar: python3 gpio_reader.py [config|valor|cerrar]")
    sys.exit(1)

if sys.argv[1] == "config":
    for pin in pins:
        GPIO.setup(pin, GPIO.IN)
    print("GPIO configurados como entrada.")
    amplitud = open("amplitudPython.txt", "w")
    tiempo = open("tiempoPython.txt", "w")
    for _ in range(10000):
        numero = 0
        for i, pin in enumerate(pins):
            valor = GPIO.input(pin)
            numero += valor << i
        amplitud.write(f"{numero}\n")
        tiempo.write(f"{time.time_ns()}\n")
    amplitud.close()
    tiempo.close()
    GPIO.cleanup()
    print("GPIO liberados.")
else:
    print("Opción inválida")