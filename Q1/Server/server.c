#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;  // sockaddr_in - references elements of the socket address. "in" for internet
	int opt = 1;
	int addrlen = sizeof(address);
	//char buffer[1024] = {0};
	//char * hello = "Hello from server";

	/*
	   socket(domain, type, protocol)
domain: integer, communication domain e.g., AF_INET (IPv4 protocol) , AF_INET6 (IPv6 protocol)
type: communication type
SOCK_STREAM: TCP(reliable, connection oriented)
SOCK_DGRAM: UDP(unreliable, connectionless)
protocol: Protocol value for Internet Protocol(IP), which is 0.
return value : socket descriptor, an integer (like a file-handle)
*/
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	printf("Socket retrieve success\n");



	/*
	   This helps in manipulating options for the socket referred by the file descriptor sockfd. 
	   This is completely optional,but it helps in reuse of address and port. 
	   Prevents error such as: “address already in use”.*/

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;  // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc. 
	address.sin_addr.s_addr = htonl(INADDR_ANY);  // Accept connections from any IP address - listens from all interfaces.
	address.sin_port = htons( PORT );    // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

	/*
	   int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	   After creation of the socket, bind function binds the socket to the address and port number specified in addr(custom data structure).
	   */

	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	/*
	   int listen(int sockfd, int backlog);
	   It puts the server socket in a passive mode, where it waits for the client to approach the server to make 
	   a connection. 
	   The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow.
	   If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED.
	   */
	// The process is two step: first you listen(), then you accept()
	printf("Server Listening....\n");
	while(1)
	{
		if (listen(server_fd, 10) < 0) // 10 is the maximum size of queue - connections you haven't accepted
		{
			perror("listen");
			exit(EXIT_FAILURE);
		}

		/*
		   int new_socket= accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		   It extracts the first connection request on the queue of pending connections for the listening socket, 
		   sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket.
		   At this point, connection is established between client and server, and they are ready to transfer data.
		   */
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		char ip[100]={};
		char buffer[1024] = {0};

		inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);
		printf("Connection established with %s\n",ip);
		valread = read( new_socket , buffer, 124);  // read infromation received into the buffer
		if(strcmp(buffer,"CLOSE")==0){
			printf("Program closed\n");
			close(new_socket);
			continue;
		}

		char f_path[1000]={};
		sprintf(f_path,"Data/%s",buffer);

		printf("%s\n",buffer );
		FILE *fp = fopen(f_path,"r");
		if(fp==NULL)
		{
			write(new_socket,"ERROR\0",6);
			printf("File opern error");
			//return 1;   
		} 
		else
        {
			/* Read data from file and send it */
			while(1)
			{
				/* First read file in chunks of 256 bytes */
				unsigned char buff[256]={0};
				int nread = fread(buff,1,256,fp);
				//printf("Bytes read %d \n", nread);        

				/* If read was success, send data. */
				if(nread > 0)
				{
					//printf("Sending \n");
					write(new_socket ,  buff, nread);
				}

				/*
				 * There is something tricky going on with read .. 
				 * Either there was error, or we reached end of file.
				 */
				if (nread < 256)
				{
					if (feof(fp))
						printf("End of file\n");
					if (ferror(fp))
						printf("Error reading\n");
					break;
				}
			}
			fclose(fp);
		}

		printf("%s disconnected\n",ip);
		close(new_socket);
	}
	return 0;
}
