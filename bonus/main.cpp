#include "include/ircBot.hpp"

int main(int ac, char **av) {
    uint16_t port;

    if ((port = arg_checker(ac ,av)) != 1)
    {
        try {
            Bot agent;

            agent.startBot(port);
            (ac == 2) ? agent.authenticate("") : agent.authenticate(av[2]);

            agent.listenForCommand();

        } catch (std::exception &e) {
            std::cerr << RED << e.what() << std::endl;
            return port;
        }
    }
    return 1;
};