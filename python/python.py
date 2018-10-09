#! /usr/bin/env python

import serial

arduino = serial.Serial("/dev/ttyACM2", 9600)

file = open("coordenadas.txt", "r")
lines = file.readlines()

# start
arduino.write("\000\n")

# send coordinates
for line in lines:
    c_input = arduino.read()
    while c_input != '\000':
        c_input = arduino.read()
    arduino.write(line),

file.close()

file = open("ruta.txt", "r")
lines = file.readlines()

# send routes
arduino.write("\000\n")
for line in lines:
    while c_input != '\000':
        c_input = arduino.read()
    arduino.write(line),

file.close()

# end
arduino.write("\000\n")
