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

void topicFlag(Channel &channel, bool sign, std::vector<std::string> args)
{
	(void) args;
	if (sign)
		channel.setTopicFlag(true);
	else
		channel.setTopicFlag(false);
}

void operatorFlag(Client &client, chnMapIt &channel, bool sign, std::vector<std::string>& args)
{
	std::map<std::string, Client> it = channel->second.getUsers();
	if (sign)
	{
		if (it.find(args[0]) == it.end())
			replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), args[0], channel->first));
		else if (channel->second.isOperator(args[0]))
			replyTo(client.getSocket(), RPL_WHOISOPERATOR(args[0]));
		else{	
			channel->second.addOperator(it[args[0]]);
			args.erase(args.begin());
			replyTo(client.getSocket(), RPL_YOUREOPER(client.getNickname()));
		}
	}
	else{
		if ((it.find(args[0])) == it.end())
			replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), args[0], channel->first));
		else {	
			channel->second.removeOperator(it[args[0]]);
			args.erase(args.begin()); //reply client is now just a user
		}
	}
}

void	keyWordFlag(Channel &channel, bool sign, std::vector<std::string>& args)
{
	if (sign)
	{
		channel.setKey(args[0]);
		args.erase(args.begin()); //reply key is  set
	}
	else
		channel.setKey(""); //reply key is  removed
}

void invetOnlyFlag(Channel &channel, bool sign)
{
	if (sign)
		channel.setIsInviteOnly(true); //reply channel is now invite only
	else
		channel.setIsInviteOnly(false); //reply channel is now not invite only
}

void limitFlag(Channel &channel, bool sign, std::vector<std::string> &args)
{
	if (sign)
	{
		std::stringstream  ss(args[0]);
		size_t limit;
		if (!(ss >> limit) || !ss.eof()){
			args.erase(args.begin());
			return ; //reply limit must be a number
		}
		channel.setMaxUsers(limit);
		args.erase(args.begin());
		//reply limit is now set
	}
	else{
		channel.setMaxUsers(0);
		//reply limit is now removed
	}
}


int check_flag_string(const std::string &flags){
	if (flags[0] != '+' && flags[0] != '-')
		return 1;
	for (size_t i = 0; i < flags.size(); i++){
		for (size_t j = i + 1; j < flags.size(); j++)
			if ((flags[i] == flags[j] && i != j) || 
				(flags[i] != 'k' && flags[i] != 'o' && flags[i] != 'i' && flags[i] != 'l' && flags[i] != 't' && flags[i] != '+' && flags[i] != '-'))
					return 1;
	}
	return 0;
}

int check_params(const std::vector<std::string> &args, const std::string &flags){
	bool sign = false;
	size_t count = 0;
	for(size_t i = 0; i < flags.size(); i++){
		if (flags[i] == '+')
			sign = true;
		else if (flags[i] == '-')
			sign = false;
		if (flags[i] == 'o')
			count++;
		else if (flags[i] == 'k' && sign == true)
			count++;
		else if (flags[i] == 'l' && sign == true)
			count++;
	}
	if (count != args.size())
		return 1;
	return 0;
}

void display_channel_mode(const Channel &channel, const Client &client){
	std::string str;
	str += "+";
	if (channel.getIsInviteOnly() == true)
		str += "i";
	if (!channel.getKey().empty())
		str += "k";
	if (channel.getMaxUsers() != 0)
	{
		std::cout << "max users: " << channel.getMaxUsers() << std::endl;
		str += "l";
	}
	if (!channel.getTopic().empty())
		str += "t";
	replyTo(client.getSocket(), RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), str));
}

std::string get_host(const struct sockaddr_in &addr)
{
	std::stringstream ss;
    ss << inet_ntoa(addr.sin_addr);
    return ss.str();
}

void Server::modeCommand(const std::vector<std::string> &fields, Client &client){
	bool sign = false;
	std::vector<std::string> args;
	
	if (fields[0].empty())
		replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
	chnMapIt it = _channels.find(fields[0]);
	if (it == _channels.end()){
		replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(client.getNickname(), fields[0]));
		return ;
	}
	if (!it->second.isClientExist(client.getNickname())){
		replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), client.getNickname(), it->first));
		return ;
	}
	if (fields.size() == 1){
		display_channel_mode(it->second, client);
		return ; 
	}
	if (!it->second.isOperator(client.getNickname())){
		replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), it->first));
		return ;
	}
	if (check_flag_string(fields[1]) == 1){
		replyTo(client.getSocket(), ERR_UNKNOWNMODE(client.getNickname()));
		return ;
	}
	for(size_t i = 2; i < fields.size(); i++)
		if (!fields[i].empty())
			args.push_back(fields[i]);
	if (check_params(args, fields[1]) == 1){
		replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
		return ;
	}
	size_t i = 0;
	for(; i < fields[1].size(); i++){
		if (fields[1][i] == '+') sign = true;
		else if (fields[1][i] == '-') sign = false;
		else if (fields[1][i] == 'o') operatorFlag(client, it, sign, args);
		else if (fields[1][i] == 'k') keyWordFlag(it->second, sign, args);
		else if (fields[1][i] == 'i') invetOnlyFlag(it->second, sign);
		else if (fields[1][i] == 'l') limitFlag(it->second, sign, args);
		else if (fields[1][i] == 't') topicFlag(it->second, sign, args);
	}
	replyTo(client.getSocket(), MODE_SET(client.getNickname(), client.getUsername(), get_host(_addr) ,it->first, fields[1]));
}