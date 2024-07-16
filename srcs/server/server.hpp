/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:50:26 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/16 03:26:32 by abberkac         ###   ########.fr       */
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

// define channel map iterator
typedef std::map<std::string, Channel>::iterator chnMapIt;

class Server {
  // command dispatching mechanism
  
  private:
        uint16_t	                    _port;
		static bool						_signal;
        std::string                     _password;
        int                             _listen_sd;
        struct sockaddr_in              _addr;
        size_t                          _nfds;
        std::vector<pollfd>    	        _pollFds;
        std::map<int, Client>           _clients;
        std::map<std::string, Channel>  _channels; // list of channels in the server

  public:

        typedef void (Server::*CommandHandler)(std::vector<std::string>&, Client&);
        std::map<std::string, CommandHandler> _commands;
        Server(void);
        Server(uint16_t port, char *password);

        unsigned int  getPort() const { return _port; }
        std::string   getPassword() const { return _password; }

        void   createServer();
		static void  sigHandler(int sigNumber);
		void  cleanUp();
        void  handlIncomeConnections();
        void  handleIncomeData(int i);

        void commandRunner(std::vector<std::string> &fields, Client &cling);

        ~Server();

      private:
        void  passCommand(std::vector<std::string> &fields, Client &client);
        void  nickCommand(std::vector<std::string> &fields, Client &client);
        void  userCommand(std::vector<std::string> &fields, Client &client);
        void  joinCommand(std::vector<std::string> &fields, Client &client);
        bool  createChannel(std::string &chnName, std::vector<std::string> &keys, Client &client);
        void  processTheJoinArgs(std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client);
        bool  joinChannel(std::string &chnName, std::vector<std::string> &keys, Client &client, chnMapIt &);
        bool  privmsgChannel(std::string &chnName, std::vector<std::string> &keys, Client &client, chnMapIt &);
        
        void  privmsgCommand(std::vector<std::string> &fields, Client &user);

        // operators methods :

        void kickCommand(std::vector<std::string> &fields, Client &client);
        void topicCommand(std::vector<std::string> &fields, Client &client);
        void inviteCommand(std::vector<std::string> &fields, Client &client);
        void quitCommand(std::vector<std::string> &fields, Client &client);
        void partCommand(std::vector<std::string> &fields, Client &client);
        void listCommand(std::vector<std::string> &fields, Client &client);
        void modeCommand(std::vector<std::string> &fields, Client &client);
        size_t countUsersInChannel(std::string &chnName);
};


std::vector<std::string> splitBySpace(std::string str);
std::vector<std::string> splitByDelim(std::string str, char delim);
std::string stringUpper(const std::string &_str);

inline void replyTo(int socket, std::string buffer) {
	send(socket, buffer.c_str(), buffer.size(), 0);
}

#endif