/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 23:27:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/27 07:17:23 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../../Inc/ft_irc.hpp"
#include <array>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/poll.h>
#include <vector>
#include <sstream>

void Err(const std::string &msg)
{
	std::cerr << msg << std::endl;
}

std::string
trimTheSpaces(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
};

std::string stringUpper(const std::string &_str)
{
	std::string upper(_str);

    for (std::string::size_type i = 0; i < _str.size(); ++i)
        upper[i] = ::toupper(_str[i]);

	return(upper);
}


std::vector<std::string> splitBySpace(const std::string &str)
{
    std::string part;
    std::vector<std::string> parts;
    std::istringstream iss(str);

    while (iss >> part) {
  
		if (!part.empty() && part[0] == ':') {
			std::string reset;
			part.erase(part.begin());
			getline(iss, reset);
			part += reset;
			parts.push_back(part);
			return parts;
		}
        parts.push_back(part);
    }

    return parts;
};

std::vector<std::string> splitByDelim(const std::string &str, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(str); // Use stringstream for efficient tokenization
    std::string token;

    while (std::getline(ss, token, delim)) { // Read tokens until a delimiter is found
        tokens.push_back(token);
    }
    return tokens;
}

void	Server::sigHandler(int sigNumber)
{
	(void)sigNumber;
	Server::_signal = true;
	std::cout << std::endl;
}

void Server::cleanUp()
{
	for (size_t i = 0; i < _nfds; i++)
	{
		close(_pollFds[i].fd);
		_pollFds[i].fd = -1;
	}
	_clients.clear();
	_channels.clear();
	if (_listen_sd > 0)
	{
		std::cout << "Closing server socket" << std::endl;
		close(_listen_sd);
	}
}

size_t Server::countUsersInChannel(const std::string &chnName)
{
	chnMapIt it = _channels.find(chnName);
	if (it == _channels.end())
		return 0;
	return it->second.getUsers().size();
}