
#include "../../Inc/irc.hpp"

void
Server::passCommand(const std::vector<std::string> &fields, Client &client)
{
	if (client.getValidPass() == false)
	{
		if (fields.empty())
			replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(std::string("Guest"), "PASS"));
		else if (fields[0] == _password)
			client.setValidPass(true);
		else
			replyTo(client.getSocket(), ERR_PASSWDMISMATCH);
	}
	else
		replyTo(client.getSocket(), ERR_ALREADYREGISTERED(client.getNickname()));
};

void
Server::nickCommand(const std::vector<std::string> &fields, Client &client)
{
	if (client.getValidPass() == true)
	{
		if (fields.empty())
		{
			const std::string &name = client.getNickname();
			return (replyTo(client.getSocket(), ERR_NONICKNAMEGIVEN((name.empty()) ? std::string("Guest") : name)));
		}
		for (clientIt it = _clients.begin() ; it != _clients.end(); ++it)
		{
			if (stringUpper(it->second.getNickname()) == stringUpper(fields[0]))
				return (replyTo(client.getSocket(), ERR_NICKNAMEINUSE(fields[0])));
		}
        std::string oldNick = client.getNickname();
		if (client.setNickname(fields[0]) == false)
			return (replyTo(client.getSocket(), ERR_ERRONEUSNICKNAME(fields[0])));
        else if (client.getRegistered()) // update in channels
		{
			std::string msg = CHANGENICK(oldNick, client.getUsername(), inet_ntoa(client.getAddr().sin_addr), fields[0]);
			for (chanIt it = _channels.begin(); it != _channels.end(); ++it) {
				if (it->second.isClientExist(oldNick))
				{
					if (it->second.isOperator(oldNick)) {
						it->second.removeUser(oldNick, 1);
						it->second.addOperator(client);
					}
					else
						it->second.removeUser(oldNick, 0);
					it->second.addUser(client);
				}
				it->second.broadCast(msg, client.getSocket());
			}
            return (replyTo(client.getSocket(), msg));
		}
	}
	else
		return (replyTo(client.getSocket(), ERR_FIRSTCOMMAND));
	_clients[client.getSocket()].refStatus(_countCli);
};

void
Server::userCommand(const std::vector<std::string> &fields, Client &client)
{
	if (!client.getRegistered())
	{
		if (client.getValidPass())
		{
			if (fields.size() >= 4)
			{
                std::string realName = fields[3];
                if (realName.empty())
                    return (replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(std::string("Guest"), "USER")));
                // else if (!client.setUsername(fields[0])|| fields[1] != "0" || fields[2] != "*" || !client.setRealname(realName))
                else if (!client.setUsername(fields[0]) || !client.setRealname(realName))
                    return (replyTo(client.getSocket(), ERR_USERFORMAT));
            }
			else
				return (replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(std::string("Guest"), "USER")));
		}
		else
			return (replyTo(client.getSocket(), ERR_FIRSTCOMMAND));
	}
	else
		return (replyTo(client.getSocket(), ERR_ALREADYREGISTERED(client.getNickname())));
	_clients[client.getSocket()].refStatus(_countCli);
};
