#ifndef IRC_HPP__
#define IRC_HPP__

// c headers:
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// c++ headers:
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iterator>
#include <string>
#include <ctime> 

// sys network headers:
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/signal.h>

#include <arpa/inet.h>
#include <netinet/in.h>


#include "define.hpp"
#include "../src/client/client.hpp"
#include "../src/channel/channel.hpp"
#include "../src/server/server.hpp"

template <typename T>
inline std::string to_string(T value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
};

uint16_t    arg_checker(int ac, char **av);
uint16_t    valid_port(char *port_arg);
bool        valid_passord(char *password);

void replaceChar(std::string &str, char oldChar, char newChar);
void Err(const std::string &msg);
std::string stringUpper(const std::string &_str);
std::string trimTheSpaces(const std::string& str);
std::vector<std::string> splitBySpace(const std::string &str);
std::vector<std::string> splitByDelim(const std::string &str, char delim);


void replyTo(int socket, const std::string &buffer);
std::string getTime();

#endif
