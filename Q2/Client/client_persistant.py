import socket
import sys
# socket is made only once
#s = socket.socket()
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print "Socket successfully created"
except socket.error as err:
    print "socket creation failed with error %s" %(err)
host = ""
port = 60004


num_files = len(sys.argv)
list_files = sys.argv
list_files.append('EXIT')
if num_files <2:
    print('Please enter file names')
    sys.exit()
i = 1
#Connection is made only once           
s.connect((host, port))

# Receives the list of files present on the server
data = s.recv(1024)
#print(list_files,num_files)
#print(list_files[1],list_files[2])

print("The following files are on the server :-")
print(data)


while 1:
    # Asks the file name
    #fname=raw_input("Enter File name:")
    fname = list_files[i]
    print(fname)

    # If client wants to EXIT
    if fname == 'EXIT':
        s.send(fname)
        s.close()
        break
    s.send(fname)
    # Waits for server to ensure that file is present on the server
    data = s.recv(10)
    #print(data)
    
    if 'FILE-ERROR' in data:
        print("File doesn't exist on server")
        i+=1
        continue
    else:
        with open(fname, 'wb') as f:
            while True:            
                data = s.recv(1024)
                #print(data) 
                if 'EOFfdgjdfhghdfjgdf' in data:
                    k = data.replace('EOFfdgjdfhghdfjgdf', '') # Replace the hash as data is to be written to file 
                    f.write(k)
                    break
                f.write(data)
            f.close()
    print(fname ,"downloaded successfully")

    #if i == num_files-1:
    #    break
    i+=1

# Connection is closed after all files are transferred
s.close()
print('connection closed')