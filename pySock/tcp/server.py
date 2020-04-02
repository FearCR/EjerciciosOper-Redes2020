#!/usr/bin/python3.8

import socket
import sys
import os
TCP_IP = '127.0.0.1'
TCP_PORT = 2002
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

try:
    while 1:
        conn, addr = s.accept()
        print ('Connection address:', addr)
        n=os.fork()
        if n>0:
            os.system("ps")
            while(1):
                data = conn.recv(BUFFER_SIZE)
                if data.decode()=="exit": break
                print ("received data:", data.decode())
                conn.send(data)  # echo
            os.wait
                #break
except KeyboardInterrupt:
        print("\nGoodbye!")
finally:
    os.wait
    conn.close()
