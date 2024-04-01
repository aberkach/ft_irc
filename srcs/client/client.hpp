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
        Client(int sock, std::string nick, std::string user, std::string real) 
            : socket(sock), nickname(nick), username(user), realname(real) {}
};