#include "server.hpp"
#include <cstddef>
#include <sstream>

void Err(std::string msg, int exitFalg)
{
	std::cerr << msg << std::endl;
	if (exitFalg)
		exit(1);
}

// default constructor :

Server::Server()
{
}

// parameterized constructor : initialize the server socket and set the port number
Server::Server(uint16_t port, char *password) : _port(port), _password(password)
{
	_fds.resize(1500);
	_listen_sd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket
	if (_listen_sd < 0)
		Err("socket() failed", 1);
	// _listen_sd < 0 ? Err("socket() failed", 1) : void(0);

	// Set socket option to allow address reuse
	int on = 1;
	if (setsockopt(_listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
	    close(_listen_sd);
		Err("setsockopt() failed", 1);
	}
	
	// Set socket to non-blocking mode
	if (fcntl(_listen_sd, F_SETFL, O_NONBLOCK) < 0) {
	    close(_listen_sd);
		Err("fcntl() failed", 1);
	}
	
	// Bind the socket to the specified address and port
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);

	if (bind(_listen_sd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0) {
	    close(_listen_sd);
		Err("bind() failed", 1);
	}

	// Start listening for incoming connections
	if (listen(_listen_sd, 128) < 0) {
	    perror("listen() failed");
	    close(_listen_sd);
	    exit(-1);
	}
	
	// Initialize the fds array with the server socket
	// memset(_fds, 0 , sizeof(_fds));
	_fds[0].fd = _listen_sd;
	_fds[0].events = POLLIN;
	_fds[0].revents = 0;
}

// Setters ::---------------------------------------------------------------------------------

// void Server::setPort(unsigned int port) {
//     if (port > 1023 && port <= 65535)
//         _port = port;
//     else
//         throw std::logic_error("Port number must be between 1024 and 65535");
// }

// void Server::setPassword(char *password) {
//     if (strlen(password) >= 8)
//         _password = password;
//     else
//         throw std::logic_error("Password must be at least 8 characters long");
// }

// ---------------------------------------------------------------------------------------------


// Handle incoming connections:
void Server::handlIncomeConnections() {
	if (_fds[0].revents == POLLIN)
	{
		int sock_len = sizeof(_addr);
		int newSck = accept(_listen_sd, (struct sockaddr *)&_addr, (socklen_t*)&sock_len);
	    if (newSck < 0)
	        perror("  accept() failed");
	    else {
			// Resize the fds array if it's full
			if (_nfds >= _fds.size())
				_fds.resize(_fds.size() * 2);
	        // Add the new client socket to the fds array and clientsFds map
	        std::cout << "New incoming connection - " << newSck << std::endl;
	        fcntl(newSck, F_SETFL, O_NONBLOCK);
			_fds[_nfds].fd = newSck;
	        _fds[_nfds].events = POLLIN;
			_fds[_nfds].revents = 0;
			// Add the new client to the clients map
	        _clients.insert(std::pair<int, Client>(newSck, Client(newSck)));
	        _nfds++;
	    }
	}
}

void Server::authentication(std::string message, std::map<int, Client>::iterator client)
{
	std::stringstream ss(message);
	std::string pass;
	// std::string user;
	// std::string nick;
	// int n;
	// char c;
	if ((ss >> pass || !ss.eof()) && pass != "PASS"){
		throw std::logic_error("You should Enter PASS <password>");
	}
	if (pass == "PASS"){
		if ((ss >> pass || !ss.eof()) && pass == _password){
			std::cout << "password correct" << std::endl;
			client->second.setRegistered(true);
		}
		else
			throw std::logic_error("password incorrect");
	}
	// if (client->second.getRegistered()){

	// 	if ((ss >> user || !ss.eof() ) && (user == "USER"  || user == "NICK")){
	// 		if (user == "USER"){
	// 			if (((ss >> user >> n  >> c >> nick) || !ss.eof()) && nick.size() == '\0'){
	// 				std::cout << "user : " << user << std::endl;
	// 				std::cout << "n : " << n << std::endl;
	// 				std::cout << "c : " << c << std::endl;
	// 				return ;
	// 			}
	// 		}
	// 		else if (user == "NICK"){
	// 			if (ss >> user || !ss.eof()){
	// 				std::cout << "user : " << user << std::endl;
	// 				return ;
	// 			}
	// 		}
	// 		else
	// 			throw std::logic_error("Wrong command");
	// 	}
	// }
}

// Handle incoming data from clients :
void Server::handleIncomeData() {
	char buffer[1024];
	int rc;
	for (size_t i = 1; i < _nfds; i++) {
		if (_fds[i].revents & POLLIN) {
			rc = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
			if (rc < 0)
				Err("recv() failed", 0);
			else if (rc == 0) {
				std::cout << "Connection closed" << std::endl;
				// Remove closed client from fds array and clientsFds map
				_clients.erase(_fds[i].fd);
				close(_fds[i].fd);
				_fds[i].fd = -1;
			}
			else {
				buffer[rc] = '\0';
				std::string message(buffer);
				
				// if the client is not registered yet, authenticate the client
				try {
					authentication(message, _clients.find(_fds[i].fd));
				} catch (std::logic_error &e) {
					std::cerr << e.what() << std::endl;
					continue;
					// send(_fds[i].fd, e.what(), strlen(e.what()), 0);
				}

				// if (message.substr(0,5) == "PASS ")
				// {
				// 	std::string pass = message.substr(5, message.size() - 6);
				// 	// std::cout << "PASS :"<< pass <<":DONE"<< std::endl;
				// 	if (pass == _password)
				// 	{
				// 		std::cout << "password correct" << std::endl;
				// 		send(_fds[i].fd,"password correct\n", 17, 0);
				// 	}
				// 	else
				// 	{
				// 		std::cout << "password incorrect" << std::endl;
				// 		send(_fds[i].fd,"password incorrect\n", 20, 0);
				// 	}
				// }
				// else if (message.substr(0,5) == "NICK ") // NICK nicknome
				// {
				// 	std::string pass = message.substr(5, message.size() - 6);

				// }
				// else if (message.substr(0,5) == "USER ") //USER logino 0 * realfr
				// {
				// 	std::string pass = message.substr(5, message.size() - 6);

				// }
				// else
				// {
				// 	std::cout << "command not available" << std::endl;
				// 	send(_fds[i].fd,"command not found\n", 19, 0);
				// }

				// here we can do whatever we want with the message
				//........
				// std::cout << message ;
			}
		}
	}
}

int Server::createServer() 
{
	int    current_size;
	int rc;
	_nfds = 1;

	// Start listening for incoming connections
	std::cout << "server is running" << std::endl;
	while (true) {
	    // Wait for events on monitored file descriptors
	    rc = poll(_fds.data(), _nfds, 0);

	    // If poll failed or timeout occurred, continue to the next iteration
	    if (rc <= 0)
	        continue;
	    // Check for incoming connection on the server socket
		handlIncomeConnections();	    

	    // Iterate through fds array to check for messages from clients
	    handleIncomeData();
	
	    // Compact the fds array to remove closed client sockets
	    current_size = 0;
	    for (size_t i = 0; i < _nfds; i++) {
	        if (_fds[i].fd >= 0) {
	            _fds[current_size] = _fds[i];
	            current_size++;
	        }
	    }
	    _nfds = current_size;
	}
}


Server::~Server() {
    return;
}
