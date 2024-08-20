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

// server quites when recv fails using the python script
// upper case in all channels
// make nickname changes update across all channels
// broadcast nickname changes to all joined channels
// verify comments
// update some methods in channel.cpp and channel.hpp
// missing protections and some protections need to be removed 
// verify the server.cpp .hpp
// check cleanup
// check if there is eny unclosed socks

// void testLeaks(void)
// {
//     system("leaks ircserv");
// }

int main(int ac, char **av)
{
    // atexit(testLeaks);
    uint16_t port;
    
    if ((port = arg_checker(ac ,av)) != 1)
    {
        Server srv(port, av[2]);
        try {
            srv.createServer();
        }
        catch (std::exception &e) {
            srv.cleanUp();
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
};
