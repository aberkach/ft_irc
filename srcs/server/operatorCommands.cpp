/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operatorCommands.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yamajid <yamajid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 23:33:20 by abberkac          #+#    #+#             */
/*   Updated: 2024/05/20 18:06:55 by yamajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <cstddef>
#include <vector>
#include "../client/client.hpp"
#include "../channel/channel.hpp"


void Server::inviteCommand(std::vector<std::string> &fields, Client &client) {
    if (fields.size() < 2) {
        replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
        return;
    }
    if (client.getRegistered()) {
        std::string invitedUser = fields[0];
        std::string chnName = fields[1];
        chnMapIt chnIt = _channels.find(chnName);
        if (chnIt == _channels.end())
        {
            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName));
            return;
        }
        if (chnIt->second.isClientExist(client.getNickname()))
        {
            // here we send a message to the client that has been invited
            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            std::string inviteMessage = RPL_INVITED(client.getNickname(), client.getUsername(), clientHost, invitedUser, chnName);
            replyTo(client.getSocket(), RPL_INVITING(client.getNickname(), invitedUser, chnName));
            for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
            {
                if (it->second.getNickname() == invitedUser)
                {
                    chnIt->second.addInvite(it->second);
                    replyTo(it->second.getSocket(), inviteMessage);
                }
            }
        }
        else
            replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chnName));
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
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
            chnMapIt chnIt = _channels.find(chnName);
            if (chnIt == _channels.end())
            {
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName));
                return;
            }
            if (chnIt->second.isOperator(client.getNickname())) {
                topic = fields[1];
                chnIt->second.setTopic(topic);
                std::string clintHost = inet_ntoa(client.getAddr().sin_addr);
                chnIt->second.broadCast(RPL_TOPICSETBY(client.getNickname(), client.getUsername(), clintHost, chnName, chnIt->second.getTopic()), -1);
            }
            else
                replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), chnName));
        }
        // if the client wants to get the topic of the channel
        else if (fields.size() == 1)
        {
            chnMapIt chnIt = _channels.find(chnName);
            if (chnIt == _channels.end())
            {
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName));
                return;
            }
            if (chnIt->second.isClientExist(client.getNickname()))
            {
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                // need to send the topic to the client
                topic = chnIt->second.getTopic();
                std::cout << "topic: " << topic << std::endl;
                replyTo(client.getSocket(), RPL_TOPICSETBY(client.getNickname(), client.getUsername(), clientHost, chnName, topic));
            }
            else
                replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chnName));
                
        }
        else
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}


// kick command
void Server::kickCommand (std::vector<std::string> &fields, Client &client) {
    if (client.getRegistered()) {
        
        if (fields.size() < 2) {
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
            return;
        }
        std::string chnName = fields[0];
        std::vector<std::string> usersBeKicked = splitByDelim(fields[1], ',');
        chnMapIt joinedChnIt = _channels.find(chnName);
        if (joinedChnIt == _channels.end())
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
        if (joinedChnIt->second.isOperator(client.getNickname())) {
            for (size_t i = 0; i < usersBeKicked.size(); i++)
            {
                if (joinedChnIt->second.isClientExist(usersBeKicked[i]))
                {
                    // send a message to the client that has been kicked
                    std::string reason;
                    if (fields.size() > 2)
                        reason = fields[2];
                    else
                        reason = client.getNickname();
                    std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                    std::string KickErrMessage = RPL_KICK(client.getNickname(), client.getRealname(), clientHost, chnName, usersBeKicked[i], reason);
                    
                    for (std::map<std::string, Client>::iterator it = joinedChnIt->second.getUsers().begin(); it != joinedChnIt->second.getUsers().end(); it++)
                    {
                        if (it->first == usersBeKicked[i])
                            continue;
                        replyTo(it->second.getSocket(), KickErrMessage);
                    }
                    if (joinedChnIt->second.getUsers().size() == 1)
                        _channels.erase(joinedChnIt);
                    else
                        joinedChnIt->second.removeUser(joinedChnIt->second.getUser(usersBeKicked[i]));
                    
                }
                // if the client is not in the channel, send an error message to the client
                else if (joinedChnIt->second.isClientExist(usersBeKicked[i]) == false)
                    replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), usersBeKicked[i], chnName));
            }
        }
        else
            replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), chnName));
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}
