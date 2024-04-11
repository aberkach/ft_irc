#ifndef SERVER_HPP__
#define SERVER_HPP__

#include "../../Inc/define.hpp"
// #include "../../Inc/ft_irc.hpp"
#include "../channel/channel.hpp"
#include "../client/client.hpp"

#include <cstddef>
#include <sys/poll.h>
#include <vector>

class Server {
  private:
        uint16_t	              _port;
        std::string             _password;
        int                     _listen_sd;
        struct sockaddr_in      _addr;
        size_t                  _nfds;
        std::vector<pollfd>    	_fds;
        std::map<int, Client>   _clients;
        std::map<std::string, Channel> server_channels; // list of channels in the server

  public:
        Server(void);
        Server(uint16_t port, char *password);

        unsigned int  getPort() const { return _port; }
        std::string   getPassword() const { return _password; }

        int   createServer();
        void  handlIncomeConnections();
        void  handleIncomeData();

        void commandList(const std::string& message, std::vector<std::string> &fields, Client &cling);

        // void updateFileDescrior(int *ng);

        ~Server();

      private: // these will be stand alone functions undifined in the class
        void  passCommand(const std::vector<std::string> &fields, Client &user);
        void  nickCommand(const std::vector<std::string> &fields, Client &user);
        void  userCommand(const std::string& message, const std::vector<std::string> &fields, Client &user);
        int   createChannel(std::string &chnName, std::vector<std::string> &keys, Client &client);
        void  joinCommand(std::vector<std::string> &fields, Client &client);
        void  processTheJoinArgs(std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client);

};

#endif