
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:49:52 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/27 20:49:48 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// + t adds rights -t removes rights
// can  only call mode if u an operator with 3 argument 


// if not operator u can only mode #channel for info on the modes in channel


#include "server.hpp"
#include "../../Inc/ft_irc.hpp"
#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <sys/poll.h>
#include <vector>
#include <algorithm>
#include <sys/signal.h>

bool Server::_signal = false;

// parameterized constructor : initialize the server socket and set the port number
Server::Server(uint16_t port, char *password) : _countCli(0), _port(port), _password(password)
{
	_pollFds.resize(1);
	_listen_sd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
	if (_listen_sd < 0)
		throw std::runtime_error("socket() failed");

	// Set socket option to allow address reuse
	int on = 1;
	if (setsockopt(_listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
	    close(_listen_sd);
		throw std::runtime_error("setsockopt() failed");
	}
	
	// Set socket to non-blocking mode
	if (fcntl(_listen_sd, F_SETFL, O_NONBLOCK) < 0) {
	    close(_listen_sd);
		throw std::runtime_error("fcntl() for the server is failed");
	}
	
	// Bind the socket to the specified address and port
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET; // Address family for IPv4
	_addr.sin_addr.s_addr = INADDR_ANY; // Listen on any IP address
	_addr.sin_port = htons(_port); // Listen on the specified port

	if (bind(_listen_sd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0) {
	    close(_listen_sd);
		throw std::runtime_error("bind() failed");
	}

	// Start listening for incoming connections
	if (listen(_listen_sd, 10) < 0) 
		throw std::runtime_error("listen() failed");
	
	// Initialize the fds array with the server socket
	_pollFds[0].fd = _listen_sd;
	_pollFds[0].events = POLLIN;
	_pollFds[0].revents = 0;

	// Initialize the commands map
	_commands["PASS"] = &Server::passCommand; // working full
	_commands["USER"] = &Server::userCommand;  // working full
	_commands["PING"] = &Server::pingCommad; // working full
	_commands["PONG"] = &Server::pongCommad; // working full
	_commands["NAMES"] = &Server::namesCommad; // working full
	_commands["PRIVMSG"] = &Server::privmsgCommand; // working full
	_commands["TOPIC"] = &Server::topicCommand; // working full
	_commands["QUIT"] = &Server::quitCommand; // working full
	_commands["LIST"] = &Server::listCommand; // working full
	_commands["PART"] = &Server::partCommand; // working full

	_commands["MODE"] = &Server::modeCommand;
	_commands["KICK"] = &Server::kickCommand;
	_commands["NICK"] = &Server::nickCommand;
	_commands["INVITE"] = &Server::inviteCommand;
	_commands["JOIN"] = &Server::joinCommand;
	// nick changes might couse a prob
	// need to make all channel related args lowercase
}

// create the server and handle the incoming connections and data
void Server::createServer() 
{
	int		current_size;
	_nfds = 1;
	int rc = 0;

	// signal handling
    signal(SIGINT, Server::sigHandler);
    signal(SIGQUIT, Server::sigHandler);
	// Start listening for incoming connections
	std::cout << "server is running : " << std::endl;
	while (_signal == false) {
		// Check if the server is shutting down by signal
		if (Server::_signal)
			throw std::runtime_error("Server is shutting down");
		rc = poll(&_pollFds[0], _pollFds.size(), -1);
	    if (rc < 0 && _signal == false) {
	        throw std::runtime_error("poll() failed");
		}
		for (size_t i = 0; i < _nfds; i++) {
			if (_pollFds[i].revents & POLLIN) 
			{
	    		// Check for incoming connection on the server socket
				if (_pollFds[i].fd == _listen_sd)
					handlIncomeConnections();
				else
					handleIncomeData(i);
			}
	    }
	    // Compact the fds array to remove closed client sockets
	    current_size = 0;
	    for (size_t i = 0; i < _nfds; i++) {
	        if (_pollFds[i].fd >= 0) {
	            _pollFds[current_size] = _pollFds[i];
	            current_size++;
	        }
	    }
	    _nfds = current_size;
	}
}

// Handle incoming connections:
void Server::handlIncomeConnections() 
{
	if (_pollFds[0].revents == POLLIN)
	{
		struct sockaddr_in client_adrs;
		socklen_t sock_len = sizeof(client_adrs);
		memset(&client_adrs, 0, sock_len);

		int newSck = accept(_listen_sd, (struct sockaddr *)&client_adrs, &sock_len);
	    if (newSck < 0)
	        Err("accept() failed");
		
	    else {
			// Resize the fds array if it's full
			if (_nfds >= _pollFds.size())
				_pollFds.resize(_pollFds.size() + 2);
	        // Add the new client socket to the fds array and clientsFds map
	        std::cout << GREEN << "New incoming connection : " << YELLOW <<  newSck << RESET << std::endl;
	        if (fcntl(newSck, F_SETFL, O_NONBLOCK))
				Err("fcntl() for the client is failed");
	        _pollFds[_nfds].fd = newSck;
	        _pollFds[_nfds].events = POLLIN;
			_pollFds[_nfds].fd = newSck;
	        _pollFds[_nfds].events = POLLIN;
			_pollFds[_nfds].revents = 0;
			// Add the new client to the clients map
	        _clients.insert(std::pair<int, Client>(newSck, Client(newSck, client_adrs)));
	        _nfds++;
			_countCli++;
	    }
	}
}


// run the correct command 
void Server::commandRunner(std::vector<std::string> &fields, Client &user)
{
	std::string command(fields[0]);
	fields.erase(fields.begin());

	if (_commands.find(command) != _commands.end())
		(this->*_commands[command])(fields, user);
	else
		replyTo(user.getSocket(), ERR_UNKNOWNCOMMAND(user.getNickname(), command));
}

std::vector<std::string>
Server::getBuffers(const std::string &buffer) {
    std::vector<std::string> messages;
    std::string::size_type start = 0, end = 0;

	while ((end = buffer.find_first_of('\n',start)) != std::string::npos) {
		while (buffer[end] && buffer[end] == '\n')
			end++;
		std::string message = buffer.substr(start, end - start);
		messages.push_back(message);
		start = end;
	}

    if (start < buffer.length()) {
        messages.push_back(buffer.substr(start));
    }

    return messages;
};

// Handle incoming data from clients :
void 
Server::handleIncomeData(int i) 
{
	char buffer[4096] = {0};
	int rc;

	rc = recv(_pollFds[i].fd, buffer, sizeof(buffer) - 1, 0);
	if (rc < 0)
		Err("recv() failed");
	else if (rc == 0) {
		std::cout << RED << "Connection closed For : " << YELLOW << _pollFds[i].fd << RESET << std::endl;
		// Remove closed client from fds array and clientsFds map)
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		{
			Client client = _clients.find(_pollFds[i].fd)->second;
			std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
			std::string quitMessage = QUIT_MSG(client.getNickname(), client.getUsername(), clientHost, "Forced quit");			
			it->second.broadCast(quitMessage, _pollFds[i].fd);
			it->second.removeUser(_clients.find(_pollFds[i].fd)->second);
		}
		_clients.erase(_pollFds[i].fd);
		close(_pollFds[i].fd);
		_pollFds[i].fd = -1;
		_countCli--;
	}
	else {
		// here we handle the message
		buffer[rc] = '\0';
		std::string rec(buffer);
		std::replace(rec.begin(), rec.end(), '\r', '\n');

		if (rec.find_first_of('\n') == std::string::npos) {
			_clients.find(_pollFds[i].fd)->second._clientBuffer += rec;
			return;
		}
		else
		{
			rec = _clients.find(_pollFds[i].fd)->second._clientBuffer + rec;
			_clients.find(_pollFds[i].fd)->second._clientBuffer.clear();
		}
		// // remove the remove all spaces from the message (included \r\n)
		std::vector<std::string> messages = getBuffers(rec);
		for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
			if ((*it = trimTheSpaces(*it)) == "")
				return;
			// split the message by space
			std::vector<std::string> fields = splitBySpace(*it);
			if (!fields.empty())
			{
				fields[0] = stringUpper(fields[0]);
				commandRunner(fields, _clients.find(_pollFds[i].fd)->second);
			}
		}
	}
}

Server::~Server(void) {
	close(_listen_sd);
}