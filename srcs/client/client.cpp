#include "client.hpp"

#define NICKLEN 10 // maybe warn about these ?
#define USERLEN 9
#define REALLEN 32

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

void Client::setValidPass(bool status)
{
    _validPass = status;
};

void Client::setRegistered(bool status )
{
    _registered = status;
};

bool Client::setNickname(const std::string& nickName)
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


bool Client::setUsername(const std::string& userName) 
{
    if (userName.empty())
        return (false);
    _userName = "~" + userName.substr(0, USERLEN);
    return (true);
};

bool Client::setRealname(const std::string& realName) 
{
    if (realName.empty())
        return (false);
     _realName = realName.substr(0,REALLEN);
     return (true);
};

/// @brief
/// #### getters ####

bool Client::getRegistered(void) const
{
    return _registered;
};

int  Client::getSocket(void) const
{
    return _socket;
};

bool Client::getValidPass(void) const
{
    return _validPass;
};


std::string Client::getNickname(void) const
{
    return _nickName;
};

std::string Client::getUsername(void) const
{
    return _userName;
};

std::string Client::getRealname(void) const
{
    return _realName;
};

void Client::refStatus(void)
{
    if (_registered == false && _validPass == true && !_nickName.empty() && !_userName.empty() && !_realName.empty())
    {
        _registered = true;

        // send(_socket,HEADER"\r\n",sizeof(HEADER),0);
    }
};

Client::~Client(void)
{};
