/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 01:37:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/31 00:45:30 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <cstring>
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
	struct pollfd fds[500];
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
	
	while (!_endServer) {
		bool failedSck = false;
		std::cout << "server is running" << std::endl;
		// we wait for the incoming connection
		rc = poll(fds, nfds, 3000);
		// if the poll failed
		if (rc < 0)
		{
			perror("  poll() failed");
			break;
		}
		// else the poll timed out
		else if (rc == 0)
		{
			std::cout << "Poll timed out !" << std::endl;
			break;
		}
		current_size = nfds;
		int i;
		// we loop through the fds array to check if we have an incoming connection or a message
		for (i = 0; i < current_size; i++)
		{
			// if the revents is 0, then we continue
			if (fds[i].revents == 0)
				continue;
			// if the revents is not POLLIN, then we have an error
			if (fds[0].revents != POLLIN)
			{
				std::cout << "Error! revents = " << fds[i].revents << std::endl;
				_endServer = true;
				break;
			}
			// this is the part where we accept the incoming connection
			if (fds[0].revents == _listen_sd)
			{
				int newSck;
				do
				{
					newSck = accept(_listen_sd, NULL, NULL);
					// if the accept failed
					if (newSck < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							perror("  accept() failed");
							_endServer = true;
						}
						break;
					}
					std::cout << "New incoming connection - " << newSck << std::endl;
					fds[nfds].fd = newSck;
					fds[nfds].events = POLLIN;
					nfds++;
				} while (newSck != -1);
			}
			// this is the part where we read the message from the client
			else
  	  		{
  	  			bool close_conn = FALSE;
				// we read the message from the client
  	  			while (1)
  	  			{
  	  				rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					// if the recv failed
  	  			  	if (rc < 0)
  	  			  	{
  	  			    	if (errno != EWOULDBLOCK)
  	  			    	{
  	  			    	  	perror("  recv() failed");
  	  			    	  	close_conn = TRUE;
  	  			    	}
  	  			    	break;
  	  			  	}
					// else the connection is closed
  	  			  	else if (rc == 0)
  	  			  	{
  	  			  	  	printf("  Connection closed\n");
  	  			  	  	close_conn = TRUE;
  	  			  	  	break;
  	  			  	}
  	  			  	int len = rc;
					buffer[len] = '\0';
					std::cout << "message received: " << buffer << std::endl;
  	  			  	
					char msg[] = "Hello from server";
  	  			  	
					rc = send(fds[i].fd, msg, strlen(msg), 0);
					// if the send failed
					if (rc < 0)
  	  			  	{
  	  			  		perror("  send() failed");
  	  			  		close_conn = TRUE;
  	  			  		break;
  	  			  	}
  	  			}
				// if the connection is closed
				if (close_conn)
  	  			{
  	    	  		close(fds[i].fd);
  	    	  		fds[i].fd = -1;
  	    	  		failedSck = TRUE;
  	    		}
			}
  	  	}
		// compress the array if we have closed a socket
		if (failedSck) {
			failedSck = FALSE;
			for (i = 0; i < nfds; i++)
			{
				if (fds[i].fd == -1)
				{
					for(int j = i; j < nfds-1; j++)
						fds[j].fd = fds[j+1].fd;
					i--;
					nfds--;
				}
			}
		}
	}
	// close all the sockets
	for (int i = 0; i < nfds; i++)
  	{
    	if(fds[i].fd >= 0)
      	close(fds[i].fd);
  	}
  	return 0;
}







