/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 20:20:32 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/21 02:21:53 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include "server.hpp"


#define PORT 8080
struct sockaddr_in server;


int createServerTest() {
    int nRet = 0;
    // initialize the socket :
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        std::cout << "Error creating socket" << std::endl;
        return 1;
    } else
        std::cout << "Socket created" << std::endl;
    // initialize the server struct :
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
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
    return 0;
}

bool isJustDigits(const std::string& str) {
    for (size_t i = 0; i < str.length(); ++i)  {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}

bool checkArgs(char **av) {
    if (isJustDigits(av[1]))
        return true;
    else {
        std::cerr << "Invalid port number" << std::endl;
        return false;
    }
    return true;
}

int main(int ac, char **av) {
    
    Server srv;
    try {
        if (ac == 3) {
            if (checkArgs(av)) {
                srv.setPort(std::stoi(av[1]));
                srv.setPassword(av[2]);
            }
        }
        else {
            std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
            return 1;
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}