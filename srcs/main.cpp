
#include "../Inc/ft_irc.hpp"

int main(int ac, char **av)
{
    uint16_t port;

    if ((port = health::arg_checker(ac ,av)) != 1)
    {
        Server srv(port, av[2]);
        srv.createServer();
    }
    return(port);
}
