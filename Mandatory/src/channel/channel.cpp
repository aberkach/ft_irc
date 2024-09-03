/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:34:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/09/03 22:25:46 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/irc.hpp"

Channel::Channel(const std::string &name) : _name(name), _topic(""), _isInviteOnly(false),
                                _topicFlag(true), _key(""),  _maxUsers(0)
{
    _unixCreation = std::time(NULL);
};

Channel::~Channel()
{
}


std::string
Channel::getCreationTime() const
{
    return to_string(_unixCreation);
};

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

size_t Channel::getMaxUsers() const
{
    return _maxUsers;
}

std::string Channel::getTopic() const
{
    return _topic;
}
bool Channel::getTopicFlag() const
{
    return _topicFlag;
}

std::vector<Client> Channel::getOperator() const
{
    return _chanOps;
}

Client &Channel::getUser(const std::string &nickName)
{
    return (_users.find(nickName)->second);
}

std::map<std::string, Client> &Channel::getUsers()
{
    return _users;
}


std::string Channel::getChannelUsersInString() const
{
    std::string users;

    for (std::map<std::string, Client>::const_iterator it = _users.begin(); it != _users.end(); it++)
    {
        for (std::vector<Client>::const_iterator opIt = _chanOps.begin(); opIt != _chanOps.end(); opIt++) {
            if (opIt->getNickname() == it->second.getNickname())
            {
                users += "@";
                break;
            }
        }
        users += it->second.getNickname();
        users += " ";
    }
    return users;
}

void Channel::setTopicFlag(bool topicFlag)
{
    _topicFlag = topicFlag;
}

void Channel::setIsInviteOnly(bool isInviteOnly)
{
    _isInviteOnly = isInviteOnly;
}

void Channel::setMaxUsers(size_t maxUsers)
{
    _maxUsers = maxUsers;
}


void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}

void Channel::setName(const std::string &name)
{
    _name = name;
}

bool Channel::setKey(const std::string &key)
{
    if (key.find_first_of("\a\b\f\t\v") != std::string::npos)
        return false;
    _key = key;
    return true;
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

void Channel::addUser(const Client &client)
{
    if (!isClientExist(client.getNickname()))
        _users.insert(std::pair<std::string, Client>(client.getNickname(), client));
}

void Channel::removeUser(std::string nickName, bool falg)
{
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->second.getNickname() == nickName)
        {
            if (isOperator(nickName) && falg)
                removeOperator(it->second);
            _users.erase(it);
            break;
        }
    }
}

void Channel::addOperator(const Client &op)
{
    if (!isOperator(op.getNickname()))
        _chanOps.insert(_chanOps.begin(), op);
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

void
Channel::broadCast(const std::string &msg, int excludedFd)
{
    for (std::map<std::string, Client>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if (it->second.getSocket() != excludedFd) {
            replyTo(it->second.getSocket(), msg);
        }
    }
};
