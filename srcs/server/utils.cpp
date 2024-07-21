/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 23:27:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/21 23:47:00 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../../Inc/ft_irc.hpp"
#include <array>
#include <cstddef>
#include <string>
#include <sys/poll.h>
#include <vector>


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
		std::cout << "Closing server socket" << std::endl;
		close(_listen_sd);
	}
}