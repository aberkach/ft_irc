/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverTest.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:48:46 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/26 22:28:04 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/_endian.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<stdlib.h>
#define CONNECTION_PORT 3500

int main()
{
    int socket_descriptor;
    int client_socket;
    char storage_buffer[80];
    int option_value = 1;
    struct sockaddr_in server_address;
    
    char* message = "This is a message from the server";
    socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_descriptor < 0)
   {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
   }     
    int status = setsockopt(socket_descriptor, SOL_SOCKET,SO_REUSEADDR , &option_value,sizeof(option_value));
    if (status  < 0){
        perror("Couldn't set options");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONNECTION_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    status = bind(socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
    if (status < 0){
        perror("Couldn't bind socket");
        exit(EXIT_FAILURE);
    }
    status = listen(socket_descriptor,4);
    if (status < 0){
        perror("Couldn't listen for connections");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        struct sockaddr_in connection_address;
        unsigned int length_of_address;

        printf("Server is listening for connections\n");
        client_socket = accept(socket_descriptor, (struct sockaddr*)&connection_address, &length_of_address);
        if (client_socket < 0){
            perror("Couldn't establish connection with client");
            exit(EXIT_FAILURE);
        }
        int bytes_read;

        while ((bytes_read = read(client_socket, storage_buffer, 79)) > 0) {
            
            storage_buffer[bytes_read] = '\0';
            printf("Message from client: %s \n",storage_buffer);
            send(client_socket, message, strlen(message), 0);
            
            if (bytes_read < 0){
                perror("Couldn't read from client");
                exit(EXIT_FAILURE);
            }
        }
    }
    close(socket_descriptor);
    close(client_socket);
    return 0;
}