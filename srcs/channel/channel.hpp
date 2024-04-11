/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:32:12 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/11 03:05:29 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP__
#define CHANNEL_HPP__

#include "../../Inc/define.hpp"
#include "../client/client.hpp"
#include <string>

class Channel {
  private:
    std::string                 _name; // name of the channel
    std::string                 _topic; // topic of the channel
    std::string                 _key; // password of the channel
    std::map<std::string, Client>         _users; // list of users in the channel
  public:
    Channel();
    Channel(std::string name);
    Channel(const Channel &src);
    ~Channel();

    std::string getName() const;
    std::string getTopic() const;
    std::string getKey() const;
    
    void getUsers();
    
    void setName(std::string name);
    void setTopic(std::string topic);
    void setKey(std::string key);
    void addUser(Client &user);
    
    bool isClientExist(std::string nickName);
};

#endif