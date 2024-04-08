/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:32:12 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/07 23:06:48 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP__
#define CHANNEL_HPP__

#include "../../Inc/define.hpp"
#include <string>
#include <map>

#include "../client/client.hpp"
class Channel {
  private:
    std::string                 _name; // name of the channel
    std::string                 _topic; // topic of the channel
    std::string                 _key; // key of the channel
    std::map<std::string, Client>         _users; // list of users in the channel
  public:
    Channel();
    Channel(std::string name);
    Channel(const Channel &src);
    ~Channel();

    std::string getName() const;
    std::string getKey() const; 

    void setName(std::string name);
    void setKey(std::string key);

    bool isClientExist(int clientFd);
    void addUser(Client &client);
};

#endif