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

        void setNickname(const std::string& nick);
        void setUsername(const std::string& user);
        void setRealname(const std::string& real);

        std::string getNickname() const ;
        std::string getUsername() const ;
        std::string getRealname() const ;

        ~Client(void);
};

#endif