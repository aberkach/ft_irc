#ifndef FT_IRC_HPP__
#define FT_IRC_HPP__

#include "define.hpp"

#include "../srcs/Poller/Poller.hpp"
#include "../srcs/client/client.hpp"
#include "../srcs/tools/health.hpp"
// #include "../srcs/server/server.hpp"

// client connected to the server
#define RPL_WELCOME(client) ":irc.1337.ma 001 " + (client) + " :Welcome to the IRC Network, " + (client)

#define RPL_YOURHOST(client, servername, version) ":irc.1337.ma 002 " + (client) + " :Your host is " + (servername) + ", running version " + (version)
#define RPL_CREATED(client, time) ":irc.1337.ma 003 " + (client) + " :This server was created " + (time)
#define RPL_MYINFO(client, servername, version) ":irc.1337.ma 004 " + (client) + " :" + (servername) + " " + (version)
#define RPL_ISUPPORT(client)  ":irc.1337.ma 005 " + (client) + " :are supported by this server"

#define ERR_ALREADYREGISTERED(client) ":irc.1337.ma 462 " +  (client) + " :You may not reregister"
#define ERR_ERRONEUSNICKNAME(client, command) ":irc.1337.ma 432 " +  (client) + " " + command + " ::Erroneus nickname"
#define ERR_UNKNOWNCOMMAND(client, command) ":irc.1337.ma 421 " +  (client) + " " + (command) + " :Unknown command"
#define ERR_NOSUCHNICK(client1, client2) ":irc.1337.ma 401 " + (client1) + " " + (client2) + " :No such nick"
#define ERR_NEEDMOREPARAMS(client, command) ":irc.1337.ma 461 " + (client) + " " + (command) + " :Not enough parameters"
#define ERR_NOSUCHCHANNEL(client, channel)  ":irc.1337.ma 403 " + (client) + " " + (channel) + " :No such channel"
#define ERR_BADCHANNELKEY(client, channel)   ":irc.1337.ma 475 " + (client) + " " + (channel) + " :Cannot join channel (+k)"
#define ERR_NOTONCHANNEL(client, channel)  ":irc.1337.ma 442 " + (client) + " " + (channel) + " :You're not on that channel"
#define ERR_USERNOTINCHANNEL(client, nickname, channel) ":irc.1337.ma 441 " + (client) + " " + (nickname) + " " + (channel) + " :They aren't on that channel"
#define ERR_CHANOPRIVSNEEDED(client, channel) ":irc.1337.ma 482 " + (client) + " " + (channel) + " :You're not channel operator"
#define ERR_PASSWDMISMATCH(client) ":irc.1337.ma 464 " + (client) + " :Password incorrect"
#define ERR_USERONCHANNEL(client, nick, channel) ":irc.1337.ma 443 " + (client) + " " + (nick) + " " + (channel) + " :is already on channel"
#define ERR_INVITEONLYCHAN(client, channel) ":irc.1337.ma 473 " + (client) + " " + (channel) + " :Cannot join channel (+i)"
#define ERR_BADCHANMASK(channel)  ":irc.1337.ma 476 " + (channel) + " :Bad Channel Mask"
#define ERR_UNKNOWNMODE(client, modechar) ":irc.1337.ma 472 " + (client) + " " + (modechar) + " :is unknown mode char to me"
#define ERR_CHANNELISFULL(client, channel)  ":irc.1337.ma 471 " + (client) + " " + (channel) + " :Cannot join channel (+l)"
#define ERR_KEYALREADYSET(client, channel) ":irc.1337.ma 467 " + (client) + " " + (channel) + " :Channel key already set"
#define ERR_NORECIPENT(client, command) ":irc.1337.ma 411 " + (client) + " :No recipient given " + (command) + ""
#define ERR_ONEOUS(client, wrongnick) MY_HOSTNAME +" 432 " + (client) + " " + (wrongnick) + " :Erroneous Nickname"
#define ERR_NICKNAMEINUSE(client) ":irc.1337.ma 433 " + (client) +  " :Nickname is already in use"
#define ERR_NONICKNAMEGIVEN(client) ":irc.1337.ma 431 " + (client) + " :No nickname given"
#define ERR_NOTEXTTOSEND(client) ":irc.1337.ma 412 " + (client) + " :No text to send"
#define ERROR_MSG(reason) ":irc.1337.ma " + (reason)
#define ERR_NOTREGISTERED(client) ":irc.1337.ma 451 " + (client) + " :You have not registered"
#define RPL_ENDOFWHO(client) ":irc.1337.ma 315 " + (client) + " :End of /WHO list"

