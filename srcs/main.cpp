
#include "../Inc/ft_irc.hpp"
#include "server/server.hpp"
#include <sys/signal.h>

int main(int ac, char **av)
{
    uint16_t port;

    if ((port = health::arg_checker(ac ,av)) != 1)
    {
        Server srv(port, av[2]);
        try {
            srv.createServer();
        } 
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            srv.cleanUp();
        }
    }
    return(port);
}
