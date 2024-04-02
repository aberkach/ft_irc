#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../../Inc/define.hpp"

class Client
{
    private:
        int socket;
        std::string nickname;
        std::string username;
        std::string realname;
        std::vector<std::string> channels;

    public:
        Client(int sock);
        Client(std::string nick, std::string user, std::string real);
};

#endif