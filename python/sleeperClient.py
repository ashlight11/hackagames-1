#!env python3
import socket

HOST= 'localhost'
PORT= 2014

with socket.socket( socket.AF_INET, socket.SOCK_STREAM) as s :
    s.connect((HOST, PORT))
    while True :
        data= s.recv(1024)
        if not data :
            break
        data= data.decode('utf-8').split('\n')
        for line in data :
            print( "- ", line )
            if line == "Your turn:" :
                s.send( b'sleep' )
