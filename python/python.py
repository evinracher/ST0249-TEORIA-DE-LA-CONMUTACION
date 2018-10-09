import serial

arduino = serial.Serial('/dev/ttyACM2', 9600)

file = open("coordenadas.txt", "r")
lines = file.readlines()

# Start
arduino.write("\000\n")

# Send coordenates
for line in lines:
    c_input = arduino.read()
    while c_input != '\000':
        c_input = arduino.read()
    arduino.write(line),

file.close()

file = open("ruta.txt", "r")
lines = file.readlines()

# Send routes
arduino.write("\000\n")
for line in lines:
    while c_input != '\000':
        c_input = arduino.read()
    arduino.write(line),

file.close()

# End
arduino.write("\000\n")
