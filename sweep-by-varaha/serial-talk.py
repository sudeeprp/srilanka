import serial
import time

import serial.tools.list_ports
ports = serial.tools.list_ports.comports()

print(f'opening {ports[0][0]}')
ser = serial.Serial(ports[0][0])

for i in range(40):
    line = ser.readline().decode('utf-8').rstrip()
    if line:
        print(line)
    time.sleep(0.5)
ser.close()
