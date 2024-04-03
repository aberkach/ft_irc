#ifndef SERVER_HPP__
#define SERVER_HPP__

#include "../../Inc/define.hpp"
#include "../client/client.hpp"
#include <cstddef>
#include <sys/poll.h>
#include <vector>

class Server
{
  private:
    uint16_t	            _port;
    std::string             _password;
    int                     _listen_sd;
    struct sockaddr_in      _addr;
    std::vector<pollfd>    	_fds;
    size_t                  _nfds;
    std::map<int, Client>	_clients;

  public:
        Server();
        Server(uint16_t port, char *password);
        
        // void setPort(unsigned int port);
        // void setPassword(char *password);

        unsigned int getPort() const { return _port; }
        std::string getPassword() const { return _password; }

        int createServer();
        void handlIncomeConnections();
        void handleIncomeData();
        void authentication(std::string message, std::map<int, Client>::iterator client);
        // void updateFileDescrior(int *ng);

        ~Server();
};

#endif