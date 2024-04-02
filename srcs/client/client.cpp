#include "client.hpp"


Client::Client(int sock) : socket(sock) {
    nickname = "default";
    username = "default";
    realname = "default";
}

Client::Client(std::string nick, std::string user, std::string real) {
    nickname = nick;
    username = user;
    realname = real;
}