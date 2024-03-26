/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 01:34:27 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/25 20:03:18 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
// provides required data types
#include <sys/types.h>
// holds address family and socket functions
#include <sys/socket.h>
// has the sockaddr_in structure
#include <netinet/in.h>
// has functions for read and write operations
#include <fcntl.h>
// basic C header
#include <stdio.h>
// header to help with strings
#include <string.h>
// has macros such as EXIT_FAILURE
#include<stdlib.h>

#include <iostream>
// port through which connection is to be made
#define CONNECTION_PORT 3500


class Server {
  private:
    unsigned int _port;
    std::string _password;

  public:
        Server();
        ~Server();
        
        void setPort(unsigned int port);
        void setPassword(char *password);
        
        unsigned int getPort() const { return _port; }
        std::string getPassword() const { return _password; }

        int createTestServer();
};

#endif // SERVER_HPP