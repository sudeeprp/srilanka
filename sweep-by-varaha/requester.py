import serial
import time
import serial.tools.list_ports
import pygame
import sys
import threading
import paho.mqtt.client as mqtt


def varaha_rise():
  pygame.mixer.music.load('huaah.mp3')
  pygame.mixer.music.play()
  time.sleep(4)
  pygame.mixer.music.load('varaha-roopam-shiva-sambhoota.mp3')
  pygame.mixer.music.play()
  time.sleep(45.5)
  print('playing done')


def ravana_power():
  pygame.mixer.music.load('ravana.mp3')
  pygame.mixer.music.play()
  request_to_ports('P')

def ravana_to_rama():
  request_to_ports('Q')
  pygame.mixer.music.load('rangapura-vihara.mp3')
  pygame.mixer.music.play()
  request_to_ports('D')
  

def varaha_final(message):
  if message == b'S':
    player = threading.Thread(target=ravana_power)
    player.start()
  elif message == b'D':
    player = threading.Thread(target=ravana_to_rama)
    player.start()
  elif message == b'R':
    request_to_ports('R')
    request_to_ports('U')


def raksha_apeksha():
  def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("varaha/#")
  def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if msg.topic == 'varaha/final':
      varaha_final(msg.payload)
  client = mqtt.Client()
  client.on_connect = on_connect
  client.on_message = on_message
  client.connect("mqtt.eclipseprojects.io", 1883, 60)
  client.loop_forever()


def dispatch(event):
  if event == 'varaha':
    player = threading.Thread(target=varaha_rise)
    player.start()


serialports = []

def dispatch_from_ports():
  while True:
    for incomingserial in serialports:
      if incomingserial.in_waiting > 0:
        serialstr = incomingserial.readline().decode().strip()
        print(serialstr)
        dispatch(serialstr)


def request_to_ports(reqstr):
  print(f'requesting all ports: {reqstr}')
  for outgoingserial in serialports:
    outgoingserial.write(reqstr.encode())


def dispatch_from_input():
  while True:
    event = input('tell: ')
    dispatch(event)



ports = serial.tools.list_ports.comports()
for comport in ports:
  print(f'opening {comport[0]}')
  serialports.append(serial.Serial(comport[0]))

if len(sys.argv) > 1:
  print('reset')
  request_to_ports('R')
else:
  pygame.mixer.init()
  finalizer = threading.Thread(target=raksha_apeksha)
  finalizer.start()
  if len(ports) > 0:
    dispatch_from_ports()
  else:
    dispatch_from_input()

