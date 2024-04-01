#ifndef FT_IRC_HPP__
#define FT_IRC_HPP__

#include "colors.hpp"
#include "define.hpp"


// class client {

// };


class Server {
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

class health {
    private:
        static uint16_t valid_port(char *port_arg);
        static bool valid_passord(char *password);
    public :
        static uint16_t arg_checker(int ac, char **av);
};


#endif // SERVER_HPP