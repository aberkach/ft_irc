/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 01:34:27 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/21 02:39:36 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include <stdexcept>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <cstring>

class Server {
  private:
    unsigned int _port;
    std::string _password;

  public:
        Server();
        ~Server();
        
        void setPort(unsigned int port);
        void setPassword(char *password);
};

#endif // SERVER_HPP