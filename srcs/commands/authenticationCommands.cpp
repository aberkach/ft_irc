/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authenticationCommands.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:25:38 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/25 07:17:24 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"
#include "../channel/channel.hpp"
#include "../client/client.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

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
	_clients[user.getSocket()].refStatus();
}


void Server::nickCommand(const std::vector<std::string> &fields, Client &user) // relook
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
            replyTo(user.getSocket(), CHANGENICK(oldNick, user.getUsername(), inet_ntoa(user.getAddr().sin_addr), fields[0]));
	}
	else
		replyTo(user.getSocket(), ERR_FIRSTCOMMAND);
	_clients[user.getSocket()].refStatus();
}

void Server::userCommand(const std::vector<std::string> &fields, Client &user)
{
	if (!user.getRegistered())
	{
		if (user.getValidPass())
		{
			if (fields.size() >= 4)
			{
                std::string realName = fields[3];
                if (realName.empty())
                    replyTo(user.getSocket(), ERR_NEEDMOREPARAMS(std::string("Guest"), "USER"));
                else if (!user.setUsername(fields[0])|| fields[1] != "0" || fields[2] != "*" || !user.setRealname(realName))
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
	_clients[user.getSocket()].refStatus();
}

void Server::privmsgCommand(const std::vector<std::string> &fields, Client &user)
{
    if (user.getRegistered())
    {
        if (fields.empty())
			replyTo(user.getSocket(), ERR_NORECIPIENT(user.getNickname(), "PRIVMSG"));
        else if (fields.size() >= 2)
        {
            std::string msg = fields[1];
            if (msg.empty())
                return (replyTo(user.getSocket(), ERR_NOTEXTTOSEND(user.getNickname())));
            if (fields[0][0] == '#')
            {
                std::map<std::string, Channel>::iterator it = _channels.find(fields[0]); // check the validity of this for upper and unickness

                if (it != _channels.end())
                    it->second.broadCast(PRIVMSG(user.getNickname(), user.getUsername(), inet_ntoa(user._addr.sin_addr), fields[0], msg), user.getSocket());
                else
                    replyTo(user.getSocket(), ERR_NOSUCHNICK(user.getNickname(), fields[0]));
            }
            else
            {
                for (std::map<int, Client>::const_iterator it = _clients.begin() ; it != _clients.end(); ++it)
                {
					std::cout << it->second.getNickname() << " = ";
					std::cout << fields[0] << std::endl;
                    if (stringUpper(it->second.getNickname()) == stringUpper(fields[0]))
                        return (replyTo(it->second.getSocket(), PRIVMSG(user.getNickname(), user.getUsername(), inet_ntoa(user._addr.sin_addr), it->second.getNickname(), msg)));
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

//server always send ping and a msg if u dont respond 
// PING :cadmium.libera.chat
// xxx!~sss@197.230.30.146 QUIT :Ping timeout: 245 seconds
// ERROR :Closing Link: 197.230.30.146 (Ping timeout: 245 seconds)