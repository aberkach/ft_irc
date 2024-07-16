

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


#include "server.hpp"
#include "../../Inc/ft_irc.hpp"
#include <array>
#include <cstddef>
#include <string>
#include <sys/poll.h>
#include <vector>

bool Server::_signal = false;

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
	_pollFds.resize(30);
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

	// Initialize the commands map

	_commands["PASS"] = &Server::passCommand; 
	_commands["NICK"] = &Server::nickCommand;
	_commands["USER"] = &Server::userCommand;
	_commands["PRIVMSG"] = &Server::privmsgCommand;
	_commands["JOIN"] = &Server::joinCommand;
	_commands["QUIT"] = &Server::quitCommand;
	_commands["KICK"] = &Server::kickCommand;
	_commands["TOPIC"] = &Server::topicCommand;
	_commands["INVITE"] = &Server::inviteCommand;
	_commands["PART"] = &Server::partCommand;
	_commands["LIST"] = &Server::listCommand;
	_commands["MODE"] = &Server::modeCommand;
}


void	Server::sigHandler(int sigNumber)
{
	(void)sigNumber;
	Server::_signal = true;
	std::cout << std::endl;
}

void Server::cleanUp()
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << "Closing socket " << it->first << std::endl;
		close(it->first);
	}
	if (_listen_sd > 0)
	{
		std::cout << "Closing server socket " << _listen_sd << std::endl;
		close(_listen_sd);
	}
}

size_t Server::countUsersInChannel(std::string &chnName)
{
	chnMapIt it = _channels.find(chnName);
	if (it == _channels.end())
		return 0;
	return it->second.getUsers().size();
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

void topicFlag(Channel &channel, bool sign, std::vector<std::string> args)
{
	(void) args;
	if (sign)
		channel.setTopicFlag(true);
	else
		channel.setTopicFlag(false);
}

void operatorFlag(Client &client, chnMapIt &channel, bool sign, std::vector<std::string>& args)
{
	std::map<std::string, Client> it = channel->second.getUsers();
	if (sign)
	{
		if (it.find(args[0]) == it.end())
			replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), args[0], channel->first));
		else if (channel->second.isOperator(args[0]))
			replyTo(client.getSocket(), RPL_WHOISOPERATOR(args[0]));
		else{	
			channel->second.addOperator(it[args[0]]);
			args.erase(args.begin());
			replyTo(client.getSocket(), RPL_YOUREOPER(client.getNickname()));
		}
	}
	else{
		if ((it.find(args[0])) == it.end())
			replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), args[0], channel->first));
		else {	
			channel->second.removeOperator(it[args[0]]);
			args.erase(args.begin()); //reply client is now just a user
		}
	}
}

void	keyWordFlag(Channel &channel, bool sign, std::vector<std::string>& args)
{
	if (sign)
	{
		channel.setKey(args[0]);
		args.erase(args.begin()); //reply key is  set
	}
	else
		channel.setKey(""); //reply key is  removed
}

void invetOnlyFlag(Channel &channel, bool sign)
{
	if (sign)
		channel.setIsInviteOnly(true); //reply channel is now invite only
	else
		channel.setIsInviteOnly(false); //reply channel is now not invite only
}

void limitFlag(Channel &channel, bool sign, std::vector<std::string>& args)
{
	if (sign)
	{
		std::stringstream  ss(args[0]);
		size_t limit;
		if (!(ss >> limit) || !ss.eof()){
			args.erase(args.begin());
			return ; //reply limit must be a number
		}
		channel.setMaxUsers(limit);
		args.erase(args.begin());
		//reply limit is now set
	}
	else{
		channel.setMaxUsers(0);
		//reply limit is now removed
	}
}


int check_flag_string(std::string flags){
	if (flags[0] != '+' && flags[0] != '-')
		return 1;
	for (size_t i = 0; i < flags.size(); i++){
		for (size_t j = i + 1; j < flags.size(); j++)
			if ((flags[i] == flags[j] && i != j) || 
				(flags[i] != 'k' && flags[i] != 'o' && flags[i] != 'i' && flags[i] != 'l' && flags[i] != 't' && flags[i] != '+' && flags[i] != '-'))
				return 1;
	}
	return 0;
}

