#ifndef FT_IRC_HPP__
#define FT_IRC_HPP__

#include "colors.hpp"
#include "define.hpp"


// class client {

// };

class Server {
    private:
        uint16_t _port;
        int _server_sock;
        std::string _passkey;

    public:
        Server(uint16_t port, const std::string &passkey) : _port(port), _passkey(passkey){};
        


        ~Server(void){};
};


class health {
    private:
        static uint16_t valid_port(char *port_arg);
        static bool valid_passord(char *password);
    public :
        static uint16_t arg_checker(int ac, char **av);
};

#endif