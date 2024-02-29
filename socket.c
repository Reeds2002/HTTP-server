#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include<stdlib.h>

#define CONNECTION_PORT 8080

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


    return 0;
}