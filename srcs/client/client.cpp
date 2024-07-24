#include "client.hpp"
#include <arpa/inet.h> // print ip adrss
#include <string>
#include <stdlib.h>
#include "../channel/channel.hpp"

#define NICKLEN 10 // maybe warn about these ?
#define USERLEN 9
#define REALLEN 32


/// TOOLS
inline void replyTo(int socket, const std::string &buffer)
{
    send(socket, buffer.c_str() , buffer.size() , 0);
};

std::string getTime() 
{
    time_t clock = time(nullptr);
    std::string timeFormat = ctime(&clock);
    return (timeFormat.substr(0, timeFormat.find('\n')));
}
///TOOLS

/// @brief
/// #### constructors ####

Client::Client(void): _socket(-1) , _registered(false) , _validPass(false) , 
    _nickName(""), _userName(""), _realName("")
{
    memset(&_addr, 0, sizeof(_addr));
};

Client::Client(int socket, struct sockaddr_in &addr) : _socket(socket) , _registered(false) , _validPass(false) , 
    _nickName(""), _userName(""), _realName("")
{
    replyTo(_socket, NOTE0);
    replyTo(_socket, NOTE1);
    replyTo(_socket, NOTE2);
    replyTo(_socket, NOTE3);
    replyTo(_socket, NOTE4);
    replyTo(_socket, NOTE5);
    replyTo(_socket, NOTE6);
    replyTo(_socket, NOTE7);
    memset(&_addr, 0, sizeof(_addr));
    memmove(&_addr, &addr, sizeof(_addr));
};

Client::Client(const Client& user) : _socket(user._socket) , _registered(user._registered) , _validPass(user._validPass) , 
    _nickName(user._nickName), _userName(user._userName), _realName(user._realName)
{
    memset(&_addr, 0, sizeof(_addr));
    memmove(&_addr, &user._addr, sizeof(_addr));
};

/// @brief
/// #### setters ####

void
Client::setSocket(int socket)
{
    _socket = socket;
};

void 
Client::setValidPass(bool status)
{
    _validPass = status;
};

void 
Client::setRegistered(bool status )
{
    _registered = status;
};

bool 
Client::setNickname(const std::string& nickName)
{
    if (nickName.empty() || nickName[0] == '$' || nickName[0] == ':' || nickName[0] == '#' || nickName[0] == '&' ||  nickName[0] == '+'
            ||  nickName[0] == '~' ||  nickName[0] == '%' || ::isdigit(nickName[0]))
        return (false);
    for (int i = 0; nickName[i] ;i++)
    {
        if (nickName[i] == ' ' || nickName[i] == ',' || nickName[i] == '*' || nickName[i] == '?' || nickName[i] == '!' || nickName[i] == '@' || nickName[i] == '.')
            return (false);
    }
    _nickName = nickName.substr(0,NICKLEN);
    return (true);
};

bool 
Client::setUsername(const std::string& userName) 
{
    if (userName.empty())
        return (false);
    _userName = "~" + userName.substr(0, USERLEN);
    return (true);
};

bool 
Client::setRealname(const std::string& realName) 
{
    if (realName.empty())
        return (false);
     _realName = realName.substr(0,REALLEN);
     return (true);
};

// set channel to the client 
// void
// Client::setChannel(std::string &chnName, Channel &channel)
// {
//     joined_channels.insert(std::pair<std::string, Channel>(chnName, channel));
// };

/// @brief
/// #### getters ####

bool 
Client::getRegistered(void) const
{
    return _registered;
};

int  
Client::getSocket(void) const
{
    return _socket;
};

bool 
Client::getValidPass(void) const
{
    return _validPass;
};


std::string 
Client::getNickname(void) const
{
    return _nickName;
};

std::string 
Client::getUsername(void) const
{
    return _userName;
};

std::string 
Client::getRealname(void) const
{
    return _realName;
};

sockaddr_in Client::getAddr(void) const
{
    return _addr;
};


void 
Client::refStatus(void)
{
    if (_registered == false && _validPass == true && !_nickName.empty() && !_userName.empty() && !_realName.empty())
    {
        _registered = true;
        std::cout << "Client " << _nickName << " is now registered" << std::endl;
        std::string host = inet_ntoa(_addr.sin_addr);
        replyTo(_socket, RPL_WELCOME(_nickName, _userName, host));
        replyTo(_socket, RPL_YOURHOST(_nickName, host, std::to_string(ntohs(_addr.sin_port))));
        replyTo(_socket, RPL_CREATED(_nickName, getTime()));
        replyTo(_socket, RPL_MYINFO(_nickName));
        replyTo(_socket, RPL_ISUPPORT(_nickName));
    }
};

Client::~Client(void)
{};
