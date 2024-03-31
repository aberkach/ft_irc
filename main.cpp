/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 20:20:32 by abberkac          #+#    #+#             */
/*   Updated: 2024/03/30 04:42:47 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/includes/server.hpp"
#include <iostream>



bool isJustDigits(const std::string& str) {
    for (size_t i = 0; i < str.length(); ++i)  {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}



bool checkArgs(char **av) {
    if (isJustDigits(av[1]))
        return true;
    else {
        std::cerr << "Invalid port number" << std::endl;
        return false;
    }
    return true;
}

int main(int ac, char **av) {
    
    Server srv;
    try {
        if (ac == 3) {
            if (checkArgs(av)) {
                srv.setPort(std::stoi(av[1]));
                srv.setPassword(av[2]);
            }
        }
        else {
            std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
            return 1;
        }
        srv.createServer();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}