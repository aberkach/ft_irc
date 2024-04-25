/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:49:52 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/25 18:24:14 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Inc/ft_irc.hpp"
#include "server.hpp"
#include <cstddef>
#include <cstdlib>
#include <arpa/inet.h> // print ip adrss
#include <iostream>
#include <sys/poll.h>
#include "../client/client.hpp"
#include "../channel/channel.hpp"

void Err(std::string msg, int exitFalg)
{
	std::cerr << msg << std::endl;
	if (exitFalg)
		exit(1);
}

std::string trimTheSpaces(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\v\b\r\n");
    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(" \t\v\b\r\n");
    return str.substr(first, (last - first + 1));
}

std::string stringUpper(const std::string &_str)
{
	std::string upper(_str);

    for (std::string::size_type i = 0; i < _str.size(); ++i)
        upper[i] = ::toupper(_str[i]);

	return(upper);
}


std::vector<std::string> splitByDelim(std::string str, char delim)
{
    // split the string by the delim
    std::vector<std::string> tokens;
    std::string token;

	if (delim != ' ') {
    	for (size_t i = 0; i < str.length(); i++)
    	{
    	    if (str[i] == delim)
    	    {
				// skip the delim
				if (token.empty())
					continue;
    	        tokens.push_back(token);
    	        token.clear();
    	    }
    	    else
    	        token += str[i];
    	}
	}
	else {
		for (size_t i = 0; i < str.length(); i++)
    	{
			if (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' || str[i] == '\b' || str[i] == '\r' || str[i] == '\n')
    	    {
				// skip the delim
				if (token.empty())
					continue;
    	        if (token[0] == ':')
		        {
		            token.erase(0, 1);
		            token += str.substr(i);
					tokens.push_back(token);
					continue;
		        }
		        tokens.push_back(token);
		        token.clear();
    	    }
			else
    	        token += str[i];
    	}
	}
    tokens.push_back(token);
    return tokens;
}

// parameterized constructor : initialize the server socket and set the port number
Server::Server(uint16_t port, char *password) : _port(port), _password(password)
{
	_fds.resize(1500);
	_listen_sd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
	if (_listen_sd < 0)
		Err("socket() failed", 1);

	// Set socket option to allow address reuse
	int on = 1;
	if (setsockopt(_listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
	    close(_listen_sd);
		Err("setsockopt() failed", 1);
	}
	
	// Set socket to non-blocking mode
	if (fcntl(_listen_sd, F_SETFL, O_NONBLOCK) < 0) {
	    close(_listen_sd);
		Err("fcntl() failed", 1);
	}
	
	// Bind the socket to the specified address and port
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);

	if (bind(_listen_sd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0) {
	    close(_listen_sd);
		Err("bind() failed", 1);
	}

	// Start listening for incoming connections
	if (listen(_listen_sd, 128) < 0) {
	    perror("listen() failed");
	    close(_listen_sd);
	    exit(-1);
	}
	
	// Initialize the fds array with the server socket
	// memset(_fds, 0 , sizeof(_fds));
	_fds[0].fd = _listen_sd;
	_fds[0].events = POLLIN;
	_fds[0].revents = 0;
}

// Setters ::-----------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------

// Handle incoming connections:
void Server::handlIncomeConnections() 
{
	if (_fds[0].revents == POLLIN)
	{
		struct sockaddr_in client_adrs;
		socklen_t sock_len = sizeof(client_adrs);
		memset(&client_adrs, 0, sock_len);

		int newSck = accept(_listen_sd, (struct sockaddr *)&client_adrs, &sock_len);
	    if (newSck < 0)
	        perror("  accept() failed");
		
	    else {
			// Resize the fds array if it's full
			if (_nfds >= _fds.size())
				_fds.resize(_fds.size() * 2);
	        // Add the new client socket to the fds array and clientsFds map
	        std::cout << "New incoming connection - " << newSck << std::endl;
			// std::cout << client_adrs.sin_addr.s_addr << std::endl;
	        fcntl(newSck, F_SETFL, O_NONBLOCK);
			_fds[_nfds].fd = newSck;
	        _fds[_nfds].events = POLLIN;
			_fds[_nfds].revents = 0;
			// Add the new client to the clients map
	        _clients.insert(std::pair<int, Client>(newSck, Client(newSck, client_adrs)));
	        _nfds++;
	    }
	}
}


void Server::commandList(const std::string& message, std::vector<std::string> &fields, Client &user)
{
	std::string command(fields[0]);
	fields.erase(fields.begin());

	if (command.empty())
		return;
	else if (command == "PASS")
		passCommand(fields, user);
	else if (command == "NICK")
		nickCommand(fields, user);
	else if (command == "USER")
		userCommand(message, fields, user);
	else if (command == "PRIVMSG")
		privmsgCommand(message, fields, user);
	else if (command == "JOIN")
		joinCommand(fields, user);
	else if (command == "QUIT")
		quitCommand(fields, user);
	else if (command == "KICK")
		kickCommand(fields, user);
	else if (command == "PONG")
	{
		if (!fields.empty())
			replyTo(user.getSocket(), fields[0]);
		replyTo(user.getSocket(), ERR_NEEDMOREPARAMS(user.getNickname(), command));
	}
	else if (command == "TOPIC") {
		topicCommand(fields, user);
	}
	else if (command == "PART")
		partCommand(fields, user);
	else if (command == "LIST")
		listCommand(fields, user);
	else if (command == "INVITE")
		inviteCommand(fields, user);
	else
		replyTo(user.getSocket(), ERR_UNKNOWNCOMMAND(user.getNickname(), command));
}


// Handle incoming data from clients :
void 
Server::handleIncomeData(int i) 
{
	char buffer[1024] = {0};
	int rc;

	rc = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
	if (rc < 0)
		Err("recv() failed", 0);
	else if (rc == 0) {
		std::cout << "Connection closed" << std::endl;
		// Remove closed client from fds array and clientsFds map
		_clients.erase(_fds[i].fd);
		close(_fds[i].fd);
		_fds[i].fd = -1;
	}
	else {
		// here we handle the message
		buffer[rc] = '\0';
		std::string rec(buffer);
		// remove the remove all spaces from the message (included \r\n)
		rec = trimTheSpaces(rec);
		// split the message by space
		std::vector<std::string> fields = splitByDelim(rec, ' ');
		if (!fields.empty())
		{
			fields[0] = stringUpper(fields[0]);
			commandList(rec ,fields, _clients.find(_fds[i].fd)->second);
			// if (fields[0] == "QUIT")
			// {
			// 	_clients.erase(_fds[i].fd);
			// 	close(_fds[i].fd);
			// 	_fds[i].fd = -1;
			// }
			_clients.find(_fds[i].fd)->second.refStatus();
		}
	}
}

int Server::createServer() 
{
	int		current_size;
	int		rc;
	_nfds = 1;

	// Start listening for incoming connections
	std::cout << "server is running : " << std::endl;
	while (true) {
	    // Wait for events on monitored file descriptors
	    rc = poll(_fds.data(), _nfds, 0);

	    // If poll failed or timeout occurred, continue to the next iteration
	    if (rc == 0)
	        continue;
		if (rc < 0) {
	        perror("  poll() failed");
	        continue;
	    }
		for (size_t i = 0; i < _nfds; i++) {
			if (_fds[i].revents & POLLIN) {
	    		// Check for incoming connection on the server socke
				if (_fds[i].fd == _listen_sd)
					handlIncomeConnections();
				else
				    // Iterate through fds array to check for messages from clients
					handleIncomeData(i);
			}
	    }
	
	    // Compact the fds array to remove closed client sockets
	    current_size = 0;
	    for (size_t i = 0; i < _nfds; i++) {
	        if (_fds[i].fd >= 0) {
	            _fds[current_size] = _fds[i];
	            current_size++;
	        }
	    }
	    _nfds = current_size;
	}
}

Server::~Server(void) {
	close(_listen_sd);
}