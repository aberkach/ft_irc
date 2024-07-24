#include "include/ircBot.hpp"

int main(int ac, char **av) {
    uint16_t port;

    if ((port = arg_checker(ac ,av)) != 1)
    {
        try {
            Bot agent;

            agent.startBot(port);
            agent.authenticate(av[2]);
            agent.listenForCommand();

        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return -10;
        }
    }
    return port;
};