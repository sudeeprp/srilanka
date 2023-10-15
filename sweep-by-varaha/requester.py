import serial
import time
import serial.tools.list_ports
import pygame
import sys
import os
import threading
import paho.mqtt.client as mqtt
from playvideo import play_video
import showimage

current_state = 'tostart'


def show_varaha():
  showimage.show_image('varaha-intro.jpg')

def varaha_rise():
  global current_state
  pygame.mixer.music.load('huaah.mp3')
  pygame.mixer.music.play()
  time.sleep(4)
  pygame.mixer.music.load('varaha-roopam-shiva-sambhoota.mp3')
  pygame.mixer.music.play()
  time.sleep(45.5 + 18)
  print('playing done')
  current_state = 'varahadone'


def ravana_power():
  global current_state
  showimage.stop_image()
  pygame.mixer.music.load('ravana.mp3')
  pygame.mixer.music.play()
  play_video('video-ravana.mp4')
  request_to_ports('P')
  current_state = 'ravanafinishing'
  showimage.show_image('vibheeshana-pointer.png')

def ravana_to_rama():
  showimage.stop_image()
  request_to_ports('Q')
  pygame.mixer.music.load('rangapura-vihara.mp3')
  pygame.mixer.music.play()
  threading.Thread(target=ranga_descends_in_a_while).start()
  play_video('video-rangapura.mp4', 'varaha-intro.jpg')

def ranga_descends_in_a_while():
  print('descending in a while...')
  time.sleep(15)
  request_to_ports('D')


def varaha_final(message):
  global current_state
  if message == b'S':
    if current_state == 'varahadone':
      current_state = 'ravanastarted'
      player = threading.Thread(target=ravana_power)
      player.start()
  elif message == b'D':
    if current_state == 'ravanafinishing':
      current_state = 'ranga'
      player = threading.Thread(target=ravana_to_rama)
      player.start()
  elif message == b'R':
    request_to_ports('R')
    request_to_ports('U')
    time.sleep(0.5)
    os._exit(0)
  elif message == b'L':
    print('Exiting...')
    os._exit(0)
  elif message == 'up':
    print('ranga returns')
    request_to_ports('U')
  elif message == 'down':
    print('ranga descends')
    request_to_ports('D')
  elif message == 'point':
    request_to_ports('P')
  elif message == 'pointoff':
    request_to_ports('Q')


def raksha_apeksha():
  def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("varaha/#")
    client.publish("varaha/ver", '2.0ex', True)
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
  global current_state
  if event == 'varaha' and current_state == 'tostart':
    current_state = 'varahastarted'
    player = threading.Thread(target=varaha_rise)
    player.start()


serialports = []

def start_over():
  introshow = threading.Thread(target=show_varaha)
  introshow.start()


def dispatch_from_ports():
  start_over()
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
  start_over()
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
  if len(ports) == 2:
    dispatch_from_ports()
  elif len(ports) == 0:
    dispatch_from_input()
  else:
    print('could not find all ports. reconnect all usb slowly, and start again')
    import keyboard
    keyboard.getch()
    os._exit(1)
