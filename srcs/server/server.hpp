/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:50:26 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/30 19:36:31 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP__
#define SERVER_HPP__

#include "../../Inc/define.hpp"
#include "../channel/channel.hpp"
#include <cstddef>
#include <sys/poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <arpa/inet.h>

typedef std::map<std::string, Channel>::iterator chnMapIt;

class Server {  
  private:
        typedef std::map<int, Client>::iterator clientIt;
        typedef void (Server::*CommandHandler)(const std::vector<std::string>&, Client&);

		    static bool						                _signal;
        uint                                  _countCli;
        uint16_t						                  _port;
        std::string                           _password;
        int                                   _listen_sd;
        std::map<std::string, CommandHandler> _commands;
        struct sockaddr_in                    _addr;
        size_t                                _nfds;
        std::vector<pollfd>    	              _pollFds;
        std::map<int, Client>                 _clients;
        std::map<std::string, Channel>        _channels; // list of channels in the server

      private:
        Server(void);
        std::string extractModeString(const std::string &modeField, Client &client);
        // void  executeModes(const std::vector<std::string> &fields, Client &Client, chnMapIt it, char *mode);
        void  displayChannelMode(const Channel &channel, const Client &client);
        void  passCommand(const std::vector<std::string> &fields, Client &client);
        void  nickCommand(const std::vector<std::string> &fields, Client &client);
        void  userCommand(const std::vector<std::string> &fields, Client &client);
        void  pingCommad(const std::vector<std::string> &fields, Client &client);
        void  pongCommad(const std::vector<std::string> &fields, Client &client);
        void  namesCommad(const std::vector<std::string> &fields, Client &client);
        void  joinCommand(const std::vector<std::string> &fields, Client &client);
        bool  createChannel(const std::string &chnName, std::vector<std::string> &keys, Client &client);
        void  processTheJoinArgs(const std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client);
        bool  joinExistChannel(const std::string &chnName, std::vector<std::string> &keys, Client &client, const chnMapIt &chnIt);
        void  privmsgCommand(const std::vector<std::string> &fields, Client &user);
        // operators methods :
        void kickCommand(const std::vector<std::string> &fields, Client &client);
        void topicCommand(const std::vector<std::string> &fields, Client &client);
        void inviteCommand(const std::vector<std::string> &fields, Client &client);
        void quitCommand(const std::vector<std::string> &fields, Client &client);
        void partCommand(const std::vector<std::string> &fields, Client &client);
        void listCommand(const std::vector<std::string> &fields, Client &client);
        void modeCommand(const std::vector<std::string> &fields, Client &client);
        size_t countUsersInChannel(const std::string &chnName);
        std::vector<std::string> getBuffers(const std::string &buffer);

        bool isClientInServer(const std::string &nickName);
  public:
        Server(uint16_t port, char *password);

        unsigned int getPort() const { return _port; }
        std::string  getPassword() const { return _password; }
		    static void  sigHandler(int sigNumber);

        void  createServer();
        void  handlIncomeConnections();
        void  handleIncomeData(int i);
        void  commandRunner(std::vector<std::string> &fields, Client &cling);
		    void  cleanUp();

        ~Server();
};

void Err(const std::string &msg);
std::string trimTheSpaces(const std::string& str);
std::vector<std::string> splitBySpace(const std::string &str);
std::vector<std::string> splitByDelim(const std::string &str, char delim);
std::string stringUpper(const std::string &_str);
void display_channel_mode(const Channel &channel, const Client &client);
inline void replyTo(int socket, const std::string &buffer) {
	send(socket, buffer.c_str(), buffer.size(), 0); // protect
}

#endif