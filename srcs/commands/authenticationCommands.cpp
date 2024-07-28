/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authenticationCommands.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:25:38 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/26 05:01:14 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"
#include "../channel/channel.hpp"
#include "../client/client.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// make all commands args capital for comparisons 
// and channels chould rely on getnick name in case of nick name changes

void
Server::passCommand(const std::vector<std::string> &fields, Client &user)
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
};

void 
Server::nickCommand(const std::vector<std::string> &fields, Client &user)
{
	if (user.getValidPass() == true)
	{
		if (fields.empty())
		{
			const std::string &name = user.getNickname();
			return (replyTo(user.getSocket(), ERR_NONICKNAMEGIVEN((name.empty()) ? std::string("Guest") : name)));
		}
		for (clientIt it = _clients.begin() ; it != _clients.end(); ++it)
		{
			if (stringUpper(it->second.getNickname()) == stringUpper(fields[0]))
				return (replyTo(user.getSocket(), ERR_NICKNAMEINUSE(fields[0])));
		}
        std::string oldNick = user.getNickname();
		if (user.setNickname(fields[0]) == false)
			return (replyTo(user.getSocket(), ERR_ERRONEUSNICKNAME(fields[0])));
        else if (user.getRegistered()) // update in channels
            return (replyTo(user.getSocket(), CHANGENICK(oldNick, user.getUsername(), inet_ntoa(user.getAddr().sin_addr), fields[0])));
	}
	else
		return (replyTo(user.getSocket(), ERR_FIRSTCOMMAND));
	_clients[user.getSocket()].refStatus(_countCli);
};

void
Server::userCommand(const std::vector<std::string> &fields, Client &user)
{
	if (!user.getRegistered())
	{
		if (user.getValidPass())
		{
			if (fields.size() >= 4)
			{
                std::string realName = fields[3];
                if (realName.empty())
                    return (replyTo(user.getSocket(), ERR_NEEDMOREPARAMS(std::string("Guest"), "USER")));
                else if (!user.setUsername(fields[0])|| fields[1] != "0" || fields[2] != "*" || !user.setRealname(realName))
                    return (replyTo(user.getSocket(), ERR_USERFORMAT));
            }
			else
				return (replyTo(user.getSocket(), ERR_NEEDMOREPARAMS(std::string("Guest"), "USER")));
		}
		else
			return (replyTo(user.getSocket(), ERR_FIRSTCOMMAND));
	}
	else
		return (replyTo(user.getSocket(), ERR_ALREADYREGISTERED(user.getNickname())));
	_clients[user.getSocket()].refStatus(_countCli);
};

void
Server::privmsgCommand(const std::vector<std::string> &fields, Client &user)
{
    if (user.getRegistered())
    {
        if (fields.empty())
			replyTo(user.getSocket(), ERR_NORECIPIENT(user.getNickname(), "PRIVMSG"));
        else if (fields.size() >= 2)
        {
			if (fields[1].empty())
				return (replyTo(user.getSocket(), ERR_NOTEXTTOSEND(user.getNickname())));

            std::vector<std::string> clients = splitByDelim(fields[0],',');
			for (std::vector<std::string>::iterator split = clients.begin(); split != clients.end(); ++split) {
				const std::string &target = *split;
				if (target[0] == '#')
				{
					chnMapIt it = _channels.find(target);
					if (it != _channels.end())
						it->second.broadCast(PRIVMSG(user.getNickname(), user.getUsername(), inet_ntoa(user._addr.sin_addr), target, fields[1]), user.getSocket());
					else
						replyTo(user.getSocket(), ERR_NOSUCHNICK(user.getNickname(), target));
				}
				else
				{
					for (clientIt it = _clients.begin() ; it != _clients.end(); ++it)
					{
						// std::cout << it->second.getNickname() << " = " << target << std::endl;
						if (stringUpper(it->second.getNickname()) == stringUpper(target))
							return (replyTo(it->second.getSocket(), PRIVMSG(user.getNickname(), user.getUsername(), inet_ntoa(user._addr.sin_addr), it->second.getNickname(), fields[1])));
					}
					replyTo(user.getSocket(), ERR_NOSUCHNICK(user.getNickname(), target));
				}
            }
        }
        else
			replyTo(user.getSocket(), ERR_NOTEXTTOSEND(user.getNickname()));
    }
    else
        replyTo(user.getSocket(), ERR_NOTREGISTERED(user.getNickname()));
};
