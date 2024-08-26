#ifndef IRCBOT_HPP
#define IRCBOT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netdb.h> // gethostbyname
#include <cstring> // memory funcs
#include <unistd.h>
#include <vector>
#include <arpa/inet.h> // For inet_pton and sockaddr_in
#include <map>
#include <sstream> // stingstream
#include <string>
#include <functional> // bind find
#include <random> //  srand
#include <stdexcept>

#include "defines.hpp"
#include "../src/bot.hpp"

// TOOLS__
uint16_t                    valid_port(char *port_arg) throw();
uint16_t                    arg_checker(int ac, char **av) throw();
std::string                 trimTheSpaces(const std::string& str) throw();
std::vector<std::string>    splitMessage(const std::string& message) throw();
int                         createTCPSock(void) throw();
bool                        connectSockByIp(int socket, const char *ip_addr, int port);
bool                        connectSockByName(int socket, const char *hostbyname, int port) throw();
void                        sendit(int sock, std::string& msg);
std::string                 recvit(int sock);
std::string                 jsonValue(const std::string& json, const std::string& key) throw();
std::vector<std::string>    splitByDelim(const std::string &str, char delim);

template <typename T>
inline std::string to_string(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
};

#endif
