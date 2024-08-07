/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 23:25:07 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/25 05:20:20 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"
#include "../../Inc/ft_irc.hpp"
#include <cstddef>
#include <string>
#include <sys/poll.h>
#include <vector>
#include <limits>

void
Server::displayChannelMode(const Channel &channel, const Client &client)
{// show the channel modes 324 + 329 
	std::string params = "+";
	std::string applied = " ";

	if (channel.getTopicFlag())
		params += "t";
	if (channel.getIsInviteOnly())
		params += "i";
	if (channel.getMaxUsers() != 0)
	{
		params += "l";
		applied += std::to_string(channel.getMaxUsers()) + " ";
	}
	if (!channel.getKey().empty())
	{
		params += "k";
		applied += channel.getKey();
	}

	replyTo(client.getSocket(), RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), (params + applied)));
	// replyTo(client.getSocket(), RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), (params + applied)));
}

std::string get_host(const struct sockaddr_in &addr)
{
	std::stringstream ss;
    ss << inet_ntoa(addr.sin_addr);
    return ss.str();
}

#define __KNOWNMODECHAR(charMode) ( 				   \
					    			charMode == 'i' || \
					    			charMode == 't' || \
					    			charMode == 'k' || \
					    			charMode == 'o' || \
					    			charMode == 'l'    \
								  )


std::string
Server::extractModeString(const std::string &modeField, Client &client)
{
    std::set<char> modes;
    std::string modesString;
    char sign = (modeField[0] == '-') ? '-' : '+';

    for (size_t i = 0; i < modeField.size(); i++)
	{
        while (modeField[i] && modeField[i] == sign)
            i++;
        if (modeField[i] != sign && (modeField[i] == '-' || modeField[i] == '+'))
            sign = modeField[i];
        else if __KNOWNMODECHAR(modeField[i])
		{
			if (modes.find(modeField[i]) == modes.end() || modeField[i] == 'o')
			{
                modesString += sign;
                modesString += modeField[i];
				modes.insert(modeField[i]);
			}
        }
		else if (modeField[i] != '-' && modeField[i] != '+')
            replyTo(client.getSocket(), ERR_UNKNOWNMODE(client.getNickname(), modeField[i]));
    }

    return modesString;
};


// broadcast the mode changes to all client in the channel
void
Server::modeCommand(const std::vector<std::string> &fields, Client &client)
{
	if (client.getRegistered())
	{
		if (fields.empty())
			replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		chnMapIt it = _channels.find(fields[0]);
		if (it != _channels.end())
		{
			int size = fields.size();
			switch (size)
			{
				case 1: 
					displayChannelMode(it->second, client);
					break;

				default:
					if (it->second.isOperator(client.getNickname()))
					{
						std::string modestr = extractModeString(fields[1], client);

						size_t arg = 2;
						char sign = '\0';
						std::string	appliedModes;
						std::vector<std::string> appliedFields;
						
						for (size_t i = 0; i < modestr.size(); i += 2)
						{
							bool set = (modestr[i] == '+');
							if (sign != modestr[i])
							{
								sign = modestr[i];
								appliedModes += sign;
							}

							switch (modestr[i + 1])
							{
								case 'i':
									appliedModes += "i";
									it->second.setIsInviteOnly(set);
									break;
								case 't':
									appliedModes += "t";
									it->second.setTopicFlag(set);
									break;
								case 'k':
									if (set && arg < fields.size())
									{
										it->second.setKey(fields[arg]);
										appliedModes += "k";
										appliedFields.push_back(fields[arg]);
										arg++;
									}
									else if (!set)
										it->second.setKey("");
									// else
									// 	replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "+k"));
									break;
								case 'o':
									if (arg < fields.size())
									{
										if (it->second.isClientExist(fields[arg]))
										{
											if (set)
												it->second.addOperator(it->second.getUser(fields[arg]));
											else
												it->second.removeOperator(it->second.getUser(fields[arg]));
											appliedModes += "o";
											appliedFields.push_back(fields[arg]);
										}
										else
											replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), fields[arg], it->first));
										arg++;
									}
									// else
										// replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "+o"));
									break;
								case 'l':
									if (set && arg < fields.size())
									{
										long maxUsers = std::atol(fields[arg].c_str());
										if (maxUsers > 0 && fields.size() <= 10 && maxUsers <= std::numeric_limits<int>::max())
										{
											it->second.setMaxUsers(maxUsers);
											appliedModes += "l";
											appliedFields.push_back(fields[arg]);
										}
										arg++;
									}
									else if (!set) {
										appliedModes += "l";
										it->second.setMaxUsers(0);
									}
									else
										replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "+l"));
									break;
								default:
									break;
							}
						}
						// it->second.broadCast(MODE_SET() , -1)
					}
					else
						replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), fields[0]));
					break;
			}
		}
		else
			replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(client.getNickname(), "MODE"));
	}
	else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(std::string("GUEST")));

};
