#ifndef SERVER_HPP__
#define SERVER_HPP__

#include "../../Inc/define.hpp"
#include "../../Inc/ft_irc.hpp"

#include <cstddef>
#include <sys/poll.h>
#include <vector>
#include <string>

#include "../channel/channel.hpp"
#define ERR_NOTONCHANNEL(client, channel)  ":irc.1337.ma 442 " + (client) + " " + (channel) + " :You're not on that channel"
class Server {
  private:
        uint16_t	              _port;
        std::string             _password;
        int                     _listen_sd;
        struct sockaddr_in      _addr;
        std::vector<pollfd>    	_fds;
        size_t                  _nfds;
        std::map<int, Client>	_clients;
        std::map<std::string, Channel> server_channels; // list of channels in the server

  public:
      Server(void);
      Server(uint16_t port, char *password);

      unsigned int getPort() const { return _port; }
      std::string getPassword() const { return _password; }

      int createServer();
      void handlIncomeConnections();
      void handleIncomeData();

      void parseCommand(std::string &message, Client &cling);
      void command_list(std::string &message, Client &cling);
      void command_join(std::vector<std::string> &splitedMsg, Client &client);
      void processTheJoinArgs(std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client);
      int createChannel(std::string &chnName, std::vector<std::string> &keys, Client &client);

      // void setClientStatus(Client &cling);

        ~Server();
};

#endif