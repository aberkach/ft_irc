/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:19:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/21 22:30:03 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"
#include "../channel/channel.hpp"
#include "../client/client.hpp"
#include <cstddef>
#include <iostream>
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
    if (fields.size() >= 1)
    {
        std::vector<std::string> chnName = splitByDelim(fields[0], ',');
        replyTo(client.getSocket(), RPL_LISTSTART(client.getNickname()));
        for (size_t i = 0; i < chnName.size(); i++)
        {
            chnMapIt chnIt = _channels.find(chnName[i]);
            if (chnIt != _channels.end())
            {
                if (chnIt->second.getTopic() != "")
                    replyTo(client.getSocket(), LIST_MSG(client.getNickname(), chnName[i], std::to_string(chnIt->second.getUsers().size()), chnIt->second.getTopic()));
                else
                    replyTo(client.getSocket(), LIST_MSG(client.getNickname(), chnName[i], std::to_string(chnIt->second.getUsers().size()), "No topic set"));
            }
            else
                replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(client.getNickname(), chnName[i]));
        }
        replyTo(client.getSocket(), RPL_LISTEND(client.getNickname()));
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
                        std::string reason = (fields.size() > 1) ? fields[1] : "Client Quit";
                        replyTo(client.getSocket(), PART_MSG(client.getNickname(), client.getUsername(), clientHost, chnName, reason));
                        chnIt->second.broadCast(PART_MSG(client.getNickname(), client.getUsername(), clientHost, chnName, reason), client.getSocket());
                        if (chnIt->second.getUsers().size() == 1)
                            _channels.erase(chnIt);
                        else
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
        // remove the client from the clients map
        //.....
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}
