#pragma once

#include <netinet/in.h>
#include <string>

class Channel;

class Client
{
    public:
        std::string     _clientBuffer;

    private:
        int             _socket;
        bool            _registered;
        bool            _validPass;
        std::string     _nickName;
        std::string     _userName;
        std::string     _realName;
        sockaddr_in     _addr;

    public:
        Client(void);
        Client(int socket, struct sockaddr_in &addr);
        Client(const Client& user);
 
        void          setSocket(int socket);
        void          setValidPass(bool status);
        void          setRegistered(bool status);
        bool          setNickname(const std::string& nickName);
        bool          setUsername(const std::string& userName);
        bool          setRealname(const std::string& realName);

        int           getSocket(void) const;
        bool          getRegistered(void) const;
        bool          getValidPass(void) const ;
        std::string   getNickname(void) const ;
        std::string   getUsername(void) const ;
        std::string   getRealname(void) const ;
        sockaddr_in   getAddr(void) const;

        void          refStatus(uint countCli);

        ~Client(void);
};
