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
file_names = ['coordenadas.txt', 'ruta.txt']

print('[SIGNAL]')
arduino.write(CRAZY_SIGNAL + '\n')
time.sleep(0.07) # these
print('[OUTPUT]') # three lines
print(arduino.read(arduino.inWaiting())) # are repeated three times

for file_name in file_names:
    print('[FILE] ' + file_name)
    file = open(file_name, 'r')

    for line in file.readlines()[1:]:
        print('[LINE] ' + line)
        arduino.write(line)
        time.sleep(0.07)
        print('[OUTPUT]')
        print(arduino.read(arduino.inWaiting()))

    print('[SIGNAL]')
    arduino.write(CRAZY_SIGNAL + '\n')
    time.sleep(0.07)
    print('[OUTPUT]')
    print(arduino.read(arduino.inWaiting()))
    file.close()
