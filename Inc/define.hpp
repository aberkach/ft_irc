#ifndef DEFINE_HPP__
#define DEFINE_HPP__

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// c headers:
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// c++ headers:
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <cctype>

// sys network headers:
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/fcntl.h>

#include <netinet/in.h>

#define NICKLEN 10
#define USERLEN 9
#define REALLEN 32

#define EVENT_READ 1 << 0
#define EVENT_WRITE 1 << 1
#define EVENT_RW (EVENT_WRITE | EVENT_READ)

#define HEADER " ██▓ ██▀███  ▄████▄       ██████ ▓█████  ██▀███   ██▒   █▓▓█████  ██▀███  \n▓██▒▓██ ▒ ██▒██▀ ▀█     ▒██    ▒ ▓█   ▀ ▓██ ▒ ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒\n▒██▒▓██ ░▄█ ▒▓█    ▄    ░ ▓██▄   ▒███   ▓██ ░▄█ ▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒\n░██░▒██▀▀█▄ ▒▓▓▄ ▄██▒     ▒   ██▒▒▓█  ▄ ▒██▀▀█▄    ▒██ █░░▒▓█  ▄ ▒██▀▀█▄  \n░██░░██▓ ▒██▒ ▓███▀ ░   ▒██████▒▒░▒████▒░██▓ ▒██▒   ▒▀█░  ░▒████▒░██▓ ▒██▒\n░▓  ░ ▒▓ ░▒▓░ ░▒ ▒  ░   ▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒▓ ░▒▓░   ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░\n ▒ ░  ░▒ ░ ▒░ ░  ▒      ░ ░▒  ░ ░ ░ ░  ░  ░▒ ░ ▒░   ░ ░░   ░ ░  ░  ░▒ ░ ▒░\n ▒ ░  ░░   ░░           ░  ░  ░     ░     ░░   ░      ░░     ░     ░░   ░ \n ░     ░    ░ ░               ░     ░  ░   ░           ░     ░  ░   ░     " + "\r\n"
// client connected to the server


#define RPL_WELCOME(client) ":irc.1337.chat 001 " + (client) + " :Welcome to the IRC Network, " + (client) + "\r\n"
#define RPL_YOURHOST(client, servername, version) ":irc.1337.chat 002 " + (client) + " :Your host is " + (servername) + ", running version " + (version) + "\r\n"
#define RPL_CREATED(client, time) ":irc.1337.chat 003 " + (client) + " :This server was created " + (time) + "\r\n"
#define RPL_MYINFO(client, servername, version) ":irc.1337.chat 004 " + (client) + " :" + (servername) + " " + (version) + "\r\n"
#define RPL_ISUPPORT(client)  ":irc.1337.chat 005 " + (client) + " :are supported by this server" + "\r\n"



#define ERR_ALREADYREGISTERED(client) ":irc.1337.chat 462 " +  (client) + " :You may not reregister" + "\r\n"
#define ERR_ERRONEUSNICKNAME(client, command) ":irc.1337.chat 432 " +  (client) + " " + (command) + " :Erroneus nickname" + "\r\n"
#define ERR_UNKNOWNCOMMAND(client, command) ":irc.1337.chat 421 " +  (client) + " " + (command) + " :Unknown command" + "\r\n"
#define ERR_NOSUCHNICK(client1, client2) ":irc.1337.chat 401 " + (client1) + " " + (client2) + " :No such nick" + "\r\n"
#define ERR_NEEDMOREPARAMS(client, command) ":irc.1337.chat 461 " + (client) + " " + (command) + " :Not enough parameters" + "\r\n"
#define ERR_NOSUCHCHANNEL(client, channel)  ":irc.1337.chat 403 " + (client) + " " + (channel) + " :No such channel" + "\r\n"
#define ERR_BADCHANNELKEY(client, channel)   ":irc.1337.chat 475 " + (client) + " " + (channel) + " :Cannot join channel (+k)" + "\r\n"
#define ERR_NOTONCHANNEL(client, channel)  ":irc.1337.chat 442 " + (client) + " " + (channel) + " :You're not on that channel" + "\r\n"
#define ERR_USERNOTINCHANNEL(client, nickname, channel) ":irc.1337.chat 441 " + (client) + " " + (nickname) + " " + (channel) + " :They aren't on that channel" + "\r\n"
#define ERR_CHANOPRIVSNEEDED(client, channel) ":irc.1337.chat 482 " + (client) + " " + (channel) + " :You're not channel operator" + "\r\n"

