#ifndef DEFINE_HPP__
#define DEFINE_HPP__

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>

#include <poll.h>

#define HEADER " ██▓ ██▀███  ▄████▄       ██████ ▓█████  ██▀███   ██▒   █▓▓█████  ██▀███  \n▓██▒▓██ ▒ ██▒██▀ ▀█     ▒██    ▒ ▓█   ▀ ▓██ ▒ ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒\n▒██▒▓██ ░▄█ ▒▓█    ▄    ░ ▓██▄   ▒███   ▓██ ░▄█ ▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒\n░██░▒██▀▀█▄ ▒▓▓▄ ▄██▒     ▒   ██▒▒▓█  ▄ ▒██▀▀█▄    ▒██ █░░▒▓█  ▄ ▒██▀▀█▄  \n░██░░██▓ ▒██▒ ▓███▀ ░   ▒██████▒▒░▒████▒░██▓ ▒██▒   ▒▀█░  ░▒████▒░██▓ ▒██▒\n░▓  ░ ▒▓ ░▒▓░ ░▒ ▒  ░   ▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒▓ ░▒▓░   ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░\n ▒ ░  ░▒ ░ ▒░ ░  ▒      ░ ░▒  ░ ░ ░ ░  ░  ░▒ ░ ▒░   ░ ░░   ░ ░  ░  ░▒ ░ ▒░\n ▒ ░  ░░   ░░           ░  ░  ░     ░     ░░   ░      ░░     ░     ░░   ░ \n ░     ░    ░ ░               ░     ░  ░   ░           ░     ░  ░   ░     "

#define LOG(x) std::cerr << "debug :" << x << std::endl;


#endif