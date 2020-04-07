#!/usr/bin/env python

import socket
import sys


TCP_IP = '127.0.0.1'
TCP_PORT = 2302
BUFFER_SIZE = 1024
message = ""

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
try:
    while(1):
        #print ("Ingrese la ip a enviar el mensaje")
        #message=input()
        #s.send(message.encode())
        print ("Ingrese el puerto a enviar el mensaje")
        message=input()
        s.send(message.encode())
        print ("Ingrese el mensaje a enviar")
        message=input()
        s.send(message.encode())
        if message=="exit": break
        message = s.recv(BUFFER_SIZE)
        print ("Se recibio del nodo verde:", message.decode())
        print ("")
except KeyboardInterrupt:
    print("\nGoodbye!")
finally:
    s.close()
