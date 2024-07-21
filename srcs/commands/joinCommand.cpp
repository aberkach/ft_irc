/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:29:46 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/21 22:30:11 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"
#include "../channel/channel.hpp"
#include "../client/client.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

bool Server::joinChannel(std::string &chnName, std::vector<std::string> &keys, Client &client, chnMapIt &chnIt) {
    // if the channel already exist, check if the client is already in the channel
    if (chnIt != _channels.end())
    {
        // if the client is already in the channel, do nothing
        if (chnIt->second.isClientExist(client.getNickname()) == true)
            return false;
        // if the client is not in the channel, add the client to the channel
        else
        {
            // check if the channel has a key
            if (keys.size() > 0)
            {
                // if the key is correct
                if (keys[0] == chnIt->second.getKey())
                {
                    // add the client to the channel
                    if (chnIt->second.getIsInviteOnly() == true)
                    {
                        if (chnIt->second.isInvited(client))
                            chnIt->second.removeInvite(client);
                        else
                        {
                            replyTo(client.getSocket(), ERR_INVITEONLYCHAN(client.getNickname(), chnName));
                            return false;
                        }
                    }
                    chnIt->second.addUser(client);
                    keys.erase(keys.begin());
                }
                // if the key is incorrect
                else
                {
                    // here we send an error message to the client to inform him that the key is incorrect
                    replyTo(client.getSocket(), ERR_BADCHANNELKEY(client.getNickname(), chnName));
                    keys.erase(keys.begin());
                    return false;
                }
            }
            // if the channel doesn't have a key, add the client to the channel
            else
            {
                // if the channel has no key, add the client to the channel
                if (chnIt->second.getIsInviteOnly() == true)
                {
                    if (chnIt->second.isInvited(client))
                        chnIt->second.removeInvite(client);
                    else
                    {
                        replyTo(client.getSocket(), ERR_INVITEONLYCHAN(client.getNickname(), chnName));
                        return false;
                    }
                }
                else if (Server::countUsersInChannel(chnName) >= chnIt->second.getMaxUsers())
                {
                    replyTo(client.getSocket(), ERR_CHANNELISFULL(client.getNickname(), chnName));
                    return false;
                }
                chnIt->second.addUser(client);
            }
        }
        // here we send a message to the client to inform him that he joined the channel and broadcast the message to the other users in the channel
        std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
        for (std::map<std::string, Client>::iterator it = chnIt->second.getUsers().begin(); it != chnIt->second.getUsers().end(); it++)
        {
            std::string usersList = chnIt->second.getChannelUsersInString();
            replyTo(it->second.getSocket(), RPL_JOIN(chnIt->second.getUserName(client.getNickname()), client.getUsername(), chnName, clientHost));
            // this replies to the client with the list of users in the channel and topic of the channel
            if (it->first == client.getNickname())
            {
                replyTo(client.getSocket(), RPL_TOPIC(chnIt->second.getUserName(client.getNickname()), chnName, chnIt->second.getTopic()));
                replyTo(client.getSocket(), RPL_NAMREPLY(usersList, chnName, chnIt->second.getUserName(client.getNickname())));
                replyTo(client.getSocket(), RPL_ENDOFNAMES(chnIt->second.getUserName(client.getNickname()), chnName));
            }
        }
    }
    return true;
}

bool Server::createChannel(std::string &chnName, std::vector<std::string> &keys, Client &client) {
    // create a new channel
    _channels.insert(std::pair<std::string, Channel>(chnName, Channel(chnName)));
    // check if the channel has a key
    if (keys.size() > 0)
    {
        // check if the key is valid
        if (keys[0].find_first_of(" ,\a\b\f\t\v$:&+~%") != std::string::npos)
        {
            // here we send an error message to the client to inform him that the key is incorrect
            keys.erase(keys.begin());
            _channels.erase(chnName);
            // shouldn't send the error message to the client !!!!!!!!!!
            replyTo(client.getSocket(), ERR_BADCHANNELKEY(client.getNickname(), chnName));
            return false;
        }
        _channels.find(chnName)->second.setKey(keys[0]);
        keys.erase(keys.begin());
    }
    else
        _channels.find(chnName)->second.setKey("");
    // make the client an operator of the channel and add him to the channel
    _channels.find(chnName)->second.addOperator(client);
    return true;
}

void Server::processTheJoinArgs(std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::string chnName = channels[i];
        // check if the channel name is valid
        if ((chnName[0] != '#') || (chnName.find_first_of(" ,\a\b\f\t\v$:+~%") != std::string::npos) || chnName.size() < 2)
        {
            // here we send an error message to the client to inform him that the channel name is incorrect
            replyTo(client.getSocket(), ERR_BADCHANMASK(chnName));
            if (keys.size() > 0)
                keys.erase(keys.begin());
            continue;
        }
        else
        {
            // check if the channel already exist
            chnMapIt chnIt = _channels.find(chnName);
            
            // create the channel
            if (chnIt == _channels.end())
            {
                if (!createChannel(chnName, keys, client))
                    continue;
                // here we send a message to the client to inform him that he joined the channel
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                chnIt = _channels.find(chnName);
                
                // here we send a message to the client to inform him that he joined the channel and broadcast the message to the other users in the channel
                for (std::map<std::string, Client>::iterator it = chnIt->second.getUsers().begin(); it != chnIt->second.getUsers().end(); it++)
                {
                    std::string usersList = chnIt->second.getChannelUsersInString();
                    replyTo(it->second.getSocket(), RPL_JOIN(client.getNickname(), client.getUsername(), chnName, clientHost));
                    replyTo(client.getSocket(), RPL_NAMREPLY(usersList, chnName, chnIt->second.getUserName(it->first)));
                    replyTo(client.getSocket(), RPL_ENDOFNAMES(chnIt->second.getUserName(it->first), chnName));
                }
            }
            // join the existing channel
            else {   
                if (!joinChannel(chnName, keys, client, chnIt))
                    continue;
            }
        }
    }
}

void Server::joinCommand(std::vector<std::string> &fields, Client &client) {

    std::vector<std::string> channels;
    std::vector<std::string> keys;

	// check if the client is registered
	if (client.getRegistered() == true) {
    	// check if the message contains a channel name
    	if (fields.size() > 0 )
    	{
    	    // split the channels and keys by comma
    	    channels = splitByDelim(fields[0], ',');
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
