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

#define EVENT_READ 1 << 0 
#define EVENT_WRITE 1 << 1
#define EVENT_RW (EVENT_WRITE | EVENT_READ)

#define SERVERNAME      "Irc_42.chat"
#define VERSION         "1.8"

#define NICKNAMELEN        "10"
#define USERNAMELEN        "9"
#define REALNAMELEN        "32"

#define MAXCLIENT       "nolim"

#define CHANELMODES     "itkol"
#define COMMANDS        "PASS NICK USER JOIN LIST KICK PING PONG INVITE TOPIC MODE PRIVMSG QUIT PART CHARSET=ascii CASEMAPPING=rfc1459 CHANTYPES=#"

#define AUTH                            ":" SERVERNAME " NOTICE AUTH :*** Looking up your hostname...\r\n"\
                                        ":" SERVERNAME " NOTICE AUTH :*** Couldn't look up your hostname\r\n"\
                                        ":" SERVERNAME " NOTICE AUTH :*** You are connected to the server as 'Guest', You have to authenticate to use this server\r\n"\
                                        ":" SERVERNAME " NOTICE AUTH :*** Available commands for the authentication process\r\n"\
                                        "                            PASS <password>\r\n"\
                                        "                            NICK <nickname>\r\n"\
                                        "                            USER <username> 0 * <realname>\r\n"\
                                        ":" SERVERNAME " NOTICE AUTH :*** NICKNAMELEN" NICKNAMELEN " USERNAMELEN=" USERNAMELEN " REALNAMELEN=" REALNAMELEN " characters\n"\
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
#define ERR_FIRSTCOMMAND                                              ":" SERVERNAME " 451 Guest :Server is expecting 'PASS <password>' first\r\n"
#define ERR_USERFORMAT                                                ":" SERVERNAME " 460 Guest :Use format USER <username> 0 * <realname>\r\n"
#define ERR_PASSWDMISMATCH                                            ":" SERVERNAME " 464 Guest :Password incorrect\r\n"


