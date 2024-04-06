#include "../../Inc/ft_irc.hpp"
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

// Server::Server() {}

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
void Server::handlIncomeConnections() 
{
	if (_fds[0].revents == POLLIN)
	{
		struct sockaddr_in client_adrs;
		socklen_t sock_len = sizeof(client_adrs);
		memset(&client_adrs, 0, sock_len);

		int newSck = accept(_listen_sd, (struct sockaddr *)&client_adrs, &sock_len);
	    if (newSck < 0)
	        perror("  accept() failed");
		
	    else {
			// Resize the fds array if it's full
			if (_nfds >= _fds.size())
				_fds.resize(_fds.size() * 2);
	        // Add the new client socket to the fds array and clientsFds map
	        std::cout << "New incoming connection - " << newSck << std::endl;
			// std::cout << client_adrs.sin_addr.s_addr << std::endl;
	        fcntl(newSck, F_SETFL, O_NONBLOCK);
			_fds[_nfds].fd = newSck;
	        _fds[_nfds].events = POLLIN;
			_fds[_nfds].revents = 0;
			// Add the new client to the clients map
	        _clients.insert(std::pair<int, Client>(newSck, Client(newSck, client_adrs)));
	        _nfds++;
	    }
	}
}

	// cant start with :
		// Dollar ($, 0x24)
		// Colon (:, 0x3A)
		// Any character listed as a channel type (#, &)
		// Any character listed as a channel membership prefix (@, ~, &, %, +)
void Server::command_list(std::string &message, Client &cling)
{
	std::string command;
	// _clients[fd];

	if (cling.getRegistered() == false)
	{
			//authenticate the client
		if (cling.getValidPass() == false && message.substr(0 ,5) == "PASS ")
		{
			if (message.substr(5, message.length() - 6) == _password)
			{
				cling.setValidPass(true);
				std::cout << "success pass" << std::endl;
			}
			else
			{
				// std::ostringstream oss;
				// oss << cling._addr.sin_addr.s_addr;
				// std::string errorMsg =;
				std::cout << cling._addr.sin_addr.s_addr << std::endl;
				// out << ":irc.1337.ma 464 " <<  cling._addr.sin_addr  << " :Password incorrect";
				//
				// send(cling.getsocket(), out , out.size(), 0);
			}
		}
		else if (cling.getValidPass() == true && message.substr(0 ,5) == "NICK ") // this should be uniq to only one user
		{
			std::map<int, Client>::const_iterator it;

			for (it = _clients.begin() ; it != _clients.end(); ++it)
			{
				if (it->second.getNickname() == message.substr(5, message.length() - 6))
				{
					std::cout << "Nickname already in use" << std::endl; // works but when connection is close the client class still lives on
        			return;
				}
			}
			if (cling.setNickname(message.substr(5, message.length() - 6)) == false)
				std::cout<<"wrong nickname"<<std::endl;
		}
		else if (cling.getValidPass() == true && message.substr(0 ,5) == "USER ")
		{
			cling.setUsername(message.substr(5, message.length() - 6));
			cling.setRealname(message.substr(5, message.length() - 6)); // pase iside and return bool for parse output
		}
		else
			std::cout << "try registring first using these commands PASS nick user" << std::endl;
	}
	else
	{
		send(cling.getsocket(),"mar7ba\n",8,0);
	}
}

// Handle incoming data from clients :
void Server::handleIncomeData() {
	char buffer[1024];
	int rc;
	for (size_t i = 1; i < _nfds; i++) 
	{
		if (_fds[i].revents & POLLIN) {
			// setClientStatus(_clients.find(_fds[i].fd)->second);
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

				command_list(message, _clients.find(_fds[i].fd)->second);
				_clients.find(_fds[i].fd)->second.refstatus();
				// try {
				// } catch (std::logic_error &e) {
				// 	std::cerr << e.what() << std::endl;
				// 	continue;
				// }

				// here we can do whatever we want with the message
				//........
				// std::cout << "FULL CONMNAD ="<< message;
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


Server::~Server(void) {
	close(_listen_sd);
}
