/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:19:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/22 18:53:01 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../channel/channel.hpp"
#include "../client/client.hpp"
#include <cstddef>
#include <string>
#include <vector>

// list command

void Server::listCommand (std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered() == false)
    {
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
        return;
    }
    if (!fields.empty())
    {
        std::vector<std::string> channels = splitByDelim(fields[0], ',');
        for (size_t i = 0; i < channels.size(); i++)
        {
            std::string chnName = channels[i];
            chnMapIt chnIt = _channels.find(chnName);
            if (chnIt != _channels.end())
            {
                if (chnIt->second.isClientExist(client.getNickname()))
                {
                    if (chnIt->second.getTopic() != "")
                        replyTo(client.getSocket(), LIST_MSG(chnIt->second.getName(), chnIt->second.getName(), std::to_string(chnIt->second.getUsers().size()), chnIt->second.getTopic()));
                    else
                        replyTo(client.getSocket(), LIST_MSG(chnIt->second.getName(), chnIt->second.getName(), std::to_string(chnIt->second.getUsers().size()), "No topic is set"));
                }
                else
                    replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chnName));
            }
            else
                replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(client.getNickname(), chnName));
        }
    }
    else
        replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "LIST"));
}


// part command
void Server::partCommand (std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered())
    {
        if (fields.empty())
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
        else
        {
            std::vector<std::string> channels = splitByDelim(fields[0], ',');
            for(size_t i = 0; i < channels.size(); i++)
            {
                std::string chnName = channels[i];
                chnMapIt chnIt = _channels.find(chnName);
                if (chnIt != _channels.end())
                {
                    if (chnIt->second.isClientExist(client.getNickname()))
                    {
                        std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                        replyTo(client.getSocket(), PART_MSG(client.getNickname(), client.getUsername(), clientHost, chnName, fields[1]));
                        if (fields.size() > 1)
                            chnIt->second.broadCast(PART_MSG(client.getNickname(), client.getUsername(), clientHost, chnName, fields[1]), client.getSocket());
                        else
                            chnIt->second.broadCast(PART_MSG(client.getNickname(), client.getUsername(), clientHost, chnName, "Client Quit"), client.getSocket());
                        chnIt->second.removeUser(client);
                    }
                    else
                        replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chnName));
                }
                else
                    replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(client.getNickname(), chnName));
            }
        }
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}

