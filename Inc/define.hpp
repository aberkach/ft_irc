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

#define EVENT_READ 1 << 0
#define EVENT_WRITE 1 << 1
#define EVENT_RW (EVENT_WRITE | EVENT_READ)

#define SERVERNAME      "Irc_42.chat"
#define VERSION         "1.6"

#define CNICKLEN        "10"
#define CUSERLEN        "9"
#define CREALLEN        "32"

#define MAXCLIENT       "nolim"

#define CHANELMODES     "itkol"
#define COMMANDS        "CHARSET=ascii PASS,NICK,USER,JOIN,LIST,KICK,WHOIS,PING,PONG,INVITE,TOPIC,MODE,PRIVMSG,QUIT,PART CHANTYPES=# CHANMODES=i,t,k,o,l"

#define AUTH                            ":" SERVERNAME " NOTICE AUTH :*** Looking up your hostname...\r\n"\
                                        ":" SERVERNAME " NOTICE AUTH :*** Couldn't look up your hostname\r\n"\
                                        ":" SERVERNAME " NOTICE AUTH :*** You are connected to the server as 'Guest', You have to authenticate to use this server\r\n"\
                                        ":" SERVERNAME " NOTICE AUTH :*** Available commands for the authentication process\r\n"\
                                        "                            PASS <password>\r\n"\
                                        "                            NICK <nickname>\r\n"\
                                        "                            USER <username> 0 * <realname>\r\n"\
                                        ":" SERVERNAME " NOTICE AUTH :*** NICKLEN" CNICKLEN " USERLEN=" CUSERLEN " REALLEN=" CREALLEN " characters\n"\
                                        "                            if u pass the token, the value will be truncated to fit the server needs\r\n"

#define RPL_WELCOME(nick, user, host)   ":" SERVERNAME " 001 " + (nick) + " :Welcome to the internet relay chat Network, " + (nick) + "[!" + (user) + "@" + (host) + "]\r\n"
#define RPL_YOURHOST(nick, host, port)  ":" SERVERNAME " 002 " + (nick) + " :Your host is " SERVERNAME "[" + (host) + "/" + (port) + "], running version " VERSION "\r\n"
#define RPL_CREATED(nick, datetime)     ":" SERVERNAME " 003 " + (nick) + " :This server was created " + (datetime) + "\r\n"
#define RPL_MYINFO(nick)                ":" SERVERNAME " 004 " + (nick) + " " SERVERNAME " " VERSION " [" CHANELMODES "]\r\n"
#define RPL_ISUPPORT(nick)              ":" SERVERNAME " 005 " + (nick) + " " COMMANDS " :are supported by this server\r\n"

#define RPL_LUSERME(nick, count)        ":" SERVERNAME " 255 " + (nick) + " :I have " + (count) + " clients on this servers\r\n"
#define RPL_LOCALUSERS(nick, count)     ":" SERVERNAME " 265 " + (nick) + " " + (count) + " :Current global users " + (count) + " , max " MAXCLIENT "\r\n"
#define RPL_MOTDSTART(nick)             ":" SERVERNAME " 375 " + (nick) + " :- " SERVERNAME " Message of the Day -\r\n"
#define RPL_MOTD(nick)                  ":" SERVERNAME " 372 " + (nick) + " :-                                                                            \r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :-  ██▓ ██▀███  ▄████▄       ██████ ▓█████  ██▀███   ██▒   █▓▓█████  ██▀███   \r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :- ▓██▒▓██ ▒ ██▒██▀ ▀█     ▒██    ▒ ▓█   ▀ ▓██ ▒ ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒ \r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :- ▒██▒▓██ ░▄█ ▒▓█    ▄    ░ ▓██▄   ▒███   ▓██ ░▄█ ▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒░\r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :- ██░▒██▀▀█▄ ▒▓▓▄ ▄██▒     ▒   ██▒▒▓█  ▄ ▒██▀▀█▄    ▒██ █░░▒▓█  ▄ ▒██▀▀█▄   ░\r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :- ██░░██▓ ▒██▒ ▓███▀ ░   ▒██████▒▒░▒████▒░██▓ ▒██▒   ▒▀█░  ░▒████▒░██▓ ▒██▒  \r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :- ░▓  ░ ▒▓ ░▒▓░ ░▒ ▒  ░   ▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒▓ ░▒▓░   ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░ \r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :-  ▒ ░  ░▒ ░ ▒░ ░  ▒      ░ ░▒  ░ ░ ░ ░  ░  ░▒ ░ ▒░   ░ ░░   ░ ░  ░  ░▒ ░ ▒░ \r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :-   ▒ ░  ░░   ░░           ░  ░  ░     ░     ░░   ░      ░░     ░     ░░   ░ \r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :-    ░     ░    ░ ░               ░     ░  ░   ░           ░     ░  ░   ░    \r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :-                                                                            \r\n"\
                                        ":" SERVERNAME " 372 " + (nick) + " :-                                                                            \r\n"
