import serial
import time
import sys
import serial.tools.list_ports
ports = serial.tools.list_ports.comports()

print(f'opening {ports[0][0]}')
ser = serial.Serial(ports[0][0])

while True:
  request = input('req: ')
  ser.write(request.encode())
