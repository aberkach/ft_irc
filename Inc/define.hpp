#ifndef DEFINE_HPP__
#define DEFINE_HPP__

// c headers:
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// c++ headers:
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

// sys network headers:
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>

#include <netinet/in.h>


#define EVENT_READ 1 << 0
#define EVENT_WRITE 1 << 1
#define EVENT_RW (EVENT_WRITE | EVENT_READ)

// debuging macro
#define LOG(x) std::cerr << "-> DEBUG :" << x << std::endl;

#define HEADER " ██▓ ██▀███  ▄████▄       ██████ ▓█████  ██▀███   ██▒   █▓▓█████  ██▀███  \n▓██▒▓██ ▒ ██▒██▀ ▀█     ▒██    ▒ ▓█   ▀ ▓██ ▒ ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒\n▒██▒▓██ ░▄█ ▒▓█    ▄    ░ ▓██▄   ▒███   ▓██ ░▄█ ▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒\n░██░▒██▀▀█▄ ▒▓▓▄ ▄██▒     ▒   ██▒▒▓█  ▄ ▒██▀▀█▄    ▒██ █░░▒▓█  ▄ ▒██▀▀█▄  \n░██░░██▓ ▒██▒ ▓███▀ ░   ▒██████▒▒░▒████▒░██▓ ▒██▒   ▒▀█░  ░▒████▒░██▓ ▒██▒\n░▓  ░ ▒▓ ░▒▓░ ░▒ ▒  ░   ▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒▓ ░▒▓░   ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░\n ▒ ░  ░▒ ░ ▒░ ░  ▒      ░ ░▒  ░ ░ ░ ░  ░  ░▒ ░ ▒░   ░ ░░   ░ ░  ░  ░▒ ░ ▒░\n ▒ ░  ░░   ░░           ░  ░  ░     ░     ░░   ░      ░░     ░     ░░   ░ \n ░     ░    ░ ░               ░     ░  ░   ░           ░     ░  ░   ░     "

#endif