#define RPL_ENDOFMOTD(nick)             ":" SERVERNAME " 376 " + (nick) + " :End of /MOTD command.\r\n"
// privmsg responses
#define ERR_NOSUCHNICK(nick, target)                                  ":" SERVERNAME " 401 " + (nick) + " " + (target) + " :No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL(client, channel)                            ":" SERVERNAME " 403 " + (client) + " " + (channel) + " :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(nick)                                    ":" SERVERNAME " 404 " + (nick) + " :Cannot send to nick/channel\r\n"
#define ERR_NORECIPIENT(nick, command)                                ":" SERVERNAME " 411 " + (nick) + " :No recipient given (" + (command) + ")\r\n"
#define ERR_NOTEXTTOSEND(nick)                                        ":" SERVERNAME " 412 " + (nick) + " :No text to send\r\n"
#define PRIVMSG(nick, username, host, target, message)                ":" + (nick) + "!" + (username) + "@" + (host) + " PRIVMSG " + (target) + " :" + (message) + "\r\n"
//nick responses
#define ERR_UNKNOWNCOMMAND(nick, command)                             ":" SERVERNAME " 421 " + (nick) + " " + (command) + " :Unknown command\r\n"
#define ERR_NONICKNAMEGIVEN(nick)                                     ":" SERVERNAME " 431 " + (nick) + " :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick)                                    ":" SERVERNAME " 432 " + (nick) + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick)                                       ":" SERVERNAME " 433 " + (nick) + " :Nickname is already in use\r\n"
#define CHANGENICK(nick, username, host, message)                     ":" + (nick) + "!" + (username) + "@" + (host) + " NICK :" + (message) + "\r\n"
#define ERR_NOTREGISTERED(nick)                                       ":" SERVERNAME " 451 " + (nick) + " :You have not registered\r\n"
//user responses
#define ERR_USERFORMAT                                                ":" SERVERNAME " 460 Guest :Use format USER <username> 0 * <realname>\r\n"
#define ERR_NEEDMOREPARAMS(nick, command)                             ":" SERVERNAME " 461 " + (nick) + " " + (command) + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(nick)                                   ":" SERVERNAME " 462 " + (nick) + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH                                            ":" SERVERNAME " 464 Guest :Password incorrect\r\n"
#define ERR_FIRSTCOMMAND                                              ":" SERVERNAME " 451 Guest :Server is expecting 'PASS <password>' first\r\n"
//channel responses
#define ERR_NOTONCHANNEL(client, channel)                             ":" SERVERNAME " 442 " + (client) + " " + (channel) + " :You're not on that channel\r\n"
#define ERR_ALREADYINCHANNEL(client, nick, channel)                   ":" SERVERNAME " 443 " + (client) + " " + (nick) + " " + (channel) + " :is already on channel\r\n"
#define ERR_BADCHANNELKEY(client, channel)                            ":" SERVERNAME " 475 " + (client) + " " + (channel) + " :wrong channel key (+k)\r\n"
#define ERR_BADCHANMASK(channel)                                      ":" SERVERNAME " 476 " + (channel) + " :Bad Channel Mask\r\n"
//mode responses
#define RPL_CHANNELMODEIS(client, channel, params)                    ":" SERVERNAME " 324 " + (client) + " " + (channel) + " " + (params) + "\r\n"
#define RPL_CREATIONTIME(client, channel, creation_time)              ":" SERVERNAME " 329 " + (client) + " " + (channel) + " " + (creation_time) + "\r\n"
#define ERR_CHANNELISFULL(client, channel)                            ":" SERVERNAME " 471 " + (client) + " " + (channel) + ": Cannot channel (+l)\r\n"
#define ERR_USERNOTINCHANNEL(client, nick, channel)                   ":" SERVERNAME " 441 " + (client) + " " + (nick) + " " + (channel) + ": They aren't on that channel\r\n"
#define ERR_INVITEONLYCHAN(client, channel)                           ":" SERVERNAME " 473 " + (client) + " " + (channel) + ":Cannot join channel (+i)\r\n"
#define ERR_UNKNOWNMODE(client, charac)                               ":" SERVERNAME " 472 " + (client) + " " + (charac) + " :is unknown mode char to me\r\n"
#define RPL_YOUREOPER(client)                                         ":" SERVERNAME " 381 " + (client) + " :You are now an IRC operator\r\n"
#define MODE_SET(nick, username, host, channel, modes)                ":" + (nick) + "!" + (username) + "@" + (host) + " " + (channel) + " MODE " + (modes) + "\r\n"
// key responses
#define ERR_INVALIDKEY (client, channel)                              ":" SERVERNAME " 525 " + (client) + " " + (channel) + " :Key is not well-formed\r\n"
//operator responses
#define ERR_CHANOPRIVSNEEDED(client, channel)                         ":" SERVERNAME " 482 " + (client) + " " + (channel) + " :You're not channel operator\r\n"
#define RPL_WHOISOPERATOR(client)                                     ":" SERVERNAME " 313 " + (client) + " " + " :is channel operator" + "\r\n"
//join responses
#define RPL_JOIN(nick, username, channelname, ipaddress)              ":" + (nick) + "!" + (username) + "@" + (ipaddress) + " JOIN " + (channelname) + "\r\n"
#define RPL_NAMREPLY(clients, channelname, nick)                      ":" SERVERNAME " 353 " + (nick) + " = " + (channelname) + " :" + (clients) + "\r\n"
#define RPL_ENDOFNAMES(client, channel)                               ":" SERVERNAME " 366 " + (client) + " " + (channel) + " :End of /NAMES list" + "\r\n"
//topic responses
#define RPL_NOTOPIC(client, channel)                                  ":" SERVERNAME " 331 " + (client) + " " + (channel) + " :No topic is set\r\n"
#define RPL_TOPICSETBY(nickname, username, ipaddr, channel, topic)    ":" + (nickname) + "!" + (username) + "@" + (ipaddr) + " TOPIC " + channel + " :" + (topic) + "\r\n"
#define RPL_TOPIC(client, channel, topic)                             ":" SERVERNAME " 332 " + (client) + " " + (channel) + " :" + (topic) + "\r\n"
//kick responses
#define RPL_KICK(kicker, username, host, channel, targetuser, reason) ":" + (kicker) + "!" + (username) + "@" + (host) + " KICK " + (channel) + " " + (targetuser) + " :" + (reason) + "\r\n"
//invite responses
#define RPL_INVITING(inviting, invited, channel)                      ":" SERVERNAME " 341 " + inviting + " inviting " + invited + " to " + channel + "\r\n"
#define RPL_INVITED(nick, username, clienthostname, invited, channel) ":" + nick + "!" + username + "@" + clienthostname + " INVITE " + invited + " :" + channel + "\r\n"
#define INVITE_MSG(client, ipaddr, channel, invited)                  ":" SERVERNAME " " + (client) + " !~ " + (ipaddr) + " INVITE " + (channel) + " " + (invited) + "\r\n"
//quit responses
#define QUIT_MSG(nickname, username, ipaddr, reason)                  ":" + (nickname) + "!" + (username) + "@" + (ipaddr) + " QUIT :Client Quit" + (reason) + "\r\n"
//part responses
#define PART_MSG(nickname, username, ipaddr, channel, reason)         ":" + (nickname) + "!" + (username) + "@" + (ipaddr) + " PART " + (channel) + " " + (reason) + "\r\n"
//list responses
#define RPL_LISTSTART(client)                                         ":" SERVERNAME " 321 " + (client) + " Channel :Users Name" + "\r\n"
#define LIST_MSG(client, channel, nb_users, topic)                    ":" SERVERNAME " 322 " + (client) + " " + (channel) + " " + (nb_users) + " :" + (topic) + "\r\n"
#define RPL_LISTEND(client)                                           ":" SERVERNAME " 323 " + (client) + " :End of /LIST" + "\r\n"
//ping/pong responses
#define PING(target)                                                  "PING :" + (target) + "\r\n"
#define PONG(target)                                                  "PONG :" + (target) + "\r\n"

#define ERR_INVALIDMODEPARAM(client, target, modechar, parameter)     ":" SERVERNAME " 696 " + (client) + " " + (target) + " " + (modechar) + " " + " You must specify a parameter for the key mode. Syntax: " + (parameter) + "\r\n"

#endif 