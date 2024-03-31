/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 01:37:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/31 18:08:15 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <cstring>
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
	_endServer = false;
	_clients.clear();
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

int Server::createServer() 
{
	char   buffer[80];
	struct sockaddr_in   addr;
	struct pollfd fds[100000];
	int    nfds = 1;
	int    current_size = 0;
	int on = 1;
	int rc;

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
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	rc = bind(_listen_sd, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0) {
	    perror("bind() failed");
	    close(_listen_sd);
	    exit(-1);
	}
	
	// Start listening for incoming connections
	if (listen(_listen_sd, 32) < 0) {
	    perror("listen() failed");
	    close(_listen_sd);
	    exit(-1);
	}
	
	// Initialize the fds array with the server socket
	memset(fds, 0 , sizeof(fds));
	fds[0].fd = _listen_sd;
	fds[0].events = POLLIN;
	
	std::map<int, int> clientsFds; // Map to store client file descriptors
	
	std::cout << "server is running" << std::endl;
	while (true) {
	    // Wait for events on monitored file descriptors
	    rc = poll(fds, nfds, 30000);
	
	    // If poll failed or timeout occurred, continue to the next iteration
	    if (rc <= 0)
	        continue;
		
	    current_size = nfds;
	
	    // Check for incoming connection on the server socket
	    if (fds[0].revents & POLLIN) {
	        int newSck;
	        newSck = accept(_listen_sd, NULL, NULL);
	        if (newSck < 0)
	            perror("  accept() failed");
	        else {
	            // Add the new client socket to the fds array and clientsFds map
	            std::cout << "New incoming connection - " << newSck << std::endl;
	            fds[nfds].fd = newSck;
	            fds[nfds].events = POLLIN;
	            fcntl(newSck, F_SETFL, O_NONBLOCK);
	            clientsFds.insert(std::pair<int, int>(newSck, newSck));
	            nfds++;
	        }
	    }
	
	    // Iterate through fds array to check for messages from clients
	    for (int i = 1; i < current_size; i++) {
	        if (fds[i].revents & POLLIN) {
	            // Receive message from client
	            rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
	            if (rc < 0)
	                perror("  recv() failed");
	            else if (rc == 0) {
	                printf("  Connection closed\n");
	                // Remove closed client from fds array and clientsFds map
	                clientsFds.erase(fds[i].fd);
	                close(fds[i].fd);
	                fds[i].fd = -1;
	            }
	            else {
	                buffer[rc] = '\0';
	                std::cout << "message received: " << buffer << std::endl;
	            }
	        }
	    }
	
	    // Compact the fds array to remove closed client sockets
	    current_size = 0;
	    for (int i = 0; i < nfds; i++) {
	        if (fds[i].fd >= 0) {
	            fds[current_size] = fds[i];
	            current_size++;
	        }
	    }
	    nfds = current_size;
	}
}







