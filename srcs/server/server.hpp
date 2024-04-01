#ifndef SERVER_HPP__
#define SERVER_HPP__

#include "../../Inc/define.hpp"

class Server
{
  private:
    unsigned int            _port;
    std::string             _password;
    int                     _listen_sd;
    struct sockaddr_in      _addr;
	struct pollfd         	_fds[5000];
    int                     _nfds;
    // std::map<int, Clients>  _clients;

  public:
        Server();
        ~Server();
        
        void setPort(unsigned int port);
        void setPassword(char *password);
        
        unsigned int getPort() const { return _port; }
        std::string getPassword() const { return _password; }

        int createServer();
        // void updateFileDescrior(int *ng);
};

#endif