/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:34:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/08/02 06:19:38 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../channel/channel.hpp"
#include <string>
#include "../client/client.hpp"

Channel::Channel(const std::string &name) : _name(name), _topic(""), _isInviteOnly(false), _topicFlag(true), _key(""),  _maxUsers(0)
{
}

Channel::~Channel()
{
}


void Channel::removeOperator(const Client &op)
{
    for (std::vector<Client>::iterator it = _chanOps.begin(); it != _chanOps.end(); it++)
    {
        if (it->getNickname() == op.getNickname())
        {
            _chanOps.erase(it);
            break;
        }
    }
}

std::string Channel::getName() const
{
    return _name;
}

std::string Channel::getKey() const
{
    return _key;
}


bool Channel::getIsInviteOnly() const
{
    return _isInviteOnly;
}

void Channel::setIsInviteOnly(bool isInviteOnly)
{
    _isInviteOnly = isInviteOnly;
}

void Channel::setMaxUsers(size_t maxUsers)
{
    _maxUsers = maxUsers;
}

size_t Channel::getMaxUsers() const
{
    return _maxUsers;
}

std::string Channel::getTopic() const
{
    return _topic;
}

void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}

bool Channel::getTopicFlag() const
{
    return _topicFlag;
}

void Channel::setTopicFlag(bool topicFlag)
{
    _topicFlag = topicFlag;
}

std::vector<Client> Channel::getOperator(){
    return _chanOps;
}

Client &Channel::getUser(std::string &nickName)
{
    return (_users.find(nickName)->second);
}

std::map<std::string, Client> &Channel::getUsers()
{
    return _users;
}


std::string Channel::getChannelUsersInString()
{
    std::string users;
    
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        for (std::vector<Client>::const_iterator opIt = _chanOps.begin(); opIt != _chanOps.end(); opIt++) {
            if (opIt->getNickname() == it->first)
            {
                users += "@";
                break;
            }
        }
        users += it->first;
        users += " ";
    }
    return users;
}

std::string Channel::getChannelModes()
{
    std::string modes;

    if (_isInviteOnly)
        modes += "i";
    if (_topicFlag)
        modes += "t";
    if (_maxUsers > 0)
        modes += "l";
    return modes;
}

void Channel::setName(const std::string &name)
{
    _name = name;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
}

bool Channel::isClientExist(const std::string &nickName)
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
    _users.insert(std::pair<std::string, Client>(client.getNickname(), Client(client)));
}

void Channel::removeUser(Client &client)
{
    // client.removeChannel(channelName);

    // maybe I gotta check if the user is the operator of the channel
    // so I need to set another operator
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->second.getNickname() == client.getNickname() || it->first == '@' + client.getNickname())
        {   
            _users.erase(it);
            break;
        }
    }
    if (isOperator(client.getNickname()))
    {
        for (std::vector<Client>::iterator it = _chanOps.begin(); it != _chanOps.end(); it++)
        {
            if (it->getNickname() == client.getNickname())
            {
                _chanOps.erase(it);
                break;
            }
        }
    }
}

void Channel::addOperator(const Client &op)
{
    // op.setChannel(_name, *this);
    _users.insert(std::pair<std::string, Client>('@' + op.getNickname(), op));
    _chanOps.insert(_chanOps.begin(), Client(op));
}

bool Channel::isOperator(const std::string &nickName)
{
    for (std::vector<Client>::iterator it = _chanOps.begin(); it != _chanOps.end(); it++)
    {
        if (it->getNickname() == nickName)
            return true;
    }
    return false;
}


void Channel::addInvite(const Client &invited)
{
    _chanInvites.insert(_chanInvites.begin(), Client(invited));
}

bool Channel::isInvited(const Client &invited)
{
    for (std::vector<Client>::iterator it = _chanInvites.begin(); it != _chanInvites.end(); it++)
    {
        if (it->getNickname() == invited.getNickname())
            return true;
    }
    return false;
}

void Channel::removeInvite(const Client &invited)
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

void replyTo(int socket, const std::string &buffer); // FORWARD DEC WILL CHANGE LATTER

void Channel::broadCast(const std::string &msg, int excludedFd)
{
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if (it->second.getSocket() != excludedFd)
            replyTo(it->second.getSocket(), msg);
    }
}