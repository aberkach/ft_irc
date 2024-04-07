#include "client.hpp"

Client::Client(int sock) : socket(sock) , _registered(false) , _validPass(false) , 
        nickname(""), username(""), realname("")
{
    memset(&_addr, 0, sizeof(_addr));
};

bool Client::setNickname(const std::string& nick) 
{
    // 	shouldnt start with the following ===> $ : # & + ~  %
    if (nick.empty() ||  nick[0] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '&' ||  nick[0] == '+'
            ||  nick[0] == '~' ||  nick[0] == '%' )
        return (false);
    // if (nick)
    
    nickname = nick;
    return (true);
};

bool Client::setUsername(const std::string& user) 
{
    username = user;

    return (false);
};

bool Client::setRealname(const std::string& real) 
{
     realname = real; 
     return (false);
};

std::string Client::getNickname() const
{
    return nickname;
};

std::string Client::getUsername() const
{
    return username;
};

std::string Client::getRealname() const
{
    return realname;
};

Client::~Client(){};
