#!/usr/bin/env python

import socket
import sys


TCP_IP = '127.0.0.1'
TCP_PORT = 2002
BUFFER_SIZE = 1024
message = ""

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
while(1):
    message=input()
    s.send(message.encode())
    if message=="exit": break
    message = s.recv(BUFFER_SIZE)
    print ("received data:", message.decode())
s.close()
