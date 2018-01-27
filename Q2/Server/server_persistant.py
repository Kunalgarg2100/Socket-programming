# Once connection is setup client can download any number of files.

import socket
import os
port = 60004
#Here we made a socket instance and passed it two parameters. 
#The first parameter is AF_INET and the second one is SOCK_STREAM.
#AF_INET refers to the address family ipv4
#Secondly the SOCK_STREAM means connection oriented TCP protocol
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print "Socket successfully created"
except socket.error as err:
    print "socket creation failed with error %s" %(err)

host = ""

# Next bind to the port, we have not typed any ip in the ip field 
# instead we have inputted an empty string this makes the server listen to requests 
# coming from other computers on the network
s.bind((host, port))
print "socket binded to %s" %(port)

# put the socket into listening mode
#5 connections are kept waiting if the server is busy and 
#if a 6th socket trys to connect then the connection is refused.
s.listen(5)
print "socket is listening"

# MAking the list of files present on the server
direc = os.path.dirname(__file__)
datadirec = os.path.join(direc, 'Data')
L = []
for file in os.listdir(datadirec):
    L.append(file)
file_list =  '\n'.join(str(x) for x in L)

while True:
    conn, addr = s.accept()    
    print 'Got connection from', addr

    # Sending the list of files to the client
    conn.send(file_list)

    # This while loop takes the filename from the client again and again without closing the connection
    while True:
        # filename is recieved from client
        filename = conn.recv(1024)

        # Client wants to close the connection, break the loop and close the connection
        if 'EXIT' in filename:
            conn.close()
            break
        print filename

        if os.path.isfile('Data/' + filename): # Checks if file available and send "FILE-SUCCESS" message to the client
            conn.send("FILE-SUCCE")    
            f = open('Data/' + filename,'rb')
            l = f.read(1024)
            while (l):
                conn.send(l)
                l = f.read(1024)
            print('Done sending')
            conn.send('EOFfdgjdfhghdfjgdf')
            f.close()
        else:
            conn.send("FILE-ERROR")    
    
    # After the client types 'EXIT' the connection is closed.
    print('Connection closed by client')    
    conn.close()