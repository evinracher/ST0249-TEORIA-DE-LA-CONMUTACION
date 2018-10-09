import serial

arduino = serial.Serial('/dev/ttyACM1', 9600)

arduino.write("\000JK\n")

