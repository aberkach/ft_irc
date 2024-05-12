

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


std::vector<std::string> splitBySpace(std::string str)
{
    // split the string by the delim
    std::vector<std::string> tokens;
    std::string token;

	std::cout << "str : " << str << std::endl;
	bool isTwoPoints = false;
	for (size_t i = 0; i < str.length(); i++)
    {
		if (str[i] == ' ' && isTwoPoints == false)
		{
			if (token.empty())
				continue;
			tokens.push_back(token);
			token.clear();
		}
		else if (str[i] == ':' && isTwoPoints == false)
			isTwoPoints = true;
		else
            token += str[i];
    }
	tokens.push_back(token);
	for (size_t i = 0; i < tokens.size(); i++)
		std::cout << "token : " << tokens[i] << std::endl;
    return tokens;
}

std::vector<std::string> splitByDelim(std::string str, char delim)
{
    // split the string by the delim
    std::vector<std::string> tokens;
    std::string token;

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
	tokens.push_back(token);
    return tokens;
}

// parameterized constructor : initialize the server socket and set the port number
Server::Server(uint16_t port, char *password) : _port(port), _password(password)
{
	_pollFds.resize(1500);
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
	// memset(_pollFds, 0 , sizeof(_pollFds));
	_pollFds[0].fd = _listen_sd;
	_pollFds[0].events = POLLIN;
	_pollFds[0].revents = 0;
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
	        perror("  accept() failed");
		
	    else {
			// Resize the fds array if it's full
			if (_nfds >= _pollFds.size())
				_pollFds.resize(_pollFds.size() * 2);
	        // Add the new client socket to the fds array and clientsFds map
	        std::cout << "New incoming connection - " << newSck << std::endl;
			// std::cout << client_adrs.sin_addr.s_addr << std::endl;
	        fcntl(newSck, F_SETFL, O_NONBLOCK);
			_pollFds[_nfds].fd = newSck;
	        _pollFds[_nfds].events = POLLIN;
			_pollFds[_nfds].revents = 0;
			// Add the new client to the clients map
	        _clients.insert(std::pair<int, Client>(newSck, Client(newSck, client_adrs)));
	        _nfds++;
	    }
	}
}

void operatorFlag(Client &client, Channel &channel, bool sign, std::vector<std::string> args)
{
	if (sign)
	{
		if (args.empty())
			return ;//reply client already operator
		if (!channel.isClientExist(args[0]))
			return ; //reply client dosen't exist
		else{
			std::map<std::string, Client> it = channel.getUsers();
			if ((it.find(args[0])) != it.end())
				return ; //reply client already operator
			else{	
				channel.addOperator(it[args[0]]);
				args.erase(args.begin());
				//reply client is now operator
			}
		}
	}
	else{
		if (args.empty())
			return ;//reply client already operator
		if (!channel.isClientExist(args[0]))
			return ; //reply client dosen't exist
		else{
			std::map<std::string, Client> it = channel.getUsers();
			if ((it.find(args[0])) == it.end())
				return ; //reply client is not an operator
			else{	
				channel.removeOperator(it[args[0]]);
				args.erase(args.begin());
				//reply client is now just a user
			}
		}
	}
}

void	keyWordFlag(Channel &channel, bool sign, std::vector<std::string> args)
{
	if (sign)
	{
		if (args.empty())
			return ;//reply no key given
		channel.setKey(args[0]);
		args.erase(args.begin());
		//reply key is now set
	}
	else{
		channel.setKey("");
		//reply key is now removed
	}
}

void invetOnlyFlag(Channel &channel, bool sign)
{
	if (sign){
		channel.setIsInviteOnly(true);
		//reply channel is now invite only
	}
	else{
		channel.setIsInviteOnly(false);
		//reply channel is now not invite only
	}
}

void limitFlag(Channel &channel, bool sign, std::vector<std::string> args)
{
	if (sign)
	{
		if (args.empty())
			return ;//reply no limit given
		std::stringstream  ss(args[0]);
		size_t limit;
		if (!(ss >> limit) || !ss.eof())
			return ; //reply limit must be a number
		channel.setMaxUsers(limit);
		args.erase(args.begin());
		//reply limit is now set
	}
	else{
		channel.setMaxUsers(0);
		//reply limit is now removed
	}
}

