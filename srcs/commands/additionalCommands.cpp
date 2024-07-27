/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additionalCommands.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 19:19:40 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/27 07:07:33 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"
#include "../channel/channel.hpp"
#include "../client/client.hpp"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

void
Server::pingCommad (const std::vector<std::string> &fields, Client &client)
{
    size_t size = fields.size();
    if (size >= 1 && fields[0] == SERVERNAME)
        replyTo(client.getSocket(),(client.getRegistered()) ? PONG(client.getNickname()) : PONG(std::string("Guest")));
    else if (size >= 1)
        replyTo(client.getSocket(), PONG(fields[0]));
    else 
        replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname() ,"PING"));
};

void
Server::pongCommad (const std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered() == false || fields.size() >= 1)
        return ;
    replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname() ,"PONG"));
};

void
Server::whoisCommad (const std::vector<std::string> &fields, Client &client)
{
    
};

// list command
void Server::listCommand (const std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered() == false) {
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
    } else if (fields.size() >= 1) {
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
void Server::partCommand (const std::vector<std::string> &fields, Client &client)
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
void Server::quitCommand(const std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered())
    {
        std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
        std::string quitMessage;
        if (fields.empty())
            quitMessage = QUIT_MSG(client.getNickname(), client.getUsername(), clientHost, "Client Quit");
        else
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
        std::map<int, Client>::iterator it =  _clients.find(client.getSocket());
        // remove the client from the clients map
        std::cout << RED << "Closing socket : " << YELLOW << it->first << RESET << std::endl;
        close(it->first);
        if (it != _clients.end())
            _clients.erase(it);
        _countCli--;
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}
