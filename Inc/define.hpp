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

#define CNICKLEN "10"
#define CUSERLEN "9"
#define CREALLEN "32"

#define EVENT_READ 1 << 0
#define EVENT_WRITE 1 << 1
#define EVENT_RW (EVENT_WRITE | EVENT_READ)

#define HEADER " ██▓ ██▀███  ▄████▄       ██████ ▓█████  ██▀███   ██▒   █▓▓█████  ██▀███  \n▓██▒▓██ ▒ ██▒██▀ ▀█     ▒██    ▒ ▓█   ▀ ▓██ ▒ ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒\n▒██▒▓██ ░▄█ ▒▓█    ▄    ░ ▓██▄   ▒███   ▓██ ░▄█ ▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒\n░██░▒██▀▀█▄ ▒▓▓▄ ▄██▒     ▒   ██▒▒▓█  ▄ ▒██▀▀█▄    ▒██ █░░▒▓█  ▄ ▒██▀▀█▄  \n░██░░██▓ ▒██▒ ▓███▀ ░   ▒██████▒▒░▒████▒░██▓ ▒██▒   ▒▀█░  ░▒████▒░██▓ ▒██▒\n░▓  ░ ▒▓ ░▒▓░ ░▒ ▒  ░   ▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒▓ ░▒▓░   ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░\n ▒ ░  ░▒ ░ ▒░ ░  ▒      ░ ░▒  ░ ░ ░ ░  ░  ░▒ ░ ▒░   ░ ░░   ░ ░  ░  ░▒ ░ ▒░\n ▒ ░  ░░   ░░           ░  ░  ░     ░     ░░   ░      ░░     ░     ░░   ░ \n ░     ░    ░ ░               ░     ░  ░   ░           ░     ░  ░   ░     " + "\r\n"
// client connected to the server

// someone having the nickname 'Dan' and someone else having the nickname 'dan', confusing other users. Servers MUST advertise the casemapping they use in the RPL_ISUPPORT numeric that’s sent when connection registration has completed.

#define SERVERNAME             "Irc_42.chat"
#define VERSION                "1.2.3"

#define USERMODES             "abcde"
#define CHANELMODES           "fgdrc"
#define CHANELMODESPARAMETER  "12345"

#define COMMANDS "CHANTYPES=#"

#define NOTE0  ":"  SERVERNAME  " NOTICE Guest :** Looking up your hostname...\r\n"
#define NOTE1  ":"  SERVERNAME  " NOTICE Guest :** Couldn't look up your hostname\r\n"
#define NOTE2  ":"  SERVERNAME  " NOTICE Guest :** You are connected to the server as 'Guest', You have to authenticate to use this server\r\n"
#define NOTE3  ":"  SERVERNAME  " NOTICE Guest :** Available commands for the authentication process\r\n"
#define NOTE4  "                            PASS <password>\r\n"
#define NOTE5  "                            NICK <nickname>\r\n"
#define NOTE6  "                            USER <username> 0 * :<realname>\r\n"
#define NOTE7  std::string(":") + SERVERNAME  " NOTICE Guest :** NICKLEN=" + CNICKLEN + " USERLEN=" + CUSERLEN + " REALLEN=" + CREALLEN + " characters\n"\
               "                            if u pass the token, the value will be truncated to fit the server needs\r\n"

#define RPL_WELCOME(nick, user, host)        ":"  SERVERNAME  " 001 " + (nick) + " :Welcome to the internet relay chat Network, " + (nick) + "[!" + (user) + "@" + (host) + "]\r\n"
#define RPL_YOURHOST(nick, host, port)       ":"  SERVERNAME  " 002 " + (nick) + " :Your host is " SERVERNAME "[" + (host) + "/" + (port) + "], running version " VERSION  "\r\n"
#define RPL_CREATED(nick, datetime)          ":"  SERVERNAME  " 003 " + (nick) + " :This server was created " + (datetime) + "\r\n"
#define RPL_MYINFO(nick)                     ":"  SERVERNAME  " 004 " + (nick) + " " + SERVERNAME + " " + VERSION + " " + USERMODES + " " + CHANELMODES + " [" + CHANELMODESPARAMETER + "]\r\n"
#define RPL_ISUPPORT(nick)                   ":"  SERVERNAME  " 005 " + (nick) + " " COMMANDS " :are supported by this server\r\n"

#define ERR_UNKNOWNCOMMAND(nick, command)    ":"  SERVERNAME  " 421 " + (nick) + " " + (command) + " :Unknown command\r\n"
#define ERR_NONICKNAMEGIVEN(nick)            ":"  SERVERNAME  " 431 " + (nick) + " :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick)           ":"  SERVERNAME  " 432 " + (nick) + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick)              ":"  SERVERNAME  " 433 " + (nick) + " :Nickname is already in use\r\n"

