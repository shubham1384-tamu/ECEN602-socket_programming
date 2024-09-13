#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>

#define BUFFER_SIZE 1024

void sigchld_handler(int s) {
    // Wait for any child process
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argument_number, char** port_number) {
    int server_socket, client_socket;
    pid_t child_id;

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(0);
    }

    // Define server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port_number[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        exit(0);
    }

    // Listen for incoming connections
    listen(server_socket, 5);
    printf("Server started, listening on port %s\n", port_number[1]);

    // Handle SIGCHLD to prevent zombie processes
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while (1) {
        socklen_t addrlen = sizeof(server_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&server_addr, &addrlen);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Fork a child process to handle the client
        if ((child_id = fork()) == 0) {
            // Child process
            close(server_socket);  // Child doesn't need the listening socket
            char buffer[BUFFER_SIZE] = {0};
            int valread;

            while ((valread = read(client_socket, buffer, sizeof(buffer))) > 0) {
                printf("Read from client: %s\n", buffer);
                send(client_socket, buffer, strlen(buffer), 0);  // Echo back
            }

            if (valread == 0) {
                printf("Client disconnected\n");
            }

            close(client_socket);  // Close the socket
            exit(0);
        } else {
            // Parent process
            close(client_socket);  // Parent doesn't need the client socket
        }
    }

    close(server_socket);
    return 0;
}
