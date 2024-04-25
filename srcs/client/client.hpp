#pragma once

#include "../../Inc/define.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
class Channel;

class Client
{
    public:
        sockaddr_in _addr;
        std::string _clientBuffer;

    private:
        int _socket;
        bool _registered;
        bool _validPass;
        std::string _nickName;
        std::string _userName;
        std::string _realName;

    public:
        Client(void);
        Client(int socket, struct sockaddr_in &addr);
        Client(const Client& user);

        void setSocket(int socket);
        void setValidPass(bool status);
        void setRegistered(bool status);
        bool setNickname(const std::string& nickName);
        bool setUsername(const std::string& userName);
        bool setRealname(const std::string& realName);
        void setChannel(std::string &chnName, Channel &channel);

        // void setAddress(sockaddr_in userAddr) { _addr = userAddr; }
        // void getAddress(sockaddr_in userAddr) { _addr = userAddr; }

        std::string getNickname(void) const ;
        std::string getUsername(void) const ;
        std::string getRealname(void) const ;
        bool getRegistered(void) const;
        bool getValidPass(void) const ;
        int  getSocket(void) const;
        sockaddr_in getAddr(void) const;
        
        // std::map<std::string, Channel>::iterator getChannel(std::string &chnName);
        // std::map<std::string, Channel> getChannels(void);

        // void removeChannel(std::string &chnName);

        void refStatus(void);

        ~Client(void);
};