#define ERR_PASSWDMISMATCH(client) ":irc.1337.chat 464 " + (client) + " :Password incorrect" + "\r\n"

#define ERR_USERONCHANNEL(client, nick, channel) ":irc.1337.chat 443 " + (client) + " " + (nick) + " " + (channel) + " :is already on channel" + "\r\n"
#define ERR_INVITEONLYCHAN(client, channel) ":irc.1337.chat 473 " + (client) + " " + (channel) + " :Cannot join channel (+i)" + "\r\n"
#define ERR_BADCHANMASK(channel)  ":irc.1337.chat 476 " + (channel) + " :Bad Channel Mask" + "\r\n"
#define ERR_UNKNOWNMODE(client, modechar) ":irc.1337.chat 472 " + (client) + " " + (modechar) + " :is unknown mode char to me" + "\r\n"
#define ERR_CHANNELISFULL(client, channel)  ":irc.1337.chat 471 " + (client) + " " + (channel) + " :Cannot join channel (+l)" + "\r\n"
#define ERR_KEYALREADYSET(client, channel) ":irc.1337.chat 467 " + (client) + " " + (channel) + " :Channel key already set" + "\r\n"
#define ERR_NORECIPENT(client, command) ":irc.1337.chat 411 " + (client) + " :No recipient given " + (command) + "" + "\r\n"
#define ERR_ONEOUS(client, wrongnick) MY_HOSTNAME +" 432 " + (client) + " " + (wrongnick) + " :Erroneous Nickname" + "\r\n"
#define ERR_NICKNAMEINUSE(client) ":irc.1337.chat 433 " + (client) +  " :Nickname is already in use" + "\r\n"
#define ERR_NONICKNAMEGIVEN(client) ":irc.1337.chat 431 " + (client) + " :No nickname given" + "\r\n"
#define ERR_NOTEXTTOSEND(client) ":irc.1337.chat 412 " + (client) + " :No text to send" + "\r\n"
#define ERROR_MSG(reason) ":irc.1337.chat " + (reason) + "\r\n"
#define ERR_NOTREGISTERED(client) ":irc.1337.chat 451 " + (client) + " :You have not registered" + "\r\n"
#define RPL_ENDOFWHO(client) ":irc.1337.chat 315 " + (client) + " :End of /WHO list" + "\r\n"

#define RPL_NICK(client, username,ipaddr, newnick)  ":" + (client) + "!~" + (username) + "@"+ (ipaddr) + " NICK :"+(newnick) + "\r\n"
#define KICK_MSG(client, ipaddr, channel, kicked, reason) ":" + (client) + "!~" + (ipaddr) + " KICK " + (channel) + " " + kicked + " " + reason + "\r\n"
#define QUIT_MSG(nickname, username, ipaddr, reason) ":"+ (nickname) + "!~" + (username) + "@" + (ipaddr) + " QUIT :Client Quit" + (reason) + "\r\n"
#define PRIVMSG(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " PRIVMSG " + (target) + " :" + (message) + "\r\n"
#define JOIN_SUCC(nickname, username, client_ip, channel) ":" + (nickname) + "!~" + (username) + "@" + (client_ip) + " JOIN " + (channel) + "\r\n"
#define MODE_MSG(client, username,ipaddr, channel,added_mode, target) ":" + (client) + "!~" + (username) +"@" + (ipaddr) + " MODE " + (channel) + " " +(added_mode) +  target + "\r\n"
#define MODE_MSG_2(client, username,ipaddr, channel,added_mode) ":" + (client) + "!~" + (username) +"@" + (ipaddr) + " MODE " + (channel) + " " +(added_mode) + "\r\n"
#define PART_MSG(nickname, username, ipaddr, channel, reason) ":" + (nickname) + "!~" + (username) + "@" + ipaddr + " PART " + (channel) + " " + (reason) + "\r\n"
#define NOTICE(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " NOTICE " + (target) + " :" + (message) + "\r\n"
#define TOPIC_MSG(client, username, ipaddr, channel, topic) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " TOPIC " + (channel) + " :" + (topic) + "\r\n"