#define ERR_USERFORMAT                       ":"  SERVERNAME  " 460 Guest :Use format USER <username> 0 * :<realname>\r\n"
#define ERR_NEEDMOREPARAMS(nick, command)    ":"  SERVERNAME  " 461 " + (nick) + " " + (command) + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(nick)          ":"  SERVERNAME  " 462 " + (nick) + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH                   ":"  SERVERNAME  " 464 Guest :Password incorrect\r\n"
#define ERR_FIRSTCOMMAND                     ":"  SERVERNAME  " 465 Guest :Server is expecting 'PASS <password>' first\r\n"

#define ERR_NOTREGISTERED(client)                                     ":" SERVERNAME " 451 " + (client) + " :You have not registered" + "\r\n"

#define ERR_NOSUCHCHANNEL(client, channel)              ":" SERVERNAME " 403 " + (client) + " " + (channel) + " :No such channel" + "\r\n"
#define ERR_USERNOTINCHANNEL(client, nickname, channel) ":" SERVERNAME " 441 " + (client) + " " + (nickname) + " " + (channel) + " :They aren't on that channel" + "\r\n"
#define ERR_NOTONCHANNEL(client, channel)               ":" SERVERNAME " 442 " + (client) + " " + (channel) + " :You're not on that channel" + "\r\n"

#define ERR_ALREADYINCHANNEL(client, nick, channel)        ":" SERVERNAME " 443 " + (client) + " " + (nick) + " " + (channel) + " :is already on channel" + "\r\n"
#define ERR_BADCHANNELKEY(client, channel)              ":" SERVERNAME " 475 " + (client) + " "  + " :Cannot join " + (channel) + " (+k)" + "\r\n"
#define ERR_BADCHANMASK(channel)                        ":" SERVERNAME " 476 " + (channel) + " :Bad Channel Mask" + "\r\n"
#define ERR_CHANOPRIVSNEEDED(client, channel)           ":" SERVERNAME " 482 " + (client) + " " + (channel) + " :You're not channel operator" + "\r\n"
#define ERR_NOSUCHNICK(client, nickname)                ":" SERVERNAME " 401 " + (client) + " " + (nickname) + " :No such nick" + "\r\n"
// #define ERR_INVALIDKEY (525) : if the operator is trying to set a key that is not valid with mode +k 

// need more things to add when the user is joined the channel successfully
#define JOIN_SUCC(client, channel)             ":" SERVERNAME " !~ " + (client) + " join successfully " + (channel) + "\r\n"
#define RPL_NOTOPIC(client, channel)           ":" SERVERNAME " 331 " + (client) + " " + (channel) + " :No topic is set" + "\r\n"
#define RPL_TOPIC(client, channel, topic)       ":" SERVERNAME " 332 " + (client) + " " + (channel) + " :" + (topic) + "\r\n"

#define KICK_MSG(client, channel, reason)             ":" SERVERNAME " !~ " + (client) + " KICKED FROM " + (channel) + " :" + reason + "\r\n"

#define QUIT_MSG(nickname, username, ipaddr, reason)                  ":"+ (nickname) + " !~ " + (username) + "@" + (ipaddr) + " QUIT :Client Quit " + SERVERNAME + (reason) + "\r\n"


#define RPL_NAMREPLY(client, channel, users_list)                     ":" SERVERNAME " 353 " + (client) + " = " + (channel) + " :" + (users_list) + "\r\n"
#define RPL_ENDOFNAMES(client, channel)                               ":" SERVERNAME " 366 " + (client) + " " + (channel) + " :End of /NAMES list" + "\r\n"
#define INVITE_MSG(client, ipaddr, channel, invited)                  ":" SERVERNAME " " + (client) + " !~ " + (ipaddr) + " INVITE " + (channel) + " " + invited + "\r\n"

// #define ERR_INVITEONLYCHAN(client, channel)             ":" SERVERNAME " 473 " + (client) + " " + (channel) + " :Cannot join channel (+i)" + "\r\n"
// #define RPL_INVITING(client, nick, channel)                           ":" +  SERVERNAME " 341 " + (client) + " invited " + (nick) + " to " + (channel) + " successfully\r\n"



// #define ERR_NOSUCHNICK(client1, client2)                ":" SERVERNAME " 401 " + (client1) + " " + (client2) + " :No such nick" + "\r\n"
// #define ERR_INVITEONLYCHAN(client, channel)             ":" SERVERNAME " 473 " + (client) + " " + (channel) + " :Cannot join channel (+i)" + "\r\n"
// #define ERR_UNKNOWNMODE(client, modechar)               ":" SERVERNAME " 472 " + (client) + " " + (modechar) + " :is unknown mode char to me" + "\r\n"
// #define ERR_CHANNELISFULL(client, channel)              ":" SERVERNAME " 471 " + (client) + " " + (channel) + " :Cannot join channel (+l)" + "\r\n"
// #define ERR_KEYALREADYSET(client, channel)              ":" SERVERNAME " 467 " + (client) + " " + (channel) + " :Channel key already set" + "\r\n"
// #define ERR_NORECIPENT(client, command)                 ":" SERVERNAME " 411 " + (client) + " :No recipient given " + (command) + "" + "\r\n"
// #define ERR_ONEOUS(client, wrongnick)                   ":" SERVERNAME " 432 " + (client) + " " + (wrongnick) + " :Erroneous Nickname" + "\r\n"
// #define ERR_NOTEXTTOSEND(client)                        ":" SERVERNAME " 412 " + (client) + " :No text to send" + "\r\n"

