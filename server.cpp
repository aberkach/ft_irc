/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 01:37:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/21 02:19:53 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <exception>
#include <stdexcept>

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

//"Password must be at least 8 characters long"