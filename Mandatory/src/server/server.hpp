/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:50:26 by abberkac          #+#    #+#             */
/*   Updated: 2024/08/13 04:36:04 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include <cstddef>
#include <sys/poll.h>
#include <vector>
#include <map>
// #include <sstream>

class Client;
class Channel;

class Server
{
    private:
        typedef std::map<int, Client>::iterator clientIt;
        typedef std::map<std::string, Channel>::iterator chanIt;
        typedef void (Server::*CommandHandler)(const std::vector<std::string>&, Client&);

		static bool						      _signal;
        uint                                  _countCli;
        uint16_t						      _port;
        std::string                           _password;
        int                                   _listen_sd;
        std::map<std::string, CommandHandler> _commands;
        struct sockaddr_in                    _addr;
        size_t                                _nfds;
        std::vector<pollfd>    	              _pollFds;
        std::map<int, Client>                 _clients;
        std::map<std::string, Channel>        _channels;

    private:
        Server(void);

        void                      handlIncomeConnections(void);
        void                      handleIncomeData(int i);
        void                      displayChannelMode(const Channel &channel, const Client &client);
        void                      processTheJoinArgs(const std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client);
        void                      modeSetReply(Client& clinet, Channel &channel, std::string& modes, const std::vector<std::string> &fields);
        void                      executeModes(const std::vector<std::string> &fields, Client &client, chanIt it);
		void                      cleanUp(void);
		static void               sigHandler(int sigNumber);
        bool                      createChannel(const std::string &chnName, std::vector<std::string> &keys, Client &client);
        bool                      isClientInServer(const std::string &nickName);
        bool                      joinExistChannel(const std::string &chnName, std::vector<std::string> &keys, Client &client, const chanIt &chnIt);
        size_t                    countUsersInChannel(const std::string &chnName);
        std::string               extractModeString(const std::string &modeField, Client &client);
        std::vector<std::string>  getBuffers(const std::string &buffer);

        void                      commandRunner(std::vector<std::string> &fields, Client &cling);
        void                      passCommand(const std::vector<std::string> &fields, Client &client);
        void                      nickCommand(const std::vector<std::string> &fields, Client &client);
        void                      userCommand(const std::vector<std::string> &fields, Client &client);
        void                      pingCommad(const std::vector<std::string> &fields, Client &client);
        void                      pongCommad(const std::vector<std::string> &fields, Client &client);
        void                      namesCommad(const std::vector<std::string> &fields, Client &client);
        void                      joinCommand(const std::vector<std::string> &fields, Client &client);
        void                      privmsgCommand(const std::vector<std::string> &fields, Client &user);
        void                      kickCommand(const std::vector<std::string> &fields, Client &client);
        void                      topicCommand(const std::vector<std::string> &fields, Client &client);
        void                      inviteCommand(const std::vector<std::string> &fields, Client &client);
        void                      quitCommand(const std::vector<std::string> &fields, Client &client);
        void                      partCommand(const std::vector<std::string> &fields, Client &client);
        void                      listCommand(const std::vector<std::string> &fields, Client &client);
        void                      modeCommand(const std::vector<std::string> &fields, Client &client);

  public:
        Server(uint16_t port, char *password);

        void    createServer();

        ~Server();
};
