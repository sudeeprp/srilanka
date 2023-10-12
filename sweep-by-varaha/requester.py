import serial
import time
import serial.tools.list_ports
import pygame

def dispatch(event):
  if event == 'varaha':
    print('varaha!')
    pygame.mixer.music.load('huaah.mp3')
    pygame.mixer.music.play()
    time.sleep(4)
    pygame.mixer.music.load('varaha-roopam-shiva-sambhoota.mp3')
    pygame.mixer.music.play()
    time.sleep(45.5)
    print('done')


serialports = []

def dispatch_from_ports():
  for comport in ports:
    print(f'opening {comport[0]}')
    serialports.append(serial.Serial(comport[0]))
  while True:
    for incomingserial in serialports:
      if incomingserial.in_waiting > 0:
        serialstr = incomingserial.readline().decode()
        print(f'received >{serialstr}<')
        dispatch(serialstr)
    time.sleep(0.25)


def request_to_ports(reqstr):
  for outgoingserial in serialports:
    outgoingserial.write(reqstr.encode())


def dispatch_from_input():
  while True:
    event = input('tell: ')
    dispatch(event)


pygame.mixer.init()
ports = serial.tools.list_ports.comports()
if len(ports) > 0:
  dispatch_from_ports()
else:
  dispatch_from_input()