#define ERR_NOTEXTTOSEND(nick)                                        ":" SERVERNAME " 412 " + (nick) + " :No text to send\r\n"
#define ERR_NONICKNAMEGIVEN(nick)                                     ":" SERVERNAME " 431 " + (nick) + " :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick)                                    ":" SERVERNAME " 432 " + (nick) + " :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick)                                       ":" SERVERNAME " 433 " + (nick) + " :Nickname is already in use\r\n"
#define ERR_NOTREGISTERED(nick)                                       ":" SERVERNAME " 451 " + (nick) + " :You have not registered\r\n"
#define ERR_ALREADYREGISTERED(nick)                                   ":" SERVERNAME " 462 " + (nick) + " :You may not reregister\r\n"
#define RPL_LISTEND(nick)                                             ":" SERVERNAME " 323 " + (nick) + " :End of /LIST\r\n"
#define ERR_NORECIPIENT(nick, command)                                ":" SERVERNAME " 411 " + (nick) + " :No recipient given (" + (command) + ")\r\n"
#define RPL_LISTSTART(nick)                                           ":" SERVERNAME " 321 " + (nick) + " Channel :Users Name\r\n"
#define RPL_INVITING(nick, invited, channel)                          ":" SERVERNAME " 341 " + (nick) + " inviting " + (invited) + " to " + (channel) + "\r\n"
#define ERR_NOSUCHNICK(nick, target)                                  ":" SERVERNAME " 401 " + (nick) + " " + (target) + " :No such nick/channel\r\n"
#define ERR_UNKNOWNMODE(nick, charac)                                 ":" SERVERNAME " 472 " + (nick) + " " + (charac) + " :is unknown mode char to me\r\n"
#define ERR_UNKNOWNCOMMAND(nick, command)                             ":" SERVERNAME " 421 " + (nick) + " " + (command) + " :Unknown command\r\n"
#define ERR_NEEDMOREPARAMS(nick, command)                             ":" SERVERNAME " 461 " + (nick) + " " + (command) + " :Not enough parameters\r\n"
#define ERR_ALREADYINCHANNEL(nick, client, channel)                   ":" SERVERNAME " 443 " + (nick) + " " + (client) + " " + (channel) + " :is already on channel\r\n"
#define ERR_NOSUCHCHANNEL(nick, channel)                              ":" SERVERNAME " 403 " + (nick) + " " + (channel) + " :No such channel\r\n"
#define ERR_NOTONCHANNEL(nick, channel)                               ":" SERVERNAME " 442 " + (nick) + " " + (channel) + " :You're not on that channel\r\n"
#define ERR_BADCHANNELKEY(nick, channel)                              ":" SERVERNAME " 475 " + (nick) + " " + (channel) + " :Invalid key\r\n"
#define ERR_INVALIDKEY(nick, channel)                                 ":" SERVERNAME " 525 " + (nick) + " " + (channel) + " :wrong channel key (+k)\r\n"
#define ERR_BADCHANMASK(nick, channel)                                ":" SERVERNAME " 476 " + (nick) + " " + (channel) + " :Bad Channel Mask\r\n"
#define RPL_CHANNELMODEIS(nick, channel, params)                      ":" SERVERNAME " 324 " + (nick) + " " + (channel) + " " + (params) + "\r\n"
#define RPL_CREATIONTIME(nick, channel, creation_time)                ":" SERVERNAME " 329 " + (nick) + " " + (channel) + " " + (creation_time) + "\r\n"
#define ERR_CHANNELISFULL(nick, channel)                              ":" SERVERNAME " 471 " + (nick) + " " + (channel) + ": Cannot channel (+l)\r\n"
#define ERR_INVITEONLYCHAN(nick, channel)                             ":" SERVERNAME " 473 " + (nick) + " " + (channel) + ":Cannot join channel (+i)\r\n"
#define ERR_CHANOPRIVSNEEDED(nick, channel)                           ":" SERVERNAME " 482 " + (nick) + " " + (channel) + " :You're not channel operator\r\n"
#define RPL_ENDOFNAMES(nick, channel)                                 ":" SERVERNAME " 366 " + (nick) + " " + (channel) + " :End of /NAMES list.\r\n"
#define RPL_TOPIC(nick, channel, topic)                               ":" SERVERNAME " 332 " + (nick) + " " + (channel) + " :" + (topic) + "\r\n"
#define LIST_MSG(nick, channel, nb_users, topic)                      ":" SERVERNAME " 322 " + (nick) + " " + (channel) + " " + (nb_users) + " :" + (topic) + "\r\n"
#define RPL_NAMREPLY(clients, channel, nick)                          ":" SERVERNAME " 353 " + (nick) + " = " + (channel) + " :" + (clients) + "\r\n"

#define PRIVMSG(nick, username, host, target, message)                ":" + (nick) + "!" + (username) + "@" + (host) + " PRIVMSG " + (target) + " :" + (message) + "\r\n"
#define CHANGENICK(nick, username, host, message)                     ":" + (nick) + "!" + (username) + "@" + (host) + " NICK :" + (message) + "\r\n"
#define MODE_SET(nick, username, host, channel, modes)                ":" + (nick) + "!" + (username) + "@" + (host) + " MODE " + (channel) + " " + (modes) + "\r\n"
#define RPL_JOIN(nick, username, channel, host)                       ":" + (nick) + "!" + (username) + "@" + (host) + " JOIN :" + (channel) + "\r\n"
#define RPL_TOPICSETBY(nick, username, host, channel, topic)          ":" + (nick) + "!" + (username) + "@" + (host) + " TOPIC " + (channel) + " :" + (topic) + "\r\n"
#define RPL_KICK(nick, username, host, channel, kickedUser, reason)   ":" + (nick) + "!" + (username) + "@" + (host) + " KICK " + (channel) + " " + (kickedUser) + " :" + (reason) + "\r\n"
#define RPL_INVITED(nick, username, host, invited, channel)           ":" + (nick) + "!" + (username) + "@" + (host) + " INVITE " + (invited) + " :" + (channel) + "\r\n"
#define QUIT_MSG(nick, username, host, reason)                        ":" + (nick) + "!" + (username) + "@" + (host) + " QUIT :Client " + (reason) + "\r\n"
#define PART_MSG(nick, username, host, channel, reason)               ":" + (nick) + "!" + (username) + "@" + (host) + " PART " + (channel) + " " + (reason) + "\r\n"

#define PING(target)                                                  "PING " + (target) + "\r\n"
#define PONG(target)                                                  "PONG " + (target) + "\r\n"

#endif
