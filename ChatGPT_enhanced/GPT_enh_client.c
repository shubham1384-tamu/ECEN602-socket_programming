#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <unistd.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argument_number, char** argv)
{
    int client_socket;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return 0;
    }

    // Address structure for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[2])); // Convert and set port

    // Input IP Address
    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return -1;
    }

    if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Error making connection");
        return -1;
    }

    printf("Connection to the server successful\n");

    while (1) {
        char write[1024];

        // Handle EOF (Test Case 3)
        if (fgets(write, sizeof(write), stdin) == NULL) {
            printf("Exiting... Closing the Client socket due to EOF.\n");
            break;
        }

        // Remove the newline character from the input
        write[strcspn(write, "\n")] = '\0';

        // Handle "exit" command (Test Case 4)
        if (strcmp(write, "exit") == 0) {
            printf("Closing the Client socket due to termination by user.\n");
            break;
        }

        // Send packet to the server
        if (send(client_socket, write, strlen(write), 0) < 0) {
            perror("Error sending message");
            break;
        }

        // Receive echo from the server
        char echo_read[1024] = {0}; // Initialize buffer to zero
        int valread = read(client_socket, echo_read, sizeof(echo_read));

        if (valread < 0) {
            perror("Error receiving message");
            break;
        }

        printf("Received from Server: %s\n", echo_read);
    }

    printf("Closing the connection\n");
    close(client_socket);
    return 0;
}
