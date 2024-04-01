/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 01:37:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/01 01:23:30 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <cctype>
#include <cstring>
#include <string>
#include <sys/fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>



#define TRUE             1
#define FALSE            0


Server::Server() {
	_port = 0;
	_password = "";
	_listen_sd = -1;
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

// void Server::updateFileDescrior(int *nfds)
// {
// 	if (_fds)
// 		delete[] _fds;
// 	_fds = new struct pollfd((*nfds) + 1);
// }

int Server::createServer() 
{
	char   buffer[1024];
	
	int    current_size = 0;
	int on = 1;
	int rc;

	nfds = 1;
	_listen_sd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
	if (_listen_sd < 0) {
	    perror("socket() failed");
	    exit(-1);
	}
	
	// Set socket option to allow address reuse
	if (setsockopt(_listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
	    perror("setsockopt() failed");
	    close(_listen_sd);
	    exit(-1);
	}
	
	// Set socket to non-blocking mode
	if (fcntl(_listen_sd, F_SETFL, O_NONBLOCK) < 0) {
	    perror("fcntl() failed");
	    close(_listen_sd);
	    exit(-1);
	}
	
	// Bind the socket to the specified address and port
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);
	
	rc = bind(_listen_sd, (struct sockaddr *)&_addr, sizeof(_addr));
	if (rc < 0) {
	    perror("bind() failed");
	    close(_listen_sd);
	    exit(-1);
	}
	
	// Start listening for incoming connections
	if (listen(_listen_sd, 128) < 0) {
	    perror("listen() failed");
	    close(_listen_sd);
	    exit(-1);
	}
	
	// Initialize the fds array with the server socket
	memset(_fds, 0 , sizeof(_fds));
	_fds[0].fd = _listen_sd;
	_fds[0].events = POLLIN;
	
	std::map<int, int> clientsFds; // Map to store client file descriptors
	
	std::cout << "server is running" << std::endl;
	while (true) {
	    // Wait for events on monitored file descriptors
	    rc = poll(_fds, nfds, 0);
	
	    // If poll failed or timeout occurred, continue to the next iteration
	    if (rc <= 0)
	        continue;
		
	    current_size = nfds;
		int sock_len = sizeof(_addr);
	
	    // Check for incoming connection on the server socket
	    if (_fds[0].revents == POLLIN) {
			int newSck = accept(_listen_sd, (struct sockaddr *)&_addr, (socklen_t*)&sock_len);
	        if (newSck < 0)
	            perror("  accept() failed");
	        else {
	            // Add the new client socket to the fds array and clientsFds map
	            std::cout << "New incoming connection - " << newSck << std::endl;
	            _fds[nfds].fd = newSck;
	            _fds[nfds].events = POLLIN;
	            fcntl(newSck, F_SETFL, O_NONBLOCK);
	            clientsFds.insert(std::pair<int, int>(newSck, newSck));
	            nfds++;
	        }
	    }
	
	    // Iterate through fds array to check for messages from clients
	    for (int i = 1; i < current_size; i++) {
	        if (_fds[i].revents & POLLIN) {
	            // Receive message from client
	            rc = recv(_fds[i].fd, buffer, (sizeof(buffer) - 1), 0);
	            if (rc < 0)
	                perror("  recv() failed");
	            else if (rc == 0) {
	                std::cout << "Connection closed" << i << std::endl;
	                // Remove closed client from fds array and clientsFds map
	                clientsFds.erase(_fds[i].fd);
	                close(_fds[i].fd);
	                _fds[i].fd = -1;
	            }
	            else {
	                buffer[rc] = '\0';
					std::string message(buffer);
					// here we can do whatever we want with the message
					//........
	                std::cout << message << std::endl;
	            }
	        }
	    }
	
	    // Compact the fds array to remove closed client sockets
	    current_size = 0;
	    for (int i = 0; i < nfds; i++) {
	        if (_fds[i].fd >= 0) {
	            _fds[current_size] = _fds[i];
	            current_size++;
	        }
	    }
	    nfds = current_size;
	}
}







