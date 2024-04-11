/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:34:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/10 02:17:01 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../channel/channel.hpp"

Channel::Channel() : _name("default")
{
}

Channel::Channel(std::string name) : _name(name)
{
}

Channel::~Channel()
{
}

Channel::Channel(const Channel &src)
{
    *this = src;
}

std::string Channel::getName() const
{
    return _name;
}

std::string Channel::getKey() const
{
    return _key;
}
void Channel::getUsers()
{
    for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        std::cout << it->first << " ";
    }
}

void Channel::setName(std::string name)
{
    _name = name;
}


void Channel::setKey(std::string key)
{
    _key = key;
} 

bool Channel::isClientExist(int clientFd)
{
    for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->second.getsocket() == clientFd)
            return true;
    }
    return false;
}

void Channel::addUser(Client &client)
{
    _users.insert(std::pair<int, Client>(client.getsocket(), client));
}