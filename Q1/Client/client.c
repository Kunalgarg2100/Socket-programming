// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
	int cnt = argc;
	if(cnt == 1)
		{
		printf("Please enter file names\n");
		return 1;
}
	int i=1;
	//printf("%s\n",argv[1]);
	while(1)
	{
		struct sockaddr_in address;
		int sock = 0;
		struct sockaddr_in serv_addr;
		char file_name[1000]={};
		
    /*
    int socket(int domain, int type, int protocol);
    socket() creates an endpoint for communication and returns a descriptor.
    domain :- The  domain argument specifies a communication domain; 
        this selects the protocol family which will be used for communication.
    type:- SOCK_STREAM Provides sequenced, reliable, two-way, connection-based byte streams.
        Sockets  of  type  SOCK_STREAM  are full-duplex byte streams.  
        They do not preserve record boundaries.  A stream socket must be in a
       connected state before any data may be sent or received on it. 
    protocol :- The  protocol  specifies a particular protocol to be used with the socket.*/
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			printf("\n Socket creation error \n");
			return -1;
		}

		// To make sure the struct is empty. Essentially sets sin_zero as 0
        // which is meant to be, and rest is defined below
        memset(&serv_addr, '0', sizeof(serv_addr)); 
		serv_addr.sin_family = AF_INET;
        
        //The htons() function converts the unsigned short integer hostshort 
        //from host byte order to network byte order.
		serv_addr.sin_port = htons(PORT);

		// Converts an IP address in numbers-and-dots notation into either a 
		// struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
		if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
			printf("\nInvalid address/ Address not supported \n");
			return -1;
		}

		/*
		   int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
		   The connect() system call connects the socket referred to by the file descriptor sockfd to the 
           address specified by addr. Server’s address and port is specified in addr.*/
		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
		{
			printf("\nConnection Failed \n");
			return -1;
		}

	//	printf("Enter file name:");
	//	char f_name[1000]={};
	//	scanf("%s",f_name);
		sprintf(file_name,"%s\0",argv[i]); //To add null after the file name

		if(strcmp(file_name,"q")==0){ //If file name is q -> close the sock
			send(sock,"CLOSE",5,0);
			close(sock);
			return -1;
		}


		else{
		printf("file-name %s\n",file_name);

            /*ssize_t send(int sockfd, const void *buf, size_t len, int flags);
            the message is found in buf and has length len. */
			send(sock , file_name , strlen(file_name) , 0 );

            /* Create file where data will be stored */
			FILE *fp;
			fp = fopen(file_name, "w"); 
			if(fp == NULL)
			{
				printf("Error opening file");
				return 1;
			}

			int bytesReceived = 0;
			char recvBuff[256];
			memset(recvBuff, '0', sizeof(recvBuff));
			int flag=0;

			/* Receive data in chunks of 256 bytes */
			while((bytesReceived = read(sock, recvBuff, 256)) > 0)
			{
				if(strcmp(recvBuff,"ERROR\0")==0){
                        flag=1;
                        break;
                }
				fwrite(recvBuff, 1,bytesReceived,fp);
			}

			fclose(fp);
			if(bytesReceived < 0)
				printf("\n Read Error \n");
			if(flag==0)
				printf("File transmission completed for %s\n",file_name);
			else
				printf("Such a file doesn't exsist at server side\n");
			close(sock);
		}
		if(i==cnt-1)
			break;
		else
			i++;
	}

	return 0;
}
