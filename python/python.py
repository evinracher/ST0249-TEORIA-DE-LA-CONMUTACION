import serial, time


arduino = serial.Serial('/dev/ttyACM2', 9600)
time.sleep(2)
file = open("coordenadas.txt", "r")
lines = file.readlines()

CRAZY_SIGNAL = '>'

# Start
arduino.write(CRAZY_SIGNAL + '\n')

# Send coordenates
for line in lines:
    while arduino.read() != CRAZY_SIGNAL:
        pass
    print("test")
    arduino.write(line)
    
file.close()
time.sleep(2)

file = open("ruta.txt", "r")
lines = file.readlines()

# Send routes
arduino.write(CRAZY_SIGNAL + '\n')
for line in lines:
    while arduino.read() != CRAZY_SIGNAL:
        pass
    arduino.write(line)
    

file.close()

# End
arduino.write(CRAZY_SIGNAL + '\n')
