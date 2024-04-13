/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:19:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/13 16:30:23 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <string>
#include <vector>

// kick command

//TODO: this still on testing
void Server::kickCommand (std::vector<std::string> &fields, Client &client) {
    if (client.getRegistered()) {

        if (fields.size() < 2) {
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
            return;
        }
        std::string chnName = fields[0];
        std::string nickName = fields[1];
        std::vector<std::string> usersBeKicked;
        if (nickName.find_first_of(',') != std::string::npos)
            usersBeKicked = splitByDelim(nickName, ',');
        else
            usersBeKicked.push_back(nickName);
        chnMapIt chnIt = server_channels.find(chnName);
        if (chnIt == server_channels.end())
        {
            std::cout << "Error: channel doesn't exist" << std::endl;
            // here we can send an error message to the client
            // ....
            return;
        }
        if (chnIt->second.isOperator(client)) {
            for (size_t i = 0; i < usersBeKicked.size(); i++)
            {
                if (chnIt->second.isClientExist(usersBeKicked[i]))
                {
                    chnIt->second.removeUser(chnIt->second.getUser(usersBeKicked[i]));
                    std::cout << "kicked " << usersBeKicked[i] << " from " << chnName << std::endl;
                    // here we can send a message to the client to inform him that he is kicked
                    // ....
                }
                else
                {
                    std::cout << "Error: user doesn't exist" << std::endl;
                    // here we can send an error message to the client
                    // ....
                }
            }
        }
        else
        {
            std::cout << "Error: you are not an operator" << std::endl;
            // here we can send an error message to the client
            // ....
        }
    }
}

bool Server::createChannel(std::string &chnName, std::vector<std::string> &keys, Client &client) {
    // check if the channel already exist
    if (!server_channels.empty() && server_channels.find(chnName) != server_channels.end())
    {
        std::cout << "Error: channel already exist" << std::endl;
        // here we can send an error message to the client
        // ....
        if (keys.size() > 0)
            keys.erase(keys.begin());
        return false;
    }
    Channel newChannel(chnName);
    server_channels.insert(std::pair<std::string, Channel>(chnName, newChannel));
    // check if the channel has a key
    if (keys.size() > 0)
    {
        // check if the key is valid
        if (keys[0].find_first_of(" ,\a\b\f\t\v$:&+~%") != std::string::npos)
        {
            keys.erase(keys.begin());
            server_channels.erase(chnName);
            std::cout << "Error: invalid channel key" << std::endl;
            // here we can send an error message to the client
            // ....
            return false;
        }
        // set the key for the channel
        server_channels.find(chnName)->second.setKey(keys[0]);
        keys.erase(keys.begin());;
    }
    else
        server_channels.find(chnName)->second.setKey("");
    // make the client an operator of the channel
    server_channels.find(chnName)->second.addOperator(client);
    // add the client to the channel
    server_channels.find(chnName)->second.addUser(client);
    return true;
}

bool Server::joinChannel(std::string &chnName, std::vector<std::string> &keys, Client &client, chnMapIt &chnIt) {
    // if the channel doesn't exist, create a new one
    if (chnIt == server_channels.end())
    {
        if (!createChannel(chnName, keys, client))
            return false;
		std::cout << client.getNickname() << " created the channel " << chnName << std::endl;
        // here we can send a message to the client to inform him that the channel is created
        // ....
    }
    // if the channel already exist, check if the client is already in the channel
    else
    {
        // if the client is already in the channel, do nothing
        if (chnIt->second.isClientExist(client.getNickname()) == true)
        {
            // std::cout << "Error: you are already in the channel" << std::endl;
			std::cout << client.getNickname() << " is already in the channel " << chnName << std::endl;
            // here we can send an error message to the client
            // ....
            return false;
        }
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
                    // here we can send a message to the client to inform him that he joined the channel
                    // ....
                }
                // if the key is incorrect
                else
                {
                    std::cout << "Error: incorrect channel key" << std::endl;
                    // here we can send an error message to the client
                    // ....
                    keys.erase(keys.begin());
                    return false;
                }
            }
            // if the channel doesn't have a key, add the client to the channel
            else
            {
                // if the channel has no key, add the client to the channel
                if (chnIt->second.getKey() == "")
                {
                    chnIt->second.addUser(client);
					std::cout << client.getNickname() << " joined the channel " << chnName << std::endl;
                    // here we can send a message to the client to inform him that he joined the channel
                    // ....
                }
                else
                {
                    std::cout << "Error: channel has a key" << std::endl;
                    // here we can send an error message to the client
                    // ....
                    return false;
                }
            }
        }
    }
    return true;
}

void Server::processTheJoinArgs(std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::string chnName = channels[i];
        // check if the channel name is valid
        if ((chnName[0] != '#') || (chnName.find_first_of(" ,\a\b\f\t\v$:+~%") != std::string::npos))
        {
            std::cout << "Error: invalid channel name" << std::endl;
            // here we can send an error message to the client
            // ....
            if (keys.size() > 0)
                keys.erase(keys.begin());
            continue;
        }
        else
        {
            // check if the channel already exist
            chnMapIt chnIt = server_channels.find(chnName);
            
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
	else
	{
		std::cout << "u need to registere first" << std::endl;
		// here we can send an error message to the client
		// ....
	}
}
 

void Server::passCommand(const std::vector<std::string> &fields, Client &user)
{
	if (user.getValidPass() == false)
	{
		if (fields.empty())
			replyTo(user.getSocket(), ERR_NEEDMOREPARAMS("Guest", "PASS"));
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
				replyTo(user.getSocket(), ERR_NEEDMOREPARAMS("Guest", "USER"));
		}
		else
			replyTo(user.getSocket(), ERR_FIRSTCOMMAND);
	}
	else
		replyTo(user.getSocket(), ERR_ALREADYREGISTERED(user.getNickname()));
}
