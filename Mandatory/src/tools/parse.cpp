
#include "../../Inc/irc.hpp"

uint16_t
valid_port(char *port_arg)
{
    size_t i;

    
    for (i = 0; port_arg[i] ;i++)
    {
        if (!isdigit(port_arg[i]) || i >= 5)
        {
            std::cerr << RED "Error: " RESET "Port isnt a digit or longer then 5 chars." << std::endl; 
            return 1;
        }
    }
    uint32_t port = atoi(port_arg);
    if (1024 > port || 65535 < port)
    {
        std::cerr << RED "Error: " RESET "Port range between 1024 and 65535." << std::endl; 
        return 1;
    }
    return port;
};

bool
valid_passord(char *password)
{
    size_t i;

    for (i = 0; password[i] ;i++)
    {
        if (isspace(password[i]))
        {
            std::cerr << "Error: " RESET "Password cant have spaces in it." << std::endl; 
            return false;
        }
    }
    if (i < 12 || i > 21)
    {
        std::cerr << RED "Error: " RESET "Password need to be between 12 and 21 characters long." << std::endl; 
        return false;
    }
    return true;
};

uint16_t
arg_checker(int ac, char **av)
{
    switch (ac)
    {
        case 2 :
            std::cerr << RED  "Error: " RESET "IRC server missing argument <password>" << std::endl;
            return 1;
        case 3 :
            uint16_t port;

            if (!(port = valid_port(av[1])) || !valid_passord(av[2]))
                return 1;
            return port;
        default :
            std::cerr << RED  "Error: " RESET "IRC server requare 2 arguments. \n" YELLOW "try ./ircserv <port> <password>" << std::endl;
            return 1;
    }
};
