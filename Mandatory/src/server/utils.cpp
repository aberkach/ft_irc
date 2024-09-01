/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 23:27:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/08/31 22:07:44 by abberkac         ###   ########.fr       */
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


std::vector<std::string>
Server::getBuffers(const std::string &buffer) {
    std::vector<std::string> messages;
    size_t start = 0, end = 0;

	while ((end = buffer.find_first_of('\n', start)) != std::string::npos) {
		while (buffer[end] && buffer[end] == '\n')
			end++;
		std::string message = buffer.substr(start, end - start);
		messages.push_back(message);
		start = end;
	}

    if (start < buffer.length())
        messages.push_back(buffer.substr(start));

    return messages;
};


void Server::dsconnectClient (int fd) {
	size_t i;
	for (i = 0; i < _nfds; i++) {
		if (_pollFds[i].fd == fd) {
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}
	for (i = 0; i < _clients.size(); i++) {
		if (_clients[i].getSocket() == fd) {
			// remove the client from the channels
			for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
			{
				if (it->second.isClientExist(_clients[i].getNickname())) {
					std::string clientHost = inet_ntoa(_clients[i].getAddr().sin_addr);
					std::string quitMessage = QUIT_MSG(_clients[i].getNickname(), _clients[i].getUsername(), clientHost, " Forced quit");
					it->second.broadCast(quitMessage, _clients[i].getSocket());
					it->second.removeUser(_clients[i].getNickname(), 1);
				}
			}
			_clients.erase(i);
			close(fd);
			break;
		}
	}
}

void Server::cleanUp()
{
	for (size_t i = 0; i < _nfds; i++)
		close(_pollFds[i].fd);
	_clients.clear();
	_channels.clear();
	if (_listen_sd > 0)
		close(_listen_sd);
}

size_t Server::countUsersInChannel(const std::string &chnName)
{
	chanIt it = _channels.find(chnName);
	if (it == _channels.end())
		return 0;
	return it->second.getUsers().size();
}
