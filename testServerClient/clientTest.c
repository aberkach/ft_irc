/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientTest.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 23:55:52 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/26 22:28:28 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/_endian.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<stdlib.h>
#define CONNECTION_PORT 3500

int main(int ac, char **av)
{
    struct sockaddr_in server_address;
    if (ac != 2)
    {
        printf("Usage: %s <message>\n", av[0]);
        exit(EXIT_FAILURE);
    }
    char* message = av[1];
    char recieve_buffer[100];
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket_descriptor < 0)
   {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
   }  
    int status = 0;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONNECTION_PORT);
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    status = connect(socket_descriptor,(struct sockaddr*)&server_address, sizeof(server_address));
    if(status < 0)
   {
        perror("Couldn't connect with the server");
        exit(EXIT_FAILURE);
   }    
    write(socket_descriptor, message, strlen(message));
    read(socket_descriptor, recieve_buffer, 100);
    printf("Message from server: %s\n", recieve_buffer);
    close(socket_descriptor);
    return 0;
}
