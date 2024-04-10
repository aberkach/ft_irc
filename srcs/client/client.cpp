#include "client.hpp"

Client::Client(int sock, struct sockaddr_in &client_adrs) : socket(sock) , _registered(false) , _validPass(false) , 
        nickname(""), username(""), realname("")
{
    memset(&_addr, 0, sizeof(_addr));
    memmove(&_addr, &client_adrs, sizeof(_addr));
};

// parse for the following ...
// 	should be uniq to only one client 
// 	shouldnt start with the following ===> $ : # & + ~  %
// 	shouldnt contain eny of these ===> space , * ? ! @ .
#define NICKLEN 10
#define USERLEN 9
#define REALLEN 32

bool Client::setNickname(const std::string& nick) // use regualr expression in the future
{
    if (nick.empty() || nick[0] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '&' ||  nick[0] == '+'
            ||  nick[0] == '~' ||  nick[0] == '%' || ::isdigit(nick[0]))
        return (false);
    for (int i = 0; nick[i] ;i++)
    {
        if (nick[i] == ' ' || nick[i] == ',' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@' || nick[i] == '.')
            return (false);
    }
    nickname = nick.substr(0,NICKLEN);
    return (true);
};


bool Client::setUsername(const std::string& user) 
{
    username = "~" + user.substr(0, USERLEN);
    return (true);
};

bool Client::setRealname(const std::string& real) 
{
    if (real.empty())
        return (false);
     realname = real.substr(0,REALLEN);
     return (true);
};

/// getters :

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