// #define ERROR_MSG(reason)                                             ":" SERVERNAME " " + (reason) + "\r\n"
// #define RPL_ENDOFWHO(client)                                          ":" SERVERNAME " 315 " + (client) + " :End of /WHO list" + "\r\n"
// #define RPL_NICK(client, username,ipaddr, newnick)                    ":" + (client) + "!~" + (username) + "@"+ (ipaddr) + " NICK :"+(newnick) + "\r\n"
// #define PRIVMSG(client, username, ipaddr, target, message)            ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " PRIVMSG " + (target) + " :" + (message) + "\r\n"
// #define MODE_MSG(client, username,ipaddr, channel,added_mode, target) ":" + (client) + "!~" + (username) +"@" + (ipaddr) + " MODE " + (channel) + " " +(added_mode) +  target + "\r\n"
// #define MODE_MSG_2(client, username,ipaddr, channel,added_mode)       ":" + (client) + "!~" + (username) +"@" + (ipaddr) + " MODE " + (channel) + " " +(added_mode) + "\r\n"
// #define PART_MSG(nickname, username, ipaddr, channel, reason)         ":" + (nickname) + "!~" + (username) + "@" + ipaddr + " PART " + (channel) + " " + (reason) + "\r\n"
// #define NOTICE(client, username, ipaddr, target, message)             ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " NOTICE " + (target) + " :" + (message) + "\r\n"
// #define TOPIC_MSG(client, username, ipaddr, channel, topic)           ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " TOPIC " + (channel) + " :" + (topic) + "\r\n"
// #define RPL_CREATIONTIME(client, channel, creation_time)              ":" SERVERNAME " 329 " + (client) + " " + (channel) + " " + (creation_time) + "\r\n"
// #define RPL_ENDOFINVITELIST(client)                                   ":" SERVERNAME " 337 " + " :End of /INVITE list" + "\r\n"
// #define RPL_CHANNELMODEIS(client, channel, modestring, modargument)   ":" SERVERNAME " 324 " + (client) + " "+ (channel) + " " +(modestring) + " " + (modargument) + "\r\n"
// #define RPL_TOPIC(client, channel, topic)                             ":" SERVERNAME " 332 " + (client) + " " + (channel) +  " :" + (topic) + "\r\n"
// #define RPL_NOTOPIC(client, channel)                                  ":" SERVERNAME " 331 " + (client) + " " + (channel) + " :No topic is set" + "\r\n"
// #define RPL_INVITING(client, nick, channel)                           ":" SERVERNAME " 341 " + (client) + " " + (nick) + " " + (channel) + "\r\n"
// #define RPL_TOPICWHOTIME(client, channel, nick, setat)                ":" SERVERNAME " 333 " + (client) + " " + (channel) + " " + (nick) + " " + (setat) + "\r\n"
// #define RPL_MOTD(client, sentence)                                    ":" SERVERNAME " 372 " + (client) + " " + (sentence) + "\r\n"
// #define ERR_UMODEUNKNOWNFLAG(client)                                  ":" SERVERNAME " 501 " + (client) + " :Unknown MODE flag" + "\r\n"
// #define ERR_INVALIDMODEPARAM(client, target, modechar, parameter)     ":" SERVERNAME " 696 " + (client) + " " + (target) + " " + (modechar) + " " + " You must specify a parameter for the key mode. Syntax: " + (parameter) + "\r\n"
// #define RPL_LIST(client, channel, nb_users, topic)                    ":" SERVERNAME " 322 " + (client) + " " + (channel) + " " + (nb_users) + " :" + (topic) + "\r\n"
// #define RPL_LISTSTART(client)                                         ":" SERVERNAME " 321 " + (client) + " Channel :Users Name" + "\r\n"
// #define RPL_LISTEND(client)                                           ":" SERVERNAME " 323 " + (client) + " :End of /LIST" + "\r\n"
// #define ERR_NORECIPIENT(client, command)                              ":" SERVERNAME " 411 " + (client) + " :No recipient given (" + (command) + ")" + "\r\n"
// #define ERR_NOORIGIN(client)                                          ":" SERVERNAME " 409 " + (client) + " :No origin specified" + "\r\n"
// #define ERR_NOSUCHSERVER(client, servername)                          ":" SERVERNAME " 402 " + (client) + " " + (servername) + " :No such server" + "\r\n"
// #define PONG(client, server)                                          "PONG " + (client) + " :" + (server) + "\r\n"
// #define PING(server)                                                  "PING " + (server) + "\r\n"

// #define RPL_WHOREPLY(client, channel, username, hostname, servername, nickname, mode, realname)     ":" SERVERNAME " 352 " + (client) + " " + (channel) + " " + (username) + " " + (hostname) + " " + (servername) + " " + (nickname) + " " + (mode) + " " + (realname) + "\r\n"

// debuging macro
// #ifndef DEBUG__
// #define DEBUG__ 0


#endif