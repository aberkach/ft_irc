#include "../../Inc/ft_irc.hpp"

uint16_t
health::arg_checker(int ac, char **av)
{
    switch (ac)
    {
        case 2 :
            std::cerr << RED  "ERROR: " RESET "IRC server missing argument <password>" << std::endl;
            return 1;
        case 3 :
            uint16_t port;

            if (!(port = valid_port(av[1])) || !valid_passord(av[2]))
                return 1;
            return port;
        default :
            std::cerr << RED  "ERROR: " RESET "IRC server requare 2 arguments. \n" YELLOW "try ./ircserv <port> <password>" << std::endl;
            return 1;
    }
}

uint16_t
health::valid_port(char *port_arg)
{
    size_t i;

    
    for (i = 0; port_arg[i] ;i++) // need to check for the port_arg range 
    {
        if (!isdigit(port_arg[i]) || i >= 5)
        {
            std::cerr << "port isnt a digit or longer then 5 chars" << std::endl; 
            return 1;
        }
    }
    uint32_t port = atoi(port_arg);
    if (1024 > port || 65535 < port)
    {
        std::cerr << "port range between 1024 and 65535" << std::endl; 
        return 1;
    }
    return port;
}

bool
health::valid_passord(char *password)
{
    size_t i;

    for (i = 0; password[i] ;i++)
    {
        if (isspace(password[i]) || i > 21)
        {
            std::cerr << "space or count more then 21" << std::endl; 
            return false;
        }
    }
    if (i < 12)
    {
        std::cerr << "no spacer in password and len between 12 and 21 chars" << i << std::endl; 
        return false;
    }
    return true;
}