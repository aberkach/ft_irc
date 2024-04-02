#include "server.hpp"

void Err(std::string msg, int exitFalg)
{
	std::cerr << msg << std::endl;
	if (exitFalg)
		exit(1);
}

Server::Server() : _port(0), _password(""), _listen_sd(-1)
{
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
	memset(_fds, 0 , sizeof(_fds));
	_fds[0].fd = _listen_sd;
	_fds[0].events = POLLIN;
	_nfds = 1;
}


void Server::setPort(unsigned int port) {
    if (port > 1023 && port <= 65535)
        _port = port;
    else
        throw std::logic_error("Port number must be between 1024 and 65535");
}

void Server::setPassword(char *password) {
    if (strlen(password) >= 8)
        _password = password;
    else
        throw std::logic_error("Password must be at least 8 characters long");
}

// void Server::updateFileDescrior(int *nfds)
// {ı
// 	if (_fds)
// 		delete[] _fds;
// 	_fds = new struct pollfd((*nfds) + 1);
// }

int Server::createServer() 
{	
	char   buffer[1024];
	int    current_size = 0;
	int rc;
	std::map<int, int> clientsFds; // Map to store client file descriptors
	
	// Start listening for incoming connections
	std::cout << "server is running" << std::endl;
	while (true) {
	    // Wait for events on monitored file descriptors
	    rc = poll(_fds, _nfds, 0);
	
	    // If poll failed or timeout occurred, continue to the next iteration
	    if (rc <= 0)
	        continue;
		
	    current_size = _nfds;
		int sock_len = sizeof(_addr);
	
	    // Check for incoming connection on the server socket
	    if (_fds[0].revents == POLLIN)
		{
			int newSck = accept(_listen_sd, (struct sockaddr *)&_addr, (socklen_t*)&sock_len);
	        if (newSck < 0)
	            perror("  accept() failed");
	        else {
	            // Add the new client socket to the fds array and clientsFds map
	            std::cout << "New incoming connection - " << newSck << std::endl;
	            _fds[_nfds].fd = newSck;
	            _fds[_nfds].events = POLLIN;
	            fcntl(newSck, F_SETFL, O_NONBLOCK);
	            clientsFds.insert(std::pair<int, int>(newSck, newSck));
	            _nfds++;
	        }
	    }
	
	    // Iterate through fds array to check for messages from clients
	    for (int i = 1; i < current_size; i++)
		{
	        if (_fds[i].revents & POLLIN) {
	            // Receive message from client
	            rc = recv(_fds[i].fd, buffer, (sizeof(buffer) - 1), 0);
	            if (rc < 0)
	                perror("  recv() failed");
	            else if (rc == 0) {
	                std::cout << "Connection closed" << i << std::endl;
	                // Remove closed client from fds array and clientsFds map
	                clientsFds.erase(_fds[i].fd);
	                close(_fds[i].fd);
	                _fds[i].fd = -1;
	            }
	            else {
	                buffer[rc] = '\0';
					std::string message(buffer);
					// here we can do whatever we want with the message
					//........
	                std::cout << message << std::endl;
	            }
	        }
	    }
	
	    // Compact the fds array to remove closed client sockets
	    current_size = 0;
	    for (int i = 0; i < _nfds; i++) {
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
