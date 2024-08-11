
#include "../../Inc/irc.hpp"

void
Server::topicCommand (const std::vector<std::string> &fields, Client &client)
{
    if (client.getRegistered() == true) {
        chanIt it;
        std::string clientHost;
        int size = fields.size();

        if (size == 1)
        {
            const std::string &chanName = fields[0];
            it = _channels.find(chanName);

            if (it != _channels.end())
            {
                if (it->second.isClientExist(client.getNickname()))
                {
                    const std::string &topic = it->second.getTopic();
                    clientHost = inet_ntoa(client.getAddr().sin_addr);

                    replyTo(client.getSocket(), RPL_TOPICSETBY(client.getNickname(), client.getUsername(), clientHost, chanName, (!topic.empty()) ? topic : "No topic set."));
                } else 
                    replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chanName));
            } else {
                clientHost = inet_ntoa(client.getAddr().sin_addr);
                replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chanName));
            }
        } else if (size > 1) {
            const std::string &defchanName = fields[0];
            it = _channels.find(defchanName);

            if (it != _channels.end())
            {
                if ((it->second.isOperator(client.getNickname()) || it->second.getTopicFlag() == true))
                {
                    const std::string &topic = fields[1];
                    clientHost = inet_ntoa(client.getAddr().sin_addr);

                    it->second.setTopic(topic);
                    it->second.broadCast(RPL_TOPICSETBY(client.getNickname(), client.getUsername(), clientHost, defchanName, it->second.getTopic()), -1);
                } else
                    replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), defchanName));
            } else {
                clientHost = inet_ntoa(client.getAddr().sin_addr);
                replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, defchanName));
            }
        } else
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC"));
    } else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
};

void Server::kickCommand (const std::vector<std::string> &fields, Client &client) {
    if (client.getRegistered())
    {
        if (fields.size() < 2) {
            replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
            return;
        }

        const std::string &chnName = fields[0];
        std::vector<std::string> usersBeKicked = splitByDelim(fields[1], ',');
        std::vector<std::string> reasons;
        if (fields.size() > 2)
            reasons = splitByDelim(fields[2], ',');
        chanIt joinedChnIt = _channels.find(chnName);
        
        if (joinedChnIt == _channels.end())
        {
            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName));
            return;
        }
        if (joinedChnIt->second.isClientExist(client.getNickname()) == false)
        {
            replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chnName));
            return;
        }
        if (joinedChnIt->second.isOperator(client.getNickname())) {
            for (size_t i = 0; i < usersBeKicked.size(); i++)
            {
                if (joinedChnIt->second.isClientExist(usersBeKicked[i]))
                {
                    // send a message to the client that has been kicked
                    std::string reason;
                    if (reasons.size() > i)
                        reason = reasons[i];
                    else
                        reason = "kicked by " + client.getNickname();
                    std::string clientHost = inet_ntoa(client.getAddr().sin_addr);

                    for (std::map<std::string, Client>::iterator it = joinedChnIt->second.getUsers().begin(); it != joinedChnIt->second.getUsers().end(); it++)
                    {
                        if (it->second.getNickname() == usersBeKicked[i])
                            replyTo(it->second.getSocket(), RPL_KICK(client.getNickname(), client.getRealname(), clientHost, chnName, usersBeKicked[i], ("you " + reason)));
                        else
                            replyTo(it->second.getSocket(), RPL_KICK(client.getNickname(), client.getRealname(), clientHost, chnName, usersBeKicked[i], reason));
                    }
                    joinedChnIt->second.removeUser(usersBeKicked[i], 1);
                    if (joinedChnIt->second.getUsers().size() == 1)
                        _channels.erase(joinedChnIt);
                }
                // if the client is not in the channel, send an error message to the client
                else if (joinedChnIt->second.isClientExist(usersBeKicked[i]) == false)
                    replyTo(client.getSocket(), ERR_USERNOTINCHANNEL(client.getNickname(), usersBeKicked[i], chnName));
            }
        }
        else
            replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), chnName));
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}

void Server::inviteCommand(const std::vector<std::string> &fields, Client &client) {
    if (fields.size() < 2) {
        replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
        return;
    }
    if (client.getRegistered()) {
        std::string invitedUser = fields[0];
        std::string chnName = fields[1];
        chanIt chnIt = _channels.find(chnName);
        if (chnIt->second.isOperator(client.getNickname()) == false)
        {
            replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), chnName));
            return;
        }
        if (chnIt == _channels.end())
        {
            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName));
            return;
        }
        if (chnIt->second.isClientExist(client.getNickname()))
        {
            // here we send a message to the client that has been invited
            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            replyTo(client.getSocket(), RPL_INVITING(client.getNickname(), invitedUser, chnName));

            if (isClientInServer(invitedUser))
            {
                std::string inviteMessage = RPL_INVITED(client.getNickname(), client.getUsername(), clientHost, invitedUser, chnName);
                for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
                {
                    if (it->second.getNickname() == invitedUser)
                    {
                        chnIt->second.addInvite(it->second);
                        replyTo(it->second.getSocket(), inviteMessage);
                    }
                }
            }
            else
                replyTo(client.getSocket(), ERR_NOSUCHNICK(client.getNickname(), invitedUser));
        }
        else
            replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chnName));
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}
