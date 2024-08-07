/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 06:22:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/07/27 07:27:07 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/ft_irc.hpp"
#include "server/server.hpp"
#include <sys/signal.h>

int main(int ac, char **av)
{
    uint16_t port;
    if ((port = health::arg_checker(ac ,av)) != 1)
    {
        try {
            Server srv(port, av[2]);
            srv.createServer();
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return(port);
}