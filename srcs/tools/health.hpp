
#ifndef HEALTH_HPP__
#define HEALTH_HPP__

#include "../../Inc/define.hpp"

class health 
{
    private:
        static uint16_t valid_port(char *port_arg);
        static bool valid_passord(char *password);
    public :
        static uint16_t arg_checker(int ac, char **av);
};

#endif