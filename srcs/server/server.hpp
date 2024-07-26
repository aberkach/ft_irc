/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:50:26 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/26 05:28:40 by abberkac         ###   ########.fr       */
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
        uint16_t						_port;
		static bool						_signal;
        std::string                     _password;
        int                             _listen_sd;
        struct sockaddr_in              _addr;
        size_t                          _nfds;
        std::vector<pollfd>    	        _pollFds;
        std::map<int, Client>           _clients;
        std::map<std::string, Channel>  _channels; // list of channels in the server

  public:

        typedef void (Server::*CommandHandler)(const std::vector<std::string>&, Client&);
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
        void  passCommand(const std::vector<std::string> &fields, Client &client);
        void  nickCommand(const std::vector<std::string> &fields, Client &client);
        void  userCommand(const std::vector<std::string> &fields, Client &client);
        void  joinCommand(const std::vector<std::string> &fields, Client &client);
        bool  createChannel(const std::string &chnName, std::vector<std::string> &keys, Client &client);
        void  processTheJoinArgs(const std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client);
        bool  joinExistChannel(const std::string &chnName, std::vector<std::string> &keys, Client &client, const chnMapIt &chnIt);
        bool  privmsgChannel(const std::string &chnName, std::vector<std::string> &keys, Client &client, const chnMapIt &chnIt);
        
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
};

void Err(const std::string &msg, int exitFalg);
std::string trimTheSpaces(const std::string& str);
std::vector<std::string> splitBySpace(const std::string &str);
std::vector<std::string> splitByDelim(const std::string &str, char delim);
std::string stringUpper(const std::string &_str);
void display_channel_mode(const Channel &channel, const Client &client);
inline void replyTo(int socket, const std::string &buffer) {
	send(socket, buffer.c_str(), buffer.size(), 0);
}

#endif