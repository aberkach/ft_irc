// #include "../Inc/ft_irc.hpp"

// int main(int ac, char**av)
// {
//     // if (!health::arg_checker(ac))
//     // {
//     //     // creating a socket
//     //     // std::cout << "hello irc" << std::endl;
//         int network_sock = socket(PF_INET, SOCK_STREAM, 0);
//         if (network_sock == -1) {
//             std::cerr << "Failed to create server socket" << std::endl;
//             return 1;
//         }
//         // specify an adree for the socket
//         struct sockaddr_in serv;
//         serv.sin_family = PF_INET;
//         serv.sin_port = htons(9002);
//         serv.sin_addr.s_addr = INADDR_ANY;

//         if (connect(network_sock, (struct sockaddr *)&serv, sizeof(serv)) == -1)
//         {
//             std::cerr << "Failed to connect to client "<< std::endl;
//             return 1;
//         }
//         char buffer[2000];
//         recv(network_sock, &buffer,sizeof(buffer), 0); // extra flag

//         std::string message = buffer;
//         std::cout << "galik : "<< message << std::endl;

//         std::getline(std::cin, message);
//         const char* c_message = message.c_str();

//         send(network_sock, c_message, strlen(c_message), 0);

//         close(network_sock);

//     // }
//     // return 1;
// }