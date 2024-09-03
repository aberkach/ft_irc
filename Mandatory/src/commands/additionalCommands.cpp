
#include "../../Inc/irc.hpp"

void
Server::privmsgCommand(const std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered())
    {
        if (fields.empty())
			replyTo(client.getSocket(), ERR_NORECIPIENT(client.getNickname(), "PRIVMSG"));
        else if (fields.size() >= 2)
        {
			if (fields[1].empty())
				return (replyTo(client.getSocket(), ERR_NOTEXTTOSEND(client.getNickname())));

            std::vector<std::string> clients = splitByDelim(fields[0], ',');

			for (std::vector<std::string>::iterator split = clients.begin(); split != clients.end(); ++split)
            {
				const std::string &target = *split;

				if (target[0] == '#')
				{
					chanIt it = _channels.find(target);
					if (it != _channels.end())
						it->second.broadCast(PRIVMSG(client.getNickname(), client.getUsername(), inet_ntoa(client.getAddr().sin_addr), target, fields[1]), client.getSocket());
					else
						replyTo(client.getSocket(), ERR_NOSUCHNICK(client.getNickname(), target));
				}
				else
				{
					for (clientIt it = _clients.begin() ; it != _clients.end(); ++it)
					{
						if (stringUpper(it->second.getNickname()) == stringUpper(target))
							return (replyTo(it->second.getSocket(), PRIVMSG(client.getNickname(), client.getUsername(), inet_ntoa(client.getAddr().sin_addr), it->second.getNickname(), fields[1])));
					}
					replyTo(client.getSocket(), ERR_NOSUCHNICK(client.getNickname(), target));
				}
            }
        }
        else
			replyTo(client.getSocket(), ERR_NOTEXTTOSEND(client.getNickname()));
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(std::string("GUEST")));
};

void
Server::pingCommad(const std::vector<std::string> &fields, Client &client)
{
    size_t size = fields.size();
    if (size >= 1 && fields[0] == SERVERNAME)
        replyTo(client.getSocket(),(client.getRegistered()) ? PONG(client.getNickname()) : PONG(std::string("Guest")));
    else if (size >= 1)
        replyTo(client.getSocket(), PONG(fields[0]));
    else
        replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname() ,"PING"));
};

void
Server::pongCommad(const std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered() == false || fields.size() >= 1)
        return ;
    replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname() ,"PONG"));
};

void
Server::namesCommad(const std::vector<std::string> &fields, Client &client)
{
    if (!client.getRegistered())
        replyTo(client.getSocket(), ERR_NOTREGISTERED(std::string("GUEST")));
    else if (fields.size() >= 1)
    {
        std::vector<std::string> names = splitByDelim(fields[0], ',');
        for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it)
        {
            chanIt chan = _channels.find(*it);
            if (chan != _channels.end())
                replyTo(client.getSocket(), RPL_NAMREPLY((chan->second.isClientExist(client.getNickname())) ? chan->second.getChannelUsersInString() : "" ,*it ,client.getNickname()));
            replyTo(client.getSocket(), RPL_ENDOFNAMES(client.getNickname(), *it));
        }
    }
    else
        replyTo(client.getSocket(), RPL_ENDOFNAMES(client.getNickname(), "*"));
};

void
Server::listCommand(const std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered() == false)
        replyTo(client.getSocket(), ERR_NOTREGISTERED(std::string("GUEST")));
    else
    {
        replyTo(client.getSocket(), RPL_LISTSTART(client.getNickname()));
        if (fields.size() >= 1)
        {
            std::vector<std::string> chnName = splitByDelim(fields[0], ',');
            for (size_t i = 0; i < chnName.size(); i++)
            {
                chanIt chnIt = _channels.find(chnName[i]);
                if (chnIt != _channels.end())
                {
                    const std::string &topic = chnIt->second.getTopic();
                    replyTo(client.getSocket(), LIST_MSG(client.getNickname(), chnName[i],
                        to_string(chnIt->second.getUsers().size()), (!topic.empty()) ? topic : "No topic set"));
                }
            }
        } else {
            for (chanIt it = _channels.begin(); it != _channels.end(); ++it)
            {
                const std::string &topic = it->second.getTopic();
                replyTo(client.getSocket(), LIST_MSG(client.getNickname(), it->second.getName(),
                        to_string(it->second.getUsers().size()), (!topic.empty()) ? topic : "No topic set"));
            }
        }
        replyTo(client.getSocket(), RPL_LISTEND(client.getNickname()));
    }
};

void
Server::partCommand (const std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered())
    {
        if (!fields.empty())
        {
            std::vector<std::string> channels = splitByDelim(fields[0], ',');

            for (size_t i = 0; i < channels.size(); i++)
            {
                const std::string &chnName = channels[i];
                chanIt chnIt = _channels.find(chnName);

                if (chnName.empty())
                    continue;
                if (chnIt != _channels.end())
                {
                    if (chnIt->second.isClientExist(client.getNickname()))
                    {
                        std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                        std::string reason = (fields.size() > 1) ? fields[1] : "left..."; 
                        chnIt->second.broadCast(PART_MSG(client.getNickname(), client.getUsername(), clientHost, chnName, reason), -1);
                        chnIt->second.removeUser(client.getNickname(), 1);
                        if (chnIt->second.getUsers().size() == 0)
                            _channels.erase(chnIt);
                    }
                    else
                        replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chnName));
                } else
                    replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(client.getNickname(), chnName));
            }
        } else
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
    } else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(std::string("GUEST")));
};

void Server::quitCommand(const std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered()) {
        std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
        std::string quitMessage = (fields.empty()) ?
                    QUIT_MSG(client.getNickname(), client.getUsername(), clientHost, "left...") :
                    QUIT_MSG(client.getNickname(), client.getUsername(), clientHost, fields[0]);

        for (chanIt it = _channels.begin(); it != _channels.end(); ) {
            if (it->second.isClientExist(client.getNickname())){
                it->second.broadCast(quitMessage, client.getSocket());
                it->second.removeUser(client.getNickname(), 1);
                if (it->second.getUsers().size() == 0)
                    it = _channels.erase(it);
                else
                    ++it;
            }
            else
                ++it;
        }
        clientIt cIt = _clients.find(client.getSocket());
        if (cIt != _clients.end())
        {
            replyTo(client.getSocket(), quitMessage);
            size_t i;
            for (i = 0; i < _pollFds.size(); i++)
            {
                if (_pollFds[i].fd == cIt->second.getSocket())
                {
                    _pollFds.erase(_pollFds.begin() + i);
                    break;
                }
            }
            close(cIt->second.getSocket());
            _clients.erase(cIt);
            _nfds--;
            _countCli--;
        }
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(std::string("GUEST")));
}