#define RPL_NAMREPLY(client, channel, users_list) ":irc.1337.ma 353 " + (client) + " = " + (channel) + " :" + (users_list) + "\r\n"
#define RPL_CREATIONTIME(client, channel, creation_time) ":irc.1337.ma 329 " + (client) + " " + (channel) + " " + (creation_time) + "\r\n"
#define RPL_ENDOFNAMES(client, channel)  ":irc.1337.ma 366 " + (client) + " " + (channel) + " :End of /NAMES list" + "\r\n"
#define RPL_ENDOFINVITELIST(client) ":irc.1337.ma 337 " + " :End of /INVITE list" + "\r\n"

#define RPL_CHANNELMODEIS(client, channel, modestring, modargument) ":irc.1337.ma 324 " + (client) + " "+ (channel) + " " +(modestring) + " " + (modargument) + "\r\n"
#define RPL_TOPIC(client, channel, topic) ":irc.1337.ma 332 " + (client) + " " + (channel) +  " :" + (topic) + "\r\n"
#define RPL_NOTOPIC(client, channel)  ":irc.1337.ma 331 " + (client) + " " + (channel) + " :No topic is set" + "\r\n"
#define RPL_INVITING(client, nick, channel)  ":irc.1337.ma 341 " + (client) + " " + (nick) + " " + (channel) + "\r\n"
#define RPL_TOPICWHOTIME(client, channel, nick, setat) ":irc.1337.ma 333 " + (client) + " " + (channel) + " " + (nick) + " " + (setat) + "\r\n"
#define RPL_MOTD(client, sentence) ":irc.1337.ma 372 " + (client) + " " + (sentence) + "\r\n"
#define ERR_UMODEUNKNOWNFLAG(client) ":irc.1337.ma 501 " + (client) + " :Unknown MODE flag" + "\r\n"
#define ERR_INVALIDMODEPARAM(client, target, modechar, parameter) ":irc.1337.ma 696 " + (client) + " " + (target) + " " + (modechar) + " " + " You must specify a parameter for the key mode. Syntax: " + (parameter) + "\r\n"
#define RPL_LIST(client, channel, nb_users, topic) ":irc.1337.ma 322 " + (client) + " " + (channel) + " " + (nb_users) + " :" + (topic) + "\r\n"
#define RPL_LISTSTART(client) ":irc.1337.ma 321 " + (client) + " Channel :Users Name" + "\r\n"
#define RPL_LISTEND(client) ":irc.1337.ma 323 " + (client) + " :End of /LIST" + "\r\n"
#define ERR_NORECIPIENT(client, command) ":irc.1337.ma 411 " + (client) + " :No recipient given (" + (command) + ")" + "\r\n"
#define ERR_NOORIGIN(client) ":irc.1337.ma 409 " + (client) + " :No origin specified" + "\r\n"
#define ERR_NOSUCHSERVER(client, servername) ":irc.1337.ma 402 " + (client) + " " + (servername) + " :No such server" + "\r\n"
#define PONG(client, server) "PONG " + (client) + " :" + (server) + "\r\n"
#define PING(server) "PING " + (server) + "\r\n"

#define RPL_WHOREPLY(client, channel, username, hostname, servername, nickname, mode, realname) ":irc.1337.ma 352 " + (client) + " " + (channel) + " " + (username) + " " + (hostname) + " " + (servername) + " " + (nickname) + " " + (mode) + " " + (realname) + "\r\n"

// debuging macro
// #ifndef DEBUG__
// #define DEBUG__ 0


#endif