void topicFlag(Channel &channel, bool sign, std::vector<std::string> args)
{
}

void Server::modeCommand(std::vector<std::string> fields, Client &client){
	bool sign = false;
	std::vector<std::string> args;
	int idx;

	if (fields[0].empty())
		return ; //reply no  channel name given
	if (fields[1].empty())
		return ; //reply no flags for mode
	chnMapIt it = _channels.find(fields[0]);
	if (it == _channels.end())
		return ; //reply channel dosen't exist
	if (!it->second.isClientExist(client.getNickname()))
		return ;  //reply user not on the channel
	if (!it->second.isOperator(client.getNickname()))
		return ;//reply is not an operator
	for(int i = 2; i < fields.size(); i++)
		if (!fields[i].empty())
			args.push_back(fields[i]);
	for(size_t i = 0; i < fields[1].size(); i++){
		if (fields[1][i] == '+')
			sign = true;
		else if (fields[1][i] == '-')
			sign = false;
		if (fields[1][i] == 'o')
			operatorFlag(client, it->second, sign, args);
		else if (fields[1][i] == 'k')
			keyWordFlag(it->second, sign, args);
		else if (fields[1][i] == 'i')
			invetOnlyFlag(it->second, sign);
		else if (fields[1][i] == 'l')
			limitFlag(it->second, sign, args);
		else if (fields[1][i] == 't')
			topicFlag(it->second, sign, args);
		else
			return ; //reply unknown mode flag
	}
}

// mode #channel +o 
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
	else if (command == "MODE")
		modeCommand(fields, user);
	else
		replyTo(user.getSocket(), ERR_UNKNOWNCOMMAND(user.getNickname(), command));
}


// Handle incoming data from clients :
void 
Server::handleIncomeData(int i) 
{
	char buffer[2048] = {0};
	int rc;

	rc = recv(_pollFds[i].fd, buffer, sizeof(buffer), 0);
	if (rc < 0)
		Err("recv() failed", 0);
	else if (rc == 0) {
		std::cout << "Connection closed" << std::endl;
		// Remove closed client from fds array and clientsFds map
		_clients.erase(_pollFds[i].fd);
		close(_pollFds[i].fd);
		_pollFds[i].fd = -1;
	}
	else {
		// here we handle the message
		buffer[rc] = '\0';
		std::string rec(buffer);
		// check if the message is valid (finished by \r\n)
		if (rec.find("\r") == std::string::npos && rec.find_first_of("\n") == std::string::npos)
		{
			_clients.find(_pollFds[i].fd)->second._clientBuffer += rec;
			return;
		}
		else
		{
			rec = _clients.find(_pollFds[i].fd)->second._clientBuffer + rec;
			_clients.find(_pollFds[i].fd)->second._clientBuffer.clear();
		}
		// remove the remove all spaces from the message (included \r\n)
		rec = trimTheSpaces(rec);
		// split the message by space
		
		std::vector<std::string> fields = splitBySpace(rec);
		if (!fields.empty())
		{
			fields[0] = stringUpper(fields[0]);
			commandList(rec ,fields, _clients.find(_pollFds[i].fd)->second);
		
			_clients.find(_pollFds[i].fd)->second.refStatus();
		}
	}
}
// lFTmZkkDWY
//zillow

int Server::createServer() 
{
	int		current_size;
	int		rc;
	_nfds = 1;

	// Start listening for incoming connections
	std::cout << "server is running : " << std::endl;
	while (true) {
	    // Wait for events on monitored file descriptors
	    rc = poll(_pollFds.data(), _nfds, 0);

	    // If poll failed or timeout occurred, continue to the next iteration
	    if (rc == 0)
	        continue;
		if (rc < 0) {
	        perror("  poll() failed");
	        continue;
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

Server::~Server(void) {
	close(_listen_sd);
}