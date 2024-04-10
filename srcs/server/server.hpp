#ifndef SERVER_HPP__
#define SERVER_HPP__

#include "../../Inc/define.hpp"
#include "../../Inc/ft_irc.hpp"
#include "../channel/channel.hpp"

#include <cstddef>
#include <sys/poll.h>
#include <vector>

class Server {
  private:
        uint16_t	              _port;
        std::string             _password;
        int                     _listen_sd;
        struct sockaddr_in      _addr;
        std::vector<pollfd>    	_fds;
        size_t                  _nfds;
        std::map<int, Client>	_clients;

      //   std::map<std::string, Channel> server_channels; // list of channels in the server

  public:
        Server(void);
        Server(uint16_t port, char *password);

        unsigned int  getPort() const { return _port; }
        std::string   getPassword() const { return _password; }

        int   createServer();
        void  handlIncomeConnections();
        void  handleIncomeData();

        void commandList(std::string& message, std::vector<std::string> &fields, Client &cling);

        // void updateFileDescrior(int *ng);

        ~Server();

      private:
        // static void passCommandStatic(Server* server, std::vector<std::string> &fields, Client &user) {
        //   server->passCommand(fields, user);
        // }

        // static void nickCommandStatic(Server* server, std::vector<std::string> &fields, Client &user) {
        //     server->nickCommand(fields, user);
        // }

        // static void userCommandStatic(Server* server, std::vector<std::string> &fields, Client &user) {
        //     server->userCommand(fields, user);
        // }

        void passCommand(std::vector<std::string> &fields, Client &user);
        void nickCommand(std::vector<std::string> &fields, Client &user);
        void userCommand(std::string& message, std::vector<std::string> &fields, Client &user);


};

#endif