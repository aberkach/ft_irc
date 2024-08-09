/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 23:27:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/08/09 00:27:24 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/irc.hpp"

bool Server::isClientInServer(const std::string &nickName)
{
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second.getNickname() == nickName)
            return true;
    }
    return false;
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
	chanIt it = _channels.find(chnName);
	if (it == _channels.end())
		return 0;
	return it->second.getUsers().size();
}