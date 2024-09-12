#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>  //for socket api
#include<sys/types.h>
#include<netinet/in.h>
#include <unistd.h>
#include<string.h>



int main(int argument_number, char** argv)
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
    server_address.sin_port = htons(atoi(argv[2])); //send actual port no

    // send input IP Adress
    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
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
    //Handle the EOF (CTRL+D) (Test Case#);
    if(fgets(write,sizeof(write),stdin)==NULL)
    {
        printf("Exiting... Closing the Client socket Due to EOF.\n");
        //send(client_socket, write, strlen(write),0);
        break;
    }
    
    
    //Check if the user wants to terminate "exit" (Handling test Case#4)
    // Remove the newline character from the input (if present)
    write[strcspn(write, "\n")] = '\0';
    
    if (strcmp(write, "exit") == 0) {
    
        printf("Closing the Client socket due to Termination by user.\n");
        break;  // Exit the loop
    }
    // send packet to server if there is EOF or Exit
    send(client_socket, write, strlen(write),0);

    //Now listen for receieving Echo from server
    char echo_read[1024]={0};// allocating 0 to the memory indexes adress 
    int valread=read(client_socket, echo_read, 1024);
    printf("Recieved from Server: %s\n",echo_read);
    //printf("%s",write); 
}

printf("Closing the connection\n");
    close(client_socket);
    return 0;
}