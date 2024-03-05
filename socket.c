#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include<stdlib.h>

#define CONNECTION_PORT 8080
#define BUFFER_SIZE 1024

void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received;

    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    } else if (bytes_received == 0) {
        printf("Client disconnected\n");
        close(client_socket);
        return;
    }

    printf("Received HTTP request:\n%s\n", buffer);

    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
    send(client_socket, response, strlen(response), 0);

    close(client_socket);
}

int main(void){
    int server_socket;
    int client_socket;

    //message
    char storage_buffer[80];
    int length_of_address;

    int option_value;

    struct sockaddr_in server_addr;
    struct sockaddr_in connection_addr;

    //Check socket creation
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //Set and validate socket options
    int status=setsockopt(server_socket, SOL_SOCKET,SO_REUSEADDR , &option_value,sizeof(option_value));
    if(status<0){
        perror("Couldn't set options");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CONNECTION_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_zero[8] = '\0';
    
    status = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    if(status < 0){
        perror("Couldn't bind socket");
        exit(EXIT_FAILURE);
    }

    printf("Server binded to port %d\n", CONNECTION_PORT);

    status = listen(server_socket, 4);
    if(status < 0){
        perror("Couln't listen for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening to port %d\n", CONNECTION_PORT);

    length_of_address = sizeof(connection_addr);

    int new_socket = accept(server_socket, &connection_addr, &length_of_address);
    if(new_socket < 0){
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    handle_request(new_socket);

    return 0;
}