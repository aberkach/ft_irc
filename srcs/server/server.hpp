#ifndef SERVER_HPP__
#define SERVER_HPP__

#include "../../Inc/define.hpp"
#include "../client/client.hpp"
#include <sys/poll.h>
#include <vector>

class Server
{
  private:
    unsigned int            _port;
    std::string             _password;
    int                     _listen_sd;
    struct sockaddr_in      _addr;
    std::vector<pollfd>      _fds;
    int                     _nfds;
    std::map<int, Client>  _clients;

  public:
        Server();
        Server(unsigned int port);
        
        void setPort(unsigned int port);
        void setPassword(char *password);

        unsigned int getPort() const { return _port; }
        std::string getPassword() const { return _password; }

        int createServer();
        void handlIncomeConnections();
        void handleIncomeData();
        // void updateFileDescrior(int *ng);

        ~Server();
};

#endif