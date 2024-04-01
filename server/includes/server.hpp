/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 01:34:27 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/31 23:38:15 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "clients.hpp"

#include <sys/socket.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string>
#include <map>


class Server {
  private:
    unsigned int _port;
    std::string _password;
    int _listen_sd;
    struct sockaddr_in   _addr;
	  struct pollfd _fds[5000];
    int    nfds;
    std::map<int, Clients> _clients;

  public:
        Server();
        ~Server();
        
        void setPort(unsigned int port);
        void setPassword(char *password);
        
        unsigned int getPort() const { return _port; }
        std::string getPassword() const { return _password; }

        int createServer();
        // void updateFileDescrior(int *ng);
};

#endif // SERVER_HPP