int check_params(std::vector<std::string> args, std::string flags){
	bool sign = false;
	size_t count = 0;
	for(size_t i = 0; i < flags.size(); i++){
		if (flags[i] == '+')
			sign = true;
		else if (flags[i] == '-')
			sign = false;
		if (flags[i] == 'o')
			count++;
		else if (flags[i] == 'k' && sign == true)
			count++;
		else if (flags[i] == 'l' && sign == true)
			count++;
	}
	if (count != args.size())
		return 1;
	return 0;
}

void display_channel_mode(Channel channel, Client &client){
	std::string str;
	str += "+";
	if (channel.getIsInviteOnly() == true)
		str += "i";
	if (!channel.getKey().empty())
		str += "k";
	if (channel.getMaxUsers() != 0)
		str += "l";
	if (!channel.getTopic().empty())
		str += "t";
	replyTo(client.getSocket(), RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), str));
}

std::string get_host(struct sockaddr_in &addr)
{
	std::stringstream ss;
    ss << inet_ntoa(addr.sin_addr);
    return ss.str();
}

void Server::modeCommand(std::vector<std::string> &fields, Client &client){
	bool sign = false;
	std::vector<std::string> args;
	
	if (fields[0].empty())
		replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
	chnMapIt it = _channels.find(fields[0]);
	if (it == _channels.end()){
		replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(client.getNickname(), fields[0]));
		return ;
	}
	if (!it->second.isClientExist(client.getNickname())){
		replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), client.getNickname(), it->first));
		return ;
	}
	if (fields.size() == 1){
		display_channel_mode(it->second, client);
		return ; 
	}
	if (!it->second.isOperator(client.getNickname())){
		replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), it->first));
		return ;
	}
	if (check_flag_string(fields[1]) == 1){
		replyTo(client.getSocket(), ERR_UNKNOWNMODE(client.getNickname()));
		return ;
	}
	for(size_t i = 2; i < fields.size(); i++)
		if (!fields[i].empty())
			args.push_back(fields[i]);
	if (check_params(args, fields[1]) == 1){
		replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		return ;
	}
	size_t i = 0;
	for(; i < fields[1].size(); i++){
		if (fields[1][i] == '+') sign = true;
		else if (fields[1][i] == '-') sign = false;
		else if (fields[1][i] == 'o') operatorFlag(client, it, sign, args);
		else if (fields[1][i] == 'k') keyWordFlag(it->second, sign, args);
		else if (fields[1][i] == 'i') invetOnlyFlag(it->second, sign);
		else if (fields[1][i] == 'l') limitFlag(it->second, sign, args);
		else if (fields[1][i] == 't') topicFlag(it->second, sign, args);
	}
	// replyTo(client.getSocket(), MODE_SET(client.getNickname(), client.getUsername(), get_host(_addr) ,it->first, fields[1]));
}

// run the correct command 
void Server::commandRunner(std::vector<std::string> &fields, Client &user)
{
	std::string command(fields[0]);
	fields.erase(fields.begin());


	if (_commands.find(command) != _commands.end())
		(this->*_commands[command])(fields, user);
	else if (command == "PING")
	{
		if (!fields.empty())
			replyTo(user.getSocket(), fields[0]);
		replyTo(user.getSocket(), ERR_NEEDMOREPARAMS(user.getNickname(), command));
	}
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
	}
	else {
		// here we handle the message
		buffer[rc] = '\0';
		std::string rec(buffer);
		// check if the message is valid (finished by \r\n)
		if (rec.find_first_of("\r") == std::string::npos && rec.find_first_of("\n") == std::string::npos)
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
			commandRunner(fields, _clients.find(_pollFds[i].fd)->second);
		
			_clients.find(_pollFds[i].fd)->second.refStatus();
		}
	}
}


void Server::createServer() 
{
	int		current_size;
	int		rc;
	_nfds = 1;

	// Start listening for incoming connections
	std::cout << "server is running : " << std::endl;
	while (true) {
	    // Wait for events on monitored file descriptors
	    rc = poll(_pollFds.data(), _nfds, 0);

		// Check if the server is shutting down by signal
		if (Server::_signal)
			throw std::runtime_error("Server is shutting down");
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