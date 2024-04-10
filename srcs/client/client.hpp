#ifndef CLIENT_HPP__
#define CLIENT_HPP__

#include "../../Inc/define.hpp"
#include "../channel/channel.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

class Client
{
    private:
        int socket;
        bool _registered;
        bool _validPass;
        std::string nickname;
        std::string username;
        std::string realname;
        // std::map<std::string, Channel> joined_channels;

    public:
        sockaddr_in _addr;

        Client(int sock, struct sockaddr_in &client_adrs);

        bool setNickname(const std::string& nick);
        bool setUsername(const std::string& user);
        bool setRealname(const std::string& real);
        void setRegistered(bool reg) { _registered = reg; }
        void setValidPass(bool pass) { _validPass = pass; }

        void setSckAddr(sockaddr_in addr) { this->_addr = addr; }

        std::string getNickname() const ;
        std::string getUsername() const ;
        std::string getRealname() const ;
        bool getRegistered() { return _registered; }
        bool getValidPass() { return _validPass; }
        int getsocket() {return socket;}

        void refstatus()
        {
            if (_registered == false && _validPass == true && !nickname.empty() && !username.empty() && !realname.empty())
            {
                _registered = true;

                send(socket,HEADER"\r\n",sizeof(HEADER),0);
                send(socket,HEADER"\r\n",sizeof(HEADER),0);
                send(socket,HEADER"\r\n",sizeof(HEADER),0);
                send(socket,HEADER"\r\n",sizeof(HEADER),0);
                send(socket,HEADER"\r\n",sizeof(HEADER),0);
            }

        }

        ~Client(void);
};

#endif