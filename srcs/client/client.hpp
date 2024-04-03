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
        bool _registered;
        bool _validPass;

    public:
        Client(int sock);

        void setNickname(const std::string& nick);
        void setUsername(const std::string& user);
        void setRealname(const std::string& real);
        void setRegistered(bool reg) { _registered = reg; }
        void setValidPass(bool pass) { _validPass = pass; }

        std::string getNickname() const ;
        std::string getUsername() const ;
        std::string getRealname() const ;
        bool getRegistered() const { return _registered; }
        bool getValidPass() const { return _validPass; }

        ~Client(void);
};

#endif