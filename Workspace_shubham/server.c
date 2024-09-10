#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>  //for socket api
#include<sys/types.h>
#include<netinet/in.h>
#include <unistd.h>
#include<string.h>

int main(int argument_number, char** port_number)
{
    int server_socket;
    pid_t child_id;
    if((server_socket = socket(AF_INET,SOCK_STREAM,0))==0)
    {
        printf("Socket creation failed\n");
    }

    //setsockopt(server_socket, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, 1,sizeof(int));

    //verifying the argument and port number
    /*
    printf("%d\n",argument_number);
    printf("%s\n",port_number[1]);
    */
    //define address structure
    struct sockaddr_in server_addr, conn_addr;
    server_addr.sin_family = AF_INET; //sets family of the address
    server_addr.sin_port = htons(9002); //atoi(port_number[1]);//htons(9002);  //send actual port no
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket,(struct sockaddr*) &server_addr, sizeof(server_addr))<0)
    {
        printf("Bind error\n");
    }

    listen(server_socket,5);  //5 connections 


    int client_socket;
    socklen_t addrlen = sizeof(server_addr);
    
    while(1)
    {
        client_socket = accept(server_socket,(struct sockaddr*)&server_addr,(socklen_t*)&addrlen); //need to explore this function
    if (client_socket < 0) {exit(1);}
    //printf("Connection accepted from %s:%d\n",inet_ntoa(client_socket.sin_addr), ntohs(client_socket.sin_port));
    if ((child_id = fork()) == 0) 
    {
        close(server_socket);
        while(1)
        {
        char read_char[1024];
        int valread=read(client_socket, read_char, 1024);
        if(valread==0)
        {
        break;
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
/*
    // send server message
    send(client_socket, message, sizeof(message),0);
*/
    close(server_socket);
    return 0;
}
