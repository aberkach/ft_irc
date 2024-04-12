/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:19:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/12 19:22:42 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


	// cant start with :
		// Dollar ($, 0x24)
		// Colon (:, 0x3A)
		// Any character listed as a channel type (#, &)
		// Any character listed as a channel membership prefix (@, ~, &, %, +)

int Server::createChannel(std::string &chnName, std::vector<std::string> &keys, Client &client) {
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
            return 1;
        }
        // set the key for the channel
        server_channels.find(chnName)->second.setKey(keys[0]);
        keys.erase(keys.begin());;
    }
    else
        server_channels.find(chnName)->second.setKey("");
    // add the client to the channel
    server_channels.find(chnName)->second.addUser(client);
    return 0;
}

void Server::processTheJoinArgs(std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::string chnName = channels[i];
        // check if the channel name is valid
        if ((chnName[0] != '#') || (chnName.find_first_of(" ,\a\b\f\t\v$:&+~%") != std::string::npos))
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
            std::map<std::string, Channel>::iterator chnIt = server_channels.find(chnName);
            // if the channel doesn't exist, create a new one
            if (chnIt == server_channels.end())
            {
                if (createChannel(chnName, keys, client))
                    continue;
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
                    continue;
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
                            continue;
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
                            continue;
                        }
                    }
                }
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
    	    channels = split(fields[0], ',');
    	    keys = split(fields[1], ',');
    	    // process the channels and keys
    	    processTheJoinArgs(channels, keys, client);
    	}
    	else
    	{
    	    std::cout << "Error: empty args" << std::endl;
    	    // here we can send an error message to the client
    	    // ....
    	}
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
