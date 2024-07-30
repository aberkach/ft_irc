/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:32:12 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/26 06:33:29 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../Inc/define.hpp"
#include <string>
#include <vector>

class Client;
class Channel 
{
  private:
      std::string	                  _name;         // name of the channel
      std::string	                  _topic;        // topic of the channel
      std::map<std::string, Client>   _users;        // list of users in the channel  ===> this should be modifed the nick names should be gotten from the client struct for key value // TODO
      std::vector<Client>             _chanInvites;  // list of invited users in the channel

      bool                            _isInviteOnly; // if the channel is invite only
      bool                            _topicFlag;    // if the topic is set
      std::string	                  _key;          // password of the channel
      std::vector<Client>             _chanOps;      // list of operators in the channel
      size_t                          _maxUsers;     // max number of users in the channel
      Channel();

  
  public:
    Channel(const std::string &name);
    // Channel(const Channel &src);
    ~Channel();

    std::string getName() const;
    std::string getTopic() const;
    std::string getKey() const;
    bool getIsInviteOnly() const;
    size_t getMaxUsers() const;
    std::string getChannelModes();

	Client &getUser(std::string &nickName);
    std::string getUserName(const std::string &clientName);
    std::map<std::string, Client> &getUsers() ;
    std::string getChannelUsersInString();
    bool getTopicFlag() const;

    void setName(const std::string &name);
    void setTopic(const std::string &topic);
    void setKey(const std::string &key);
    void setIsInviteOnly(bool isInviteOnly);
    void setMaxUsers(size_t maxUsers);
    void setTopicFlag(bool topicFlag);

    void addUser(Client &user);
	void removeUser(Client &client);
    bool isClientExist(const std::string &nickName);
	void addOperator(const Client &op);
    void removeOperator(const Client &op);
	bool isOperator(const std::string &nickName);
    std::vector<Client> getOperator();

    void addInvite(const Client &invited);
    bool isInvited(const Client &invited);
    void removeInvite(const Client &invited);

    void broadCast(const std::string &msg, int excludedFd);
};