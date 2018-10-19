#! /usr/bin/env python

import serial, serial.tools.list_ports
import time

ports = list(serial.tools.list_ports.comports())
arduino = serial.Serial(ports[0][0], 115200) # buggy line
time.sleep(2)

#for p in ports: # use this instead of the buggy line
#    if p[1] == 'Arduino Mega':
#        print p
#        arduino = serial.Serial(p[0], 9600)
#        time.sleep(2)
#        break

CRAZY_SIGNAL = '>'

arduino.write(CRAZY_SIGNAL)
time.sleep(0.04)

print('Sending coordinates...\n')

file = open('coordenadas.txt', 'r')

for line in file.readlines()[1:]:
    arduino.write(line)
    time.sleep(0.04)
file.close()

arduino.write(CRAZY_SIGNAL)
time.sleep(0.04)

print('Processing route...\n')

file = open('ruta.txt', 'r')

for line in file.readlines()[1:]:
    arduino.write(line)
    time.sleep(0.07)
    print(arduino.read(arduino.inWaiting()))
file.close()

arduino.write(CRAZY_SIGNAL)
time.sleep(0.07)
print(arduino.read(arduino.inWaiting()))
