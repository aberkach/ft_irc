#include "client.hpp"

Client::Client(int sock) : socket(sock) , _registered(false) , _validPass(false) , 
        nickname(""), username(""), realname("")
{
    memset(&_addr, 0, sizeof(_addr));
};

void Client::setNickname(const std::string& nick) 
{
     nickname = nick; 
};

void Client::setUsername(const std::string& user) 
{
     username = user; 
};

void Client::setRealname(const std::string& real) 
{
     realname = real; 
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
