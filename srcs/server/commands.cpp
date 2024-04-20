/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:19:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/20 20:10:42 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "../channel/channel.hpp"
#include "../client/client.hpp"


// invite command
// still on testing
void Server::inviteCommand(std::vector<std::string> &fields, Client &client) {
    if (fields.size() < 2) {
        replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
        return;
    }
    if (client.getRegistered()) {
        std::string invitedUser = fields[0];
        std::string chnName = fields[1];
        chnMapIt chnIt = server_channels.find(chnName);
        if (chnIt == server_channels.end())
        {
            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName));
            return;
        }
        if (chnIt->second.isOperator(client)) {
            if (chnIt->second.isClientExist(invitedUser))
            {
                // here we send a message to the client that has been invited
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                std::string inviteMessage = INVITE_MSG(client.getNickname(), clientHost, invitedUser, chnName);
                replyTo(chnIt->second.getUser(invitedUser).getSocket(), inviteMessage);
            }
            else
                replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), invitedUser, chnName));
        }
        else
            replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), chnName));
    }
}

// topic command 

void Server::topicCommand (std::vector<std::string> &fields, Client &client) {
        
    if (client.getRegistered() == true) {
        std::string chnName = fields[0];
        std::string topic;
        // if the client wants to change the topic of the channel
        if(fields.size() > 1) {
            topic = fields[1];
            // check if the client is operator of the channel
            chnMapIt chnIt = server_channels.find(chnName);
            if (chnIt == server_channels.end())
            {
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName));
                return;
            }
            if (chnIt->second.isOperator(client)) {
                topic = fields[1];
                chnIt->second.setTopic(topic);
                replyTo(client.getSocket(), RPL_TOPIC(client.getNickname(), chnName, topic));
            }
            else
                replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), chnName));
        }
        // if the client wants to get the topic of the channel
        else if (fields.size() == 1)
        {
            chnMapIt chnIt = server_channels.find(chnName);
            if (chnIt == server_channels.end())
            {
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName));
                return;
            }
            if (chnIt->second.getTopic() != "")
                replyTo(client.getSocket(), RPL_TOPIC(client.getNickname(), chnName, chnIt->second.getTopic()));
            else
                replyTo(client.getSocket(), RPL_NOTOPIC(client.getNickname(), chnName));
        }
        else
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}


// kick command
//TODO: this still on testing /.......

void Server::kickCommand (std::vector<std::string> &fields, Client &client) {
    if (client.getRegistered()) {

        if (fields.size() < 2) {
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
            return;
        }
        std::string chnName = fields[0];
        std::string usersBeKicked = fields[1];
        chnMapIt joinedChnIt = server_channels.find(chnName);
        if (joinedChnIt == server_channels.end())
        {
            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName));
            return;
        }
        if (joinedChnIt->second.isClientExist(client.getNickname()) == false)
        {
            replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chnName));
            return;
        }
        if (joinedChnIt->second.isOperator(client)) {
            if (joinedChnIt->second.isClientExist(usersBeKicked))
            {
                // send a message to the client that has been kicked
                std::string reason = "You've been kicked from the channel";
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                std::string KickErrMessage = RPL_KICK(client.getNickname(), client.getRealname(), clientHost, chnName, usersBeKicked, reason);
                
                for (std::map<std::string, Client>::iterator it = joinedChnIt->second.getUsers().begin(); it != joinedChnIt->second.getUsers().end(); it++)
                {
                    replyTo(it->second.getSocket(), KickErrMessage);
                }
                joinedChnIt->second.removeUser(joinedChnIt->second.getUser(usersBeKicked), chnName);
                
            }
            // if the client is not in the channel, send an error message to the client
            else
                replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), usersBeKicked, chnName));
        }
        else
            replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), chnName));
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}


//TODO: this still on testing /.......

bool Server::joinChannel(std::string &chnName, std::vector<std::string> &keys, Client &client, chnMapIt &chnIt) {
    // if the channel already exist, check if the client is already in the channel
    if (chnIt != server_channels.end())
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
                if (chnIt->second.getKey() == "")
                    chnIt->second.addUser(client);
                else
                {
                    // here we send an error message to the client to inform him that the channel has a key
                    replyTo(client.getSocket(), ERR_BADCHANNELKEY(client.getNickname(), chnName));
                    return false;
                }
            }
        }
        std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
        // replyTo(client.getSocket(), RPL_TOPIC(chnIt->second.getUserName(client.getNickname()), chnName, chnIt->second.getTopic()));
        
        // replyTo(client.getSocket(), RPL_TOPIC(chnIt->second.getUserName(client.getNickname()), chnName, chnIt->second.getTopic()));
        for (std::map<std::string, Client>::iterator it = chnIt->second.getUsers().begin(); it != chnIt->second.getUsers().end(); it++)
        {
            std::string usersList = chnIt->second.getChannelUsersInString();
            replyTo(it->second.getSocket(), RPL_JOIN(chnIt->second.getUserName(client.getNickname()), client.getRealname(), chnName, clientHost));
            if (it->first == client.getNickname())
            {
                // replyTo(client.getSocket(), RPL_TOPIC(chnIt->second.getUserName(client.getNickname()), chnName, chnIt->second.getTopic()));
                replyTo(client.getSocket(), RPL_NAMREPLY(usersList, chnName, chnIt->second.getUserName(client.getNickname())));
                replyTo(client.getSocket(), RPL_ENDOFNAMES(chnIt->second.getUserName(client.getNickname()), chnName));
            }
        }
    }
    return true;
}

