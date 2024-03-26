/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 01:37:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/25 20:04:49 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <sys/socket.h>


Server::Server() {
    return;
}

Server::~Server() {
    return;
}

void Server::setPort(unsigned int port) {
    if (port > 1023 && port <= 65535)
        _port = port;
    else
        throw std::logic_error("Port number must be between 1024 and 65535");
}

void Server::setPassword(char *password) {
    if (strlen(password) >= 8)
        _password = password;
    else
        throw std::logic_error("Password must be at least 8 characters long");
}

int Server::createTestServer() {
    int nRet = 0;
    // initialize the socket :
    int sock;
    struct sockaddr_in server;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        std::cout << "Error creating socket" << std::endl;
        return 1;
    } else
        std::cout << "Socket created" << std::endl;
    // initialize the server struct :
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);
    server.sin_addr.s_addr = INADDR_ANY;
    std::memset(&(server.sin_zero), 0, 8);

    // bind the socket to the server :
    nRet = bind(sock, (struct sockaddr *)&server, sizeof(server));
    if (nRet == -1) {
        std::cout << "Error binding socket" << std::endl;
        return 1;
    } else
        std::cout << "Socket binded" << std::endl;
    // listen to the socket :
    nRet = listen(sock, 5);
    if (nRet == -1) {
        std::cout << "Error listening to socket" << std::endl;
        return 1;
    } else
        std::cout << "Listening to socket" << std::endl;
    int acc = accept(sock, NULL, NULL);
    if (acc == -1) {
        std::cout << "Error accepting connection" << std::endl;
        return 1;
    } else
        std::cout << "Connection accepted" << std::endl;
    return 0;
}
