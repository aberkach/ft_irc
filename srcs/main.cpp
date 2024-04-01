#include "../Inc/ft_irc.hpp"

// workflow diagram client 
//socket --> connect ---> recieve

//workflow diagram for server

//socket --> bind --> listen --> accept 

// int main(int ac, char **av)
// {
//     uint16_t port;

//     if ((port = health::arg_checker(ac ,av)) != 1)
//     {
//         int sock = socket(PF_INET, SOCK_STREAM, 0);
//         if (!sock)
//             return 2;
//         sockaddr_in serv_info;
//         serv_info.sin_family = PF_INET;
//         //listen on all available network interfaces (like localhost, LAN, etc.). 
//         //This means the socket can receive connections from any of the network 
//         //interfaces on your system.
//         serv_info.sin_family = INADDR_ANY;
//         serv_info.sin_port = htons(port);

//         // Host TO Network Short". It converts a 16-bit number from host byte order to network byte order.
//         // Network protocols require that the byte order of data sent over the network is in a standard format  called network byte order, which is big-endian. However, the byte order used by the host machine (host byte order) can be either big-endian or little-endian, depending on the machine's architecture.
        
//         // struct sockaddr test;
//         // test
//         if (bind(sock, (struct sockaddr *)&serv_info, sizeof(serv_info)))
//             return 2;


//         std::cout << HEADER << std::endl << port << " port number "<< std::endl;
//         if (listen(sock,5))
//             return 2;
//         struct pollfd fds[1024];
//         // fds.
//         while (true) 
//         {
//             int new_sock = ();
//             if (new_sock == -1) 
//             {
//                 perror("accept failed");
//                 return 4;
//             }

//             char buffer[1024] = {0};
//             ssize_t valread = recv(new_sock , buffer, 1024, 0);
//             if (valread == -1) {
//                 perror("recv failed");
//                 return 5;
//             }
//             printf("%s\n",buffer );
//         }
//         // Server IRC(port,av[2]);
//         // fd_set readfds;   
//         // FD_ZERO(&readfds);
//         // // FD_SET(socket, &readfds);


//         return (0);
//     }
//     return(port);
// }


int main(int ac, char **av)
{
    uint16_t port;

    if ((port = health::arg_checker(ac ,av)) != 1)
    {
        Server srv;

        srv.setPort(port);
        srv.setPassword(av[2]);
        srv.createServer();
        return (0);
    }
    return(port);
}