bool Server::createChannel(std::string &chnName, std::vector<std::string> &keys, Client &client) {
    // create a new channel
    server_channels.insert(std::pair<std::string, Channel>(chnName, Channel(chnName)));
    // check if the channel has a key
    if (keys.size() > 0)
    {
        // check if the key is valid
        if (keys[0].find_first_of(" ,\a\b\f\t\v$:&+~%") != std::string::npos)
        {
            // here we send an error message to the client to inform him that the key is incorrect
            keys.erase(keys.begin());
            server_channels.erase(chnName);
            replyTo(client.getSocket(), ERR_BADCHANNELKEY(client.getNickname(), chnName));
            return false;
        }
        server_channels.find(chnName)->second.setKey(keys[0]);
        keys.erase(keys.begin());
    }
    else
        server_channels.find(chnName)->second.setKey("");
    // make the client an operator of the channel and add him to the channel
    server_channels.find(chnName)->second.addOperator(client);
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
            chnMapIt chnIt = server_channels.find(chnName);
            
            // create the channel
            if (chnIt == server_channels.end())
            {
                if (!createChannel(chnName, keys, client))
                    continue;
                // here we send a message to the client to inform him that he joined the channel
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                chnIt = server_channels.find(chnName);
                
                // replyTo(client.getSocket(), RPL_TOPIC(chnIt->second.getUserName(client.getNickname()), chnName, chnIt->second.getTopic()));
                for (std::map<std::string, Client>::iterator it = chnIt->second.getUsers().begin(); it != chnIt->second.getUsers().end(); it++)
                {
                    std::string usersList = server_channels.find(chnName)->second.getChannelUsersInString();
                    replyTo(it->second.getSocket(), RPL_JOIN(client.getNickname(), client.getRealname(), chnName, clientHost));
                    replyTo(client.getSocket(), RPL_NAMREPLY(usersList, chnName, chnIt->second.getUserName(it->first)));
                    replyTo(client.getSocket(), RPL_ENDOFNAMES(chnIt->second.getUserName(it->first), chnName));
                }
            }

            // join the channel
            if (!joinChannel(chnName, keys, client, chnIt))
                continue;
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
    // print the users in the channel
    // for (chnMapIt it = server_channels.begin(); it != server_channels.end(); it++)
    // {
    //     std::cout << "channel: " << it->first << std::endl;
    //     std::map<std::string, Client> users = it->second.getUsers();
    //     for (std::map<std::string, Client>::iterator it2 = users.begin(); it2 != users.end(); it2++)
    //         std::cout << "user: " << it2->first << std::endl;
    //     std::cout << "----------------" << std::endl;
    // }
}
 

void Server::passCommand(const std::vector<std::string> &fields, Client &user)
{
	if (user.getValidPass() == false)
	{
		if (fields.empty())
			replyTo(user.getSocket(), ERR_NEEDMOREPARAMS(std::string("Guest"), "PASS"));
		else if (fields[0] == _password)
			user.setValidPass(true);
		else
			replyTo(user.getSocket(), ERR_PASSWDMISMATCH);
	}
	else
		replyTo(user.getSocket(), ERR_ALREADYREGISTERED(user.getNickname()));
}


void Server::nickCommand(const std::vector<std::string> &fields, Client &user) // can be resent after registration
{
	if (user.getValidPass() == true)
	{
		if (fields.empty())
		{
			replyTo(user.getSocket(), ERR_NONICKNAMEGIVEN(user.getNickname()));
			return;
		}
		std::map<int, Client>::const_iterator it;
		
		for (it = _clients.begin() ; it != _clients.end(); ++it)
		{
			if (stringUpper(it->second.getNickname()) == stringUpper(fields[0])) // dosnt get free when client leaves !! // nicknames, channel names casemapping sensitivity !!!
			{
				replyTo(user.getSocket(), ERR_NICKNAMEINUSE(fields[0]));
				return;
			}
		}
		// add a condition for the msg in case he is registerd and changed his name to smthing else !!!
		if (user.setNickname(fields[0]) == false)
			replyTo(user.getSocket(), ERR_ERRONEUSNICKNAME(fields[0]));
	}
	else
		replyTo(user.getSocket(), ERR_FIRSTCOMMAND);
}

void Server::userCommand(const std::string& message, const std::vector<std::string> &fields, Client &user)
{
	if (!user.getRegistered())
	{
		if (user.getValidPass())
		{
			if (fields.size() > 3) // real name may contain spaces
			{
				size_t p;
				if (( p = message.find_first_of(":")) == std::string::npos || fields[1] != "0" || fields[2] != "*" || !user.setUsername(fields[0]) || !user.setRealname(message.substr(p + 1)))
					replyTo(user.getSocket(), ERR_USERFORMAT);
			}
			else
				replyTo(user.getSocket(), ERR_NEEDMOREPARAMS(std::string("Guest"), "USER"));
		}
		else
			replyTo(user.getSocket(), ERR_FIRSTCOMMAND);
	}
	else
		replyTo(user.getSocket(), ERR_ALREADYREGISTERED(user.getNickname()));
}
