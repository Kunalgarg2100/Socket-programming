# Connection is closed after a file is sent

import socket
import os
port = 60007    

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

direc = os.path.dirname(__file__)
datadirec = os.path.join(direc, 'Data')
L = []
for file in os.listdir(datadirec):
    L.append(file)
file_list =  '\n'.join(str(x) for x in L)

while True:
    # Establish connection with client.
    conn, addr = s.accept()
    print 'Got connection from', addr
    
    # Making the list if files present in the directory
    
    # Sending the list of files to the client
    conn.send(file_list)

    # Recieving the file name from client
    filename = conn.recv(1024)
    if 'EXIT' in filename: # If we type 'EXIT' -> No file is transfeered and the server switches off
        print 'Connection closed by client'
        conn.close()
        continue

    print filename # Filename given by the client

    if os.path.isfile('Data/' + filename): #Checks if the file is present in the server
        conn.send("FILE-SUCCE") # If yes send "FILE-SUCCESS" message to client  
        
        # Open the file
        f = open('Data/' + filename,'rb')

        # Reading form the file
        txt = f.read(1024)
        while (txt):
            conn.send(txt)
            txt = f.read(1024)
        f.close()

        #After the whole file is send, server sends this coupon to tell client that file has sended
        conn.send('434139230920poiuytrewgjdfhghdfjgdf')
        print('Done sending')
        print('Thank you for connecting')
    else: # If no file is present server sends "FILE-ERROR" message
        conn.send("FILE-ERROR")
        print('File not found')
        print('Thank you for connecting') 

    
    
    # Close the connection with the client after a file is sent
    conn.close()