#define RPL_NICK(client, username,ipaddr, newnick)  ":" + (client) + "!~" + (username) + "@"+ (ipaddr) + " NICK :"+(newnick)
#define KICK_MSG(client, ipaddr, channel, kicked, reason) ":" + (client) + "!~" + (ipaddr) + " KICK " + (channel) + " " + kicked + " " + reason
#define QUIT_MSG(nickname, username, ipaddr, reason) ":"+ (nickname) + "!~" + (username) + "@" + (ipaddr) + " QUIT :Client Quit" + (reason)
#define PRIVMSG(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " PRIVMSG " + (target) + " :" + (message)
#define JOIN_SUCC(nickname, username, client_ip, channel) ":" + (nickname) + "!~" + (username) + "@" + (client_ip) + " JOIN " + (channel)
#define MODE_MSG(client, username,ipaddr, channel,added_mode, target) ":" + (client) + "!~" + (username) +"@" + (ipaddr) + " MODE " + (channel) + " " +(added_mode) +  target
#define MODE_MSG_2(client, username,ipaddr, channel,added_mode) ":" + (client) + "!~" + (username) +"@" + (ipaddr) + " MODE " + (channel) + " " +(added_mode)
#define PART_MSG(nickname, username, ipaddr, channel, reason) ":" + (nickname) + "!~" + (username) + "@" + ipaddr + " PART " + (channel) + " " + (reason)
#define NOTICE(client, username, ipaddr, target, message) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " NOTICE " + (target) + " :" + (message)
#define TOPIC_MSG(client, username, ipaddr, channel, topic) ":" + (client) + "!~" + (username) + "@" + (ipaddr) + " TOPIC " + (channel) + " :" + (topic)

#define RPL_NAMREPLY(client, channel, users_list) ":irc.1337.ma 353 " + (client) + " = " + (channel) + " :" + (users_list)
#define RPL_CREATIONTIME(client, channel, creation_time) ":irc.1337.ma 329 " + (client) + " " + (channel) + " " + (creation_time)
#define RPL_ENDOFNAMES(client, channel)  ":irc.1337.ma 366 " + (client) + " " + (channel) + " :End of /NAMES list"
#define RPL_ENDOFINVITELIST(client) ":irc.1337.ma 337 " + " :End of /INVITE list"

#define RPL_CHANNELMODEIS(client, channel, modestring, modargument) ":irc.1337.ma 324 " + (client) + " "+ (channel) + " " +(modestring) + " " + (modargument)
#define RPL_TOPIC(client, channel, topic) ":irc.1337.ma 332 " + (client) + " " + (channel) +  " :" + (topic)
#define RPL_NOTOPIC(client, channel)  ":irc.1337.ma 331 " + (client) + " " + (channel) + " :No topic is set"
#define RPL_INVITING(client, nick, channel)  ":irc.1337.ma 341 " + (client) + " " + (nick) + " " + (channel)
#define RPL_TOPICWHOTIME(client, channel, nick, setat) ":irc.1337.ma 333 " + (client) + " " + (channel) + " " + (nick) + " " + (setat)
#define RPL_MOTD(client, sentence) ":irc.1337.ma 372 " + (client) + " " + (sentence)
#define ERR_UMODEUNKNOWNFLAG(client) ":irc.1337.ma 501 " + (client) + " :Unknown MODE flag"
#define ERR_INVALIDMODEPARAM(client, target, modechar, parameter) ":irc.1337.ma 696 " + (client) + " " + (target) + " " + (modechar) + " " + " You must specify a parameter for the key mode. Syntax: " + (parameter)
#define RPL_LIST(client, channel, nb_users, topic) ":irc.1337.ma 322 " + (client) + " " + (channel) + " " + (nb_users) + " :" + (topic)
#define RPL_LISTSTART(client) ":irc.1337.ma 321 " + (client) + " Channel :Users Name"
#define RPL_LISTEND(client) ":irc.1337.ma 323 " + (client) + " :End of /LIST"
#define ERR_NORECIPIENT(client, command) ":irc.1337.ma 411 " + (client) + " :No recipient given (" + (command) + ")"
#define ERR_NOORIGIN(client) ":irc.1337.ma 409 " + (client) + " :No origin specified"
#define ERR_NOSUCHSERVER(client, servername) ":irc.1337.ma 402 " + (client) + " " + (servername) + " :No such server"
#define PONG(client, server) "PONG " + (client) + " :" + (server)
#define PING(server) "PING " + (server)

#define RPL_WHOREPLY(client, channel, username, hostname, servername, nickname, mode, realname) ":irc.1337.ma 352 " + (client) + " " + (channel) + " " + (username) + " " + (hostname) + " " + (servername) + " " + (nickname) + " " + (mode) + " " + (realname)


#endif // SERVER_HPP