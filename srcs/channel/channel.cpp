/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:34:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/21 23:27:10 by abberkac         ###   ########.fr       */
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
    return (_users.find(nickName)->second);
}

std::map<std::string, Client> &Channel::getUsers()
{
    return _users;
}

std::string Channel::getUserName(std::string clientName)    
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

void Channel::removeUser(Client &client)
{
    // client.removeChannel(channelName);

    // maybe I gotta check if the user is the operator of the channel
    // so I need to set another operator
    for (std::vector<Client>::iterator it = _chanOps.begin(); it != _chanOps.end(); it++)
    {
        if (it->getNickname() == client.getNickname())
        {
            _chanOps.erase(it);
            break;
        }
    }
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->second.getNickname() == client.getNickname() || it->first == '@' + client.getNickname())
        {
            _users.erase(it);
            break;
        }
    }
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


void Channel::addInvite(Client &invited)
{
    _chanInvites.insert(_chanInvites.begin(), Client(invited));
}

bool Channel::isInvited(Client &invited)
{
    for (std::vector<Client>::iterator it = _chanInvites.begin(); it != _chanInvites.end(); it++)
    {
        if (it->getNickname() == invited.getNickname())
            return true;
    }
    return false;
}

void Channel::removeInvite(Client &invited)
{
    for (std::vector<Client>::iterator it = _chanInvites.begin(); it != _chanInvites.end(); it++)
    {
        if (it->getNickname() == invited.getNickname())
        {
            _chanInvites.erase(it);
            break;
        }
    }
}

void replyTo(int socket, std::string buffer); // FORWARD DEC WILL CHANGE LATTER

void Channel::broadCast(std::string msg, int excludedFd)
{
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if (it->second.getSocket() != excludedFd)
            replyTo(it->second.getSocket(), msg);
    }
}