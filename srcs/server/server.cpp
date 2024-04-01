// #include "../../Inc/ft_irc.hpp"


// int server(void)
// {

//         char buffer[200] = "hello ma nigga";

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

//         if (bind(network_sock, (struct sockaddr *)&serv, sizeof(serv)) == -1)
//         {
//             std::cerr << "Failed to connect to server "<< std::endl;
//             return 1;
//         }

//         listen(network_sock,5);

//         int clinet_sock = accept(network_sock, NULL,NULL);

//         send(clinet_sock,&buffer,sizeof(buffer),0);

//         recv(clinet_sock,&buffer,sizeof(buffer), 0);

//         std::cout << buffer << std::endl;

//         close(network_sock);


// }