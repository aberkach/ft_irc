/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:34:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/19 19:20:21 by abberkac         ###   ########.fr       */
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

// Channel::Channel(const Channel &src)
// {
//     *this = src;
// }

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

void Channel::setTopic(std::string topic)
{
    _topic = topic;
}

Client &Channel::getUser(std::string &nickName)
{
    return _users[nickName];
}

std::map<std::string, Client> &Channel::getUsers()
{
    return _users;
}

std::string Channel::getUserName(std::string clientName) const
{
    for (std::map<std::string, Client>::const_iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->first == clientName)
            return it->second.getNickname();
        else if (it->first == '@' + clientName)
            return '@' + it->second.getNickname();
    }
    return "User not found";
}

std::string &Channel::getChannelUsersInString()
{
    std::string *users = new std::string();
    
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        *users += it->first;
        *users += " ";
    }
    return *users;
}

std::vector<std::string> Channel::getUsersList()
{
    std::vector<std::string> usersList;
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); it++)
        usersList.push_back(it->first);
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
    // client.setChannel(_name, *this);
    _users.insert(std::pair<std::string, Client>(client.getNickname(), client));
}

void Channel::removeUser(Client &client, std::string &channelName)
{
    // client.removeChannel(channelName);

    // maybe I gotta check if the user is the operator of the channel
    // so I need to set another operator
    _users.erase(client.getNickname());
}

void Channel::addOperator(Client &op)
{
    // op.setChannel(_name, *this);
    _users.insert(std::pair<std::string, Client>('@' + op.getNickname(), op));
    _chanOps.insert(_chanOps.begin(), Client(op));
}

bool Channel::isOperator(Client &op)
{
    for (std::vector<Client>::iterator it = _chanOps.begin(); it != _chanOps.end(); it++)
    {
        if (it->getNickname() == op.getNickname())
            return true;
    }
    return false;
}