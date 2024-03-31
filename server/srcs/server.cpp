/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 01:37:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/31 01:54:55 by abberkac         ###   ########.fr       */
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

	_listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (_listen_sd < 0)
	{
		perror("socket() failed");
		exit(-1);
	}
	
	if (setsockopt(_listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		perror("setsockopt() failed");
		close(_listen_sd);
		exit(-1);
	}
	
	if (fcntl(_listen_sd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("fcntl() failed");
		close(_listen_sd);
		exit(-1);
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	
	rc = bind(_listen_sd, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0)
	{
		perror("bind() failed");
		close(_listen_sd);
		exit(-1);
	}
	
	if (listen(_listen_sd, 32) < 0)
	{
		perror("listen() failed");
		close(_listen_sd);
		exit(-1);
	}
	
	memset(fds, 0 , sizeof(fds));
	fds[0].fd = _listen_sd;
	fds[0].events = POLLIN;
	
	std::map<int, int> clientsFds;
	std::cout << "server is running" << std::endl;
	while (true) {
		// we wait for the incoming connection
		rc = poll(fds, nfds, 30000);
		// if the poll failed
		if (rc <= 0)
			continue;
		current_size = nfds;
		// we loop through the fds array to check if we have an incoming connection or a message
		if (fds[0].revents & POLLIN)
		{
			int newSck;
			newSck = accept(_listen_sd, NULL, NULL);
			if (newSck < 0)
				perror("  accept() failed");
			else
			{
				std::cout << "New incoming connection - " << newSck << std::endl;
				fds[nfds].fd = newSck;
				fds[nfds].events = POLLIN;
				fcntl(newSck, F_SETFL, O_NONBLOCK);
				clientsFds.insert(std::pair<int, int>(newSck, newSck));
				nfds++;
			}
		}
		for (int i = 1; i < current_size; i++)
		{
			// this is the part where we read the message from the client
			if (fds[i].revents & POLLIN)
  	  		{
				// we read the message from the client
  	  			rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (rc < 0)
					perror("  recv() failed");
				else if (rc == 0) {
  	  			  	printf("  Connection closed\n");
					clientsFds.erase(fds[i].fd);
					close(fds[i].fd);
					fds[i].fd = -1;
				}
				else 
				{
					buffer[rc] = '\0';
					std::cout << "message received: " << buffer << std::endl;
				}
			}
  	  	}
		current_size = 0;
		for(int i = 0; i < nfds; i++)
		{
			if (fds[i].fd >= 0)
			{
				fds[current_size] = fds[i];
				current_size++;
			}
		}
		nfds = current_size;
	}
}







