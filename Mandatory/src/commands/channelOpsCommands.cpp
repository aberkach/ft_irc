
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
        if (fields.size() < 2)
            return(replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "KICK")));

        const std::string &chnName = fields[0];
        std::string usersBeKicked = fields[1];
        chanIt joinedChnIt = _channels.find(chnName);
        
        if (joinedChnIt == _channels.end()){
            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            return (replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName)));
        }
        if (joinedChnIt->second.isClientExist(client.getNickname()) == false)
            return(replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), chnName)));

        if (joinedChnIt->second.isOperator(client.getNickname())) {
            if (joinedChnIt->second.isClientExist(usersBeKicked))
            {
                std::string reason;
                if (fields.size() > 2)
                    reason = fields[2];
                else
                    reason = "kicked by " + client.getNickname();
                std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
                std::string KickErrMessage = RPL_KICK(client.getNickname(), client.getRealname(), clientHost, chnName, usersBeKicked, reason);
                for (std::map<std::string, Client>::iterator it = joinedChnIt->second.getUsers().begin(); it != joinedChnIt->second.getUsers().end(); it++)
                {
                    // if (it->second.getNickname() == usersBeKicked)
                    //     continue;
                    replyTo(it->second.getSocket(), KickErrMessage);
                }
                if (joinedChnIt->second.getUsers().size() == 1)
                    _channels.erase(joinedChnIt);
                else
                    joinedChnIt->second.removeUser(usersBeKicked, 1);
            }
            else
                replyTo(client.getSocket(), ERR_NOTONCHANNEL(usersBeKicked, chnName));
        }
        else
            replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), chnName));
    }
    else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(client.getNickname()));
}


void Server::inviteCommand(const std::vector<std::string> &fields, Client &client) {
    if (fields.size() < 2) 
        return (replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE")));

    if (client.getRegistered()) {
        std::string invitedUser = fields[0];
        std::string chnName = fields[1];
        chanIt chnIt = _channels.find(chnName);
        
        if (chnIt == _channels.end()){
            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            return (replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(clientHost, chnName)));
        }
        if (chnIt->second.isClientExist(client.getNickname())) {
            if (chnIt->second.isOperator(client.getNickname()) == false)
                return (replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), chnName)));

            std::string clientHost = inet_ntoa(client.getAddr().sin_addr);
            if (isClientInServer(invitedUser)) {
                std::string inviteMessage = RPL_INVITED(client.getNickname(), client.getUsername(), clientHost, invitedUser, chnName);
                for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
                    if (it->second.getNickname() == invitedUser) {
                        chnIt->second.addInvite(it->second);
                        replyTo(it->second.getSocket(), inviteMessage);
                    }
                }
                replyTo(client.getSocket(), RPL_INVITING(client.getNickname(), invitedUser, chnName));
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
