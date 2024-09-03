/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 06:22:34 by abberkac          #+#    #+#             */
/*   Updated: 2024/08/20 03:30:30 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Inc/irc.hpp"

int main(int ac, char **av)
{
    uint16_t port;

    if ((port = arg_checker(ac ,av)) != 1)
    {
        try {
            Server srv(port, av[2]);
            srv.createServer();
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
};
