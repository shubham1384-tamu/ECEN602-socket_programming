#include<stdlib.h>
#include<stdio.h>       // for I/O operations
#include<sys/socket.h>  //for socket api
#include<sys/types.h>
#include<netinet/in.h>
#include <unistd.h>
#include<string.h>      //for string manipulation

#define BUFFER_SIZE 1024

//Main c code
int main(int argument_number, char** port_number)
{
    int server_socket;
    pid_t child_id;
    if((server_socket = socket(AF_INET,SOCK_STREAM,0))==0)
    {
        printf("Socket creation failed\n");
        exit(0);
    }
    else
    {
        printf("Socket server started \n");
    }

    //define address structure
    struct sockaddr_in server_addr;  // Socket descriptor
    server_addr.sin_family = AF_INET; //sets family of the address. AF_INET is for connecting on an IpV4 address
    server_addr.sin_port = htons(atoi(port_number[1]));  // actual port no defined by the user.
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //struct in_addr ipAddr = server_addr.sin_addr;  //to get ipv4 address

    // Socket binding is used to connect the socket with the specified IP address and port for listening
    if(bind(server_socket,(struct sockaddr*) &server_addr, sizeof(server_addr))<0)
    {
        printf("Bind error\n");
        exit(0);
    }
    else
    {
    char server_ip[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &server_addr.sin_addr.s_addr, server_ip, INET_ADDRSTRLEN );
    printf("Server is at IP: %s\n",server_ip);
    }
    
    //listen for incoming server request
    listen(server_socket,5);  //5 connections 


    int client_socket;
    socklen_t addrlen = sizeof(server_addr);
    
    int number_of_connections=0;
    while(1)
    {
        
        client_socket = accept(server_socket,(struct sockaddr*)&server_addr,(socklen_t*)&addrlen); //need to explore this function
    //client_socket = accept(server_socket,(struct sockaddr*)&client_address,(socklen_t*)&client_address_size); //need to explore this function

    if (client_socket < 0) {
        exit(1);
        }
        else
        {
            number_of_connections+=1;
            printf("Accepted a new connnection\n");
            printf("Total clients connected: %d\n",number_of_connections);
            
        }
    //printf("Connection accepted from %s:%d\n",inet_ntoa(client_socket0>sin_addr), ntohs(client_socket->sin_port));
    
    // create a new process using fork
    //if child_id=0, that means it is a child process
    if ((child_id = fork()) == 0) 
    {
        close(server_socket);  //closed parent process as child process is handling existing client
        while(1)
        {
        char read_char[BUFFER_SIZE]={0};
        int valread=read(client_socket, read_char, sizeof(read_char));
        if(valread==0)
        {
        printf("TCP_FIN packet received. Client disconnected\n");
        close(client_socket);
        printf("Exiting child process\n");
        number_of_connections-=1;
        exit(0);
        }
        // print the data read
        printf("Read from client: %s\n",read_char);

        //char write_back[1024];
        send(client_socket, read_char, strlen(read_char),0);
        }
         close(client_socket);
    }
    
    close(client_socket);
    
    }
    printf("Closing server socket\n");
    close(server_socket);
    return 0;
}
