/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:29:46 by abberkac          #+#    #+#             */
/*   Updated: 2024/08/31 22:17:37 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/irc.hpp"

bool Server::joinExistChannel(const std::string &chnName, std::vector<std::string> &keys, Client &client, const chanIt &chnIt) {
    // if the channel already exist, check if the client is already in the channel
    if (chnIt != _channels.end())
    {
        // if the client is already in the channel, do nothing
        if (chnIt->second.isClientExist(client.getNickname()) == true)
            return (replyTo(client.getSocket(), ERR_ALREADYINCHANNEL(client.getUsername(), client.getNickname(), chnName)), false);
        else {
            // check if the channel has a key
            if (keys.size() > 0) {
                // if the key is correct
                if (keys[0] == chnIt->second.getKey()) {
                    // add the client to the channel
                    if (chnIt->second.getIsInviteOnly() == true) {
                        if (chnIt->second.isInvited(client))
                            chnIt->second.removeInvite(client);
                        else
                            return (replyTo(client.getSocket(), ERR_INVITEONLYCHAN(client.getNickname(), chnName)), false);
                    }
                    chnIt->second.addUser(client);
                    keys.erase(keys.begin());
                }
                // if the key is incorrect
                else {
                    // here we send an error message to the client to inform him that the key is incorrect
                    replyTo(client.getSocket(), ERR_BADCHANNELKEY(client.getNickname(), chnName));
                    return (keys.erase(keys.begin()), false);
                }
            }
            // if the channel doesn't have a key, add the client to the channel
            else {
                // if the channel has no key, add the client to the channel
                if (chnIt->second.getIsInviteOnly() == true) {
                    if (chnIt->second.isInvited(client))
                        chnIt->second.removeInvite(client);
                    else
                        return (replyTo(client.getSocket(), ERR_INVITEONLYCHAN(client.getNickname(), chnName)), false);
                }
                // check if the channel is full
                if (Server::countUsersInChannel(chnName) >= chnIt->second.getMaxUsers() && chnIt->second.getMaxUsers() != 0)
                    return (replyTo(client.getSocket(), ERR_CHANNELISFULL(client.getNickname(), chnName)), false);
                chnIt->second.addUser(client);
            }
        }
        // here we send a message to the client to inform him that he joined the channel and broadcast the message to the other users in the channel
        std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
        for (std::map<std::string, Client>::iterator it = chnIt->second.getUsers().begin(); it != chnIt->second.getUsers().end(); it++)
        {
            std::string usersList = chnIt->second.getChannelUsersInString();
            replyTo(it->second.getSocket(), RPL_JOIN(client.getNickname(), client.getUsername(), chnName, clientHost));
            // this replies to the client with the list of users in the channel and topic of the channel
            if (it->second.getNickname() == client.getNickname()) {
                replyTo(client.getSocket(), RPL_NAMREPLY(usersList, chnName, client.getNickname()));
                replyTo(client.getSocket(), RPL_ENDOFNAMES(client.getNickname(), chnName));
                
                if (chnIt->second.getTopicFlag()) {
                    const std::string &top = chnIt->second.getTopic();
                    if (!top.empty())
                        replyTo(client.getSocket(), RPL_TOPIC(client.getNickname(), chnName, top));
                }
            }
        }
    }
    return true;
}

bool Server::createChannel(const std::string &chnName, std::vector<std::string> &keys, Client &client) {
    _channels.insert(std::pair<std::string, Channel>(chnName, Channel(chnName)));
    chanIt chnIt = _channels.find(chnName);
    chnIt->second.setMaxUsers(0);
    if (keys.size() > 0) {
        // check if the key is valid
        if (!chnIt->second.setKey(keys[0])) {
            keys.erase(keys.begin());
            _channels.erase(chnIt);
            replyTo(client.getSocket(), ERR_INVALIDKEY(client.getNickname(), chnName));
            return false;
        }
        keys.erase(keys.begin());
    }
    // add the client to the channel
    chnIt->second.addUser(client);
    // make the client an operator of the channel and add him to the channel
    chnIt->second.addOperator(client);
    return true;
}

void Server::processTheJoinArgs(const std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client)
{
    for (size_t i = 0; i < channels.size(); i++) {
        std::string chnName = channels[i];
        // check if the channel name is valid
        
        if ((chnName[0] != '#') || (chnName.find_first_of(" ,\a\b\f\t\v$:+~%") != std::string::npos) || chnName.size() < 2) {
            // here we send an error message to the client to inform him that the channel name is incorrect
            replyTo(client.getSocket(), ERR_BADCHANMASK(client.getNickname(), chnName)); 
            if (keys.size() > 0)
                keys.erase(keys.begin());
            continue;
        }
        else {
            std::string tmpChn = stringUpper(chnName);
            chanIt chnIt;
            for (chnIt = _channels.begin(); chnIt != _channels.end(); chnIt++)
            {
                std::string tmp = stringUpper(chnIt->first);
                if (tmp == tmpChn) 
                    break;
            }
            // create the channel
            if (chnIt == _channels.end()) {
                if (!createChannel(chnName, keys, client))
                    continue;
                // here we send a message to the client to inform him that he joined the channel
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                chnIt = _channels.find(chnName);
                
                // here we send a message to the client to inform him that he joined the channel and broadcast the message to the other users in the channel
                std::string usersList = chnIt->second.getChannelUsersInString();
                for (std::map<std::string, Client>::iterator it = chnIt->second.getUsers().begin(); it != chnIt->second.getUsers().end(); it++) {
                    replyTo(it->second.getSocket(), RPL_JOIN(client.getNickname(), client.getUsername(), chnName, clientHost));
                    replyTo(client.getSocket(), RPL_NAMREPLY(usersList, chnName, it->second.getNickname()));
                    replyTo(client.getSocket(), RPL_ENDOFNAMES(it->second.getNickname(), chnName));
                }
            }
            // join the existing channel
            else
                if (!joinExistChannel(chnName, keys, client, chnIt))
                    continue;
        }
    }
}

void Server::joinCommand(const std::vector<std::string> &fields, Client &client) {

    std::vector<std::string> channels;
    std::vector<std::string> keys;

	// check if the client is registered
	if (client.getRegistered() == true) {
    	// check if the message contains a channel name
    	if (fields.size() > 0 )
    	{
    	    // split the channels and keys by comma
    	    channels = splitByDelim(fields[0], ',');
            if (fields.size() > 1)
    	        keys = splitByDelim(fields[1], ',');
    	    // process the channels and keys
    	    processTheJoinArgs(channels, keys, client);
    	}
    	else
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
	}
    // if the client is not registered, send an error message    
	else
		replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}
