/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yamajid <yamajid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:32:12 by abberkac          #+#    #+#             */
/*   Updated: 2024/05/12 15:23:30 by yamajid          ###   ########.fr       */
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
      std::string	                  _name; // name of the channel
      std::string	                  _topic; // topic of the channel
      std::string	                  _key; // password of the channel
      std::map<std::string, Client>   _users; // list of users in the channel
      std::vector<Client>             _chanOps; // list of operators in the channel
      std::vector<Client>             _chanInvites; // list of invited users in the channel
      bool                           _isInviteOnly; // if the channel is invite only
      size_t                        _maxUsers; // max number of users in the channel
  
  public:
    Channel();
    Channel(std::string name);
    // Channel(const Channel &src);
    ~Channel();

    std::string getName() const;
    std::string getTopic() const;
    std::string getKey() const;
    bool getIsInviteOnly() const;
    size_t getMaxUsers() const;

	Client &getUser(std::string &nickName);
    std::string getUserName(std::string clientName);
    std::map<std::string, Client> &getUsers();
    std::string &getChannelUsersInString();
    std::vector<std::string> getUsersList();

    void setName(std::string name);
    void setTopic(std::string topic);
    void setKey(std::string key);
    void setIsInviteOnly(bool isInviteOnly);
    void setMaxUsers(size_t maxUsers);

    void addUser(Client &user);
	void removeUser(Client &client);
    bool isClientExist(std::string nickName);
	void addOperator(Client &op);
    void removeOperator(Client &op);
	bool isOperator(std::string nickName);
    std::vector<Client> getOperator();

    void addInvite(Client &invited);
    bool isInvited(Client &invited);
    void removeInvite(Client &invited);

    void broadCast(std::string msg, int excludedFd);
};