// quit command
void Server::quitCommand(std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered())
    {
        std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
        std::string quitMessage;
        if (fields.empty())
            quitMessage = QUIT_MSG(client.getNickname(), client.getUsername(), clientHost, "Client Quit");
        
        quitMessage = QUIT_MSG(client.getNickname(), client.getUsername(), clientHost, fields[0]);
        replyTo(client.getSocket(), quitMessage);
        for (chnMapIt it = _channels.begin(); it != _channels.end(); it++)
        {
            if (it->second.isClientExist(client.getNickname()))
            {
                it->second.broadCast(quitMessage, client.getSocket());
                it->second.removeUser(client);
            }
        }
        close(client.getSocket());
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}




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
                    chnIt->second.addUser(client);
                    if (chnIt->second.isInvited(client))
                        chnIt->second.removeInvite(client);
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
        // here we send a message to the client to inform him that he joined the channel and broadcast the message to the other users in the channel
        std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
        for (std::map<std::string, Client>::iterator it = chnIt->second.getUsers().begin(); it != chnIt->second.getUsers().end(); it++)
        {
            std::string usersList = chnIt->second.getChannelUsersInString();
            replyTo(it->second.getSocket(), RPL_JOIN(chnIt->second.getUserName(client.getNickname()), client.getUsername(), chnName, clientHost));
            // this replies to the client with the list of users in the channel and topic of the channel
            if (it->first == client.getNickname())
            {
                if (chnIt->second.getTopic() != "")
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
                

                for (std::map<std::string, Client>::iterator it = chnIt->second.getUsers().begin(); it != chnIt->second.getUsers().end(); it++)
                {
                    std::string usersList = chnIt->second.getChannelUsersInString();
                    
                    if (chnIt->second.getTopic() != "")
                        replyTo(it->second.getSocket(), RPL_TOPIC(chnIt->second.getUserName(client.getNickname()), chnName, chnIt->second.getTopic()));
                    replyTo(it->second.getSocket(), RPL_JOIN(client.getNickname(), client.getUsername(), chnName, clientHost));
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
        std::string oldNick = user.getNickname();
		if (user.setNickname(fields[0]) == false)
			return (replyTo(user.getSocket(), ERR_ERRONEUSNICKNAME(fields[0])));
        if (user.getRegistered())
            replyTo(user.getSocket(), CHANGENICK(oldNick, user.getUsername(), inet_ntoa(_addr.sin_addr), fields[0]));
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
				size_t p;// refigure for the : 
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

void Server::privmsgCommand(const std::string& message, std::vector<std::string> &fields, Client &user)
{
    if (user.getRegistered())
    {
        if (fields.empty())
			replyTo(user.getSocket(), ERR_NORECIPIENT(user.getNickname(), "PRIVMSG"));
        else if (fields.size() >= 2)
        {
            if (fields[0] == "$")
            {
                std::string msg = (fields[1][0] == ':') ? message.substr(message.find_first_of(":") + 1) : fields[1];
                if (msg.empty())
                    return (replyTo(user.getSocket(), ERR_NOTEXTTOSEND(user.getNickname())));

                for (std::map<int, Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
                    replyTo(it->second.getSocket(), PRIVMSG(user.getNickname(), user.getUsername(), inet_ntoa(user._addr.sin_addr), it->second.getNickname(), msg));
            }
            else if (fields[0][0] == '#')
            {


                std::map<std::string, Channel>::iterator it = _channels.find(fields[0]);
                if (it != _channels.end())
                {
                    if (fields[1][0] == ':')
                    {
                        std::string msg = message.substr(message.find_first_of(":") + 1);
                        if (msg.empty())
                            return (replyTo(user.getSocket(), ERR_NOTEXTTOSEND(user.getNickname())));
                        it->second.broadCast(PRIVMSG(user.getNickname(), user.getUsername(), inet_ntoa(user._addr.sin_addr), fields[0], msg), user.getSocket());
                    }
                    else
                        it->second.broadCast(PRIVMSG(user.getNickname(), user.getUsername(), inet_ntoa(user._addr.sin_addr), fields[0], fields[1]), user.getSocket());
                }
                else
                    replyTo(user.getSocket(), ERR_NOSUCHNICK(user.getNickname(), fields[0]));
            }
            else
            {
                for (std::map<int, Client>::const_iterator it = _clients.begin() ; it != _clients.end(); ++it)
                {
                    if (stringUpper(it->second.getNickname()) == stringUpper(fields[0]))
                    {
                        if (fields[1][0] == ':')
                        {
                            std::string msg = message.substr(message.find_first_of(":") + 1);
                            if (msg.empty())
                                return (replyTo(user.getSocket(), ERR_NOTEXTTOSEND(user.getNickname())));
                            replyTo(it->second.getSocket(), PRIVMSG(user.getNickname(), user.getUsername(), inet_ntoa(user._addr.sin_addr), it->second.getNickname(), msg));
                        }
                        else
                            replyTo(it->second.getSocket(), PRIVMSG(user.getNickname(), user.getUsername(), inet_ntoa(user._addr.sin_addr), it->second.getNickname(), fields[1]));
                        return;
                    }
                }
                replyTo(user.getSocket(), ERR_NOSUCHNICK(user.getNickname(), fields[0]));
            }
        }
        else
			replyTo(user.getSocket(), ERR_NOTEXTTOSEND(user.getNickname()));
    }
    else
        replyTo(user.getSocket(), ERR_NOTREGISTERED(user.getNickname()));
}