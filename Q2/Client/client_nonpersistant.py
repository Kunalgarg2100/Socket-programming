import socket                   
import sys
host = ""
port = 60007

# number of files to be transfered
num_files = len(sys.argv) 

# Names of files to be transfered
list_files = sys.argv
list_files.append('EXIT')

#print(list_files,num_files)
#print(list_files[1],list_files[2])

if num_files <2:
    print('Please enter file names') # If no filenames are givem 
    sys.exit()

y = 1
while True:
    # Making new socket for every file transfer
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print "Socket successfully created"
    except socket.error as err:
        print "socket creation failed with error %s" %(err)
    #s = socket.socket()  

    s.connect((host, port)) # Tries to connect to the server after every file transfer
    
    data = s.recv(1024)
    if y == 1:
        print("The following files are on the server :-")
        print(data) #Prints the list of files on the server

    fname = list_files[y]
    print(fname)

    #fname= raw_input("Enter File name: ")
    if fname == 'EXIT': #If filename is EXIT client exits and closes the connection
        s.send(fname)
        s.close()
        break
    
    s.send(fname) #Sends filename to server

    # Receives if file is present on server of not?
    data = s.recv(10) 

    if 'FILE-ERROR' in data: # If file is not present on server
        print("File doesn't exist on server")
        y+=1
        continue

    else:
        #print('File is present on server')
        with open(fname, 'wb') as f:
            i=0
            while True:
                i+=1            
                data = s.recv(1024)
                if '434139230920poiuytrewgjdfhghdfjgdf' in data:
                    k = data.replace('434139230920poiuytrewgjdfhghdfjgdf', '') # replace the hash msg with nothing
                    f.write(k)
                    break
                else:
                    f.write(data)
            f.close()
    y+=1

    print(fname, 'Successfully downloaded the file')
    s.close()
    print('Connection Closed\n')