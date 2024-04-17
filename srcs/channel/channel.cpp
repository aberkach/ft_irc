/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:34:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/17 12:17:01 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../channel/channel.hpp"
#include <string>
#include "../client/client.hpp"

Channel::Channel() : _name("default"), _topic("default"), _key("default")
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

std::string Channel::getTopic() const
{
    return _topic;
}

Client &Channel::getUser(std::string &nickName)
{
    return _users[nickName];
}

void Channel::getUsers()
{
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        std::cout << it->first << " ";
    }
}

std::string Channel::getUsersList()
{
    std::string usersList;
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        usersList += it->first + " ";
    }
    return usersList;
}

void Channel::setName(std::string name)
{
    _name = name;
}


void Channel::setKey(std::string key)
{
    _key = key;
}

bool Channel::isClientExist(std::string nickName)
{
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->second.getNickname() == nickName)
            return true;
    }
    return false;
}

void Channel::addUser(Client &client)
{
    client.setChannel(_name, *this);
    _users.insert(std::pair<std::string, Client>(client.getNickname(), client));
}

void Channel::removeUser(Client &client)
{
    _users.erase(client.getNickname());
}

void Channel::addOperator(Client &op)
{
    _users.insert(std::pair<std::string, Client>('@' + op.getNickname(), op));
    op.setChannel(_name, *this);
    _chanOps.push_back(_users['@' + op.getNickname()]);
}

bool Channel::isOperator(Client &op)
{
    if (_users.find('@' + op.getNickname()) != _users.end())
        return true;
    return false;
}