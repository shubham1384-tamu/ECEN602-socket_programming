#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>  //for socket api
#include<sys/types.h>
#include<netinet/in.h>
#include <unistd.h>
#include<string.h>


int main()
{
    int client_socket;
    if((client_socket = socket(AF_INET,SOCK_STREAM, 0))<0)  //domain=AF_INET, TCP protocol, default TCP protocol
    {
        printf("socket creation error\n");
        return 0;
    }

    //netinet
    //address structure for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; //sets family of the address
    server_address.sin_port = htons(9002);//htons(9002);  //send actual port no
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        printf("Invalid address / Address not supported\n");
        return -1;
    }
    
    //server_address.sin_addr.s_addr = INADDR_ANY;  // connect to localhost

    if(connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address))<0)
    {
        printf("Error making connection\n");
        //return(0);
    }
    else
    {
        printf("Connection to the server successful\n");
    }

while(1)
{
    char write[1024];
    //scanf("%s",write);
    if(fgets(write,sizeof(write),stdin)==NULL)
    {
        send(client_socket, write, strlen(write),0);
        break;
    }
    else
    send(client_socket, write, strlen(write),0);
    //char* hello = "Hello from client";
    


    //echoed signal read
    char echo_read[1024];
    int valread=read(client_socket, echo_read, 1024);
    printf("Read from Server: %s\n",echo_read);
    //printf("%s",write); 
}

printf("Closing the connection\n");
    close(client_socket);
    return 0;
}