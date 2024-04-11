#include "server.hpp"

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
	// Set socket option to allow the server to reuse the address and port
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
		// std::pair<int, Client> client(0, Client(0));
		Client client(0);
		int sock_len = sizeof(_addr);

		// Accept incoming connection
		int newSck = accept(_listen_sd, (struct sockaddr *)&client._addr, (socklen_t*)&sock_len);
	    if (newSck < 0)
	        perror("  accept() failed");
	    else {
			// Resize the fds array if it's full
			if (_nfds >= _fds.size())
				_fds.resize(_fds.size() * 2);
	        // Add the new client socket to the fds array and clientsFds map
	        std::cout << "New incoming connection - " << newSck << std::endl;
	        fcntl(newSck, F_SETFL, O_NONBLOCK);
			// add the new client to the fds (of poll)
			_fds[_nfds].fd = newSck;
	        _fds[_nfds].events = POLLIN;
			_fds[_nfds].revents = 0;
			// Add the new client to the clients map
	        _clients.insert(std::pair<int, Client>(newSck, Client(newSck)));
	        _nfds++;
	    }
	}
}

// split the string by the delim and (/n, /r)
std::vector<std::string> split(std::string str, char delim)
{
    // split the string by the delim
	std::vector<std::string> tokens;
	std::string token;

	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == delim || str[i] == '\n' || str[i] == '\r')
		{
			tokens.push_back(token);
			token.clear();
		}
		else
			token += str[i];
	}
	tokens.push_back(token);
	return tokens;
}

int Server::createChannel(std::string &chnName, std::vector<std::string> &keys, Client &client) {
	Channel newChannel(chnName);
	server_channels.insert(std::pair<std::string, Channel>(chnName, newChannel));
	// check if the channel has a key
	if (keys.size() > 0)
	{
		// check if the key is valid
		if (keys[0].find_first_of(" ,\a\b\f\t\v$:&+~%") != std::string::npos)
		{
			keys.erase(keys.begin());
			server_channels.erase(chnName);
			std::cout << "Error: invalid channel key" << std::endl;
			// here we can send an error message to the client
			// ....
			return 1;
		}
		// set the key for the channel
		server_channels.find(chnName)->second.setKey(keys[0]);
		keys.erase(keys.begin());;
	}
	else
		server_channels.find(chnName)->second.setKey("");
	// add the client to the channel
	server_channels.find(chnName)->second.addUser(client);
	return 0;
}

void Server::processTheJoinArgs(std::vector<std::string> &channels , std::vector<std::string> &keys, Client &client)
{
	for (size_t i = 0; i < channels.size(); i++)
    {
        std::string chnName = channels[i];
		// check if the channel name is valid
		if ((chnName[0] != '#') || (chnName.find_first_of(" ,\a\b\f\t\v$:&+~%") != std::string::npos))
		{
			std::cout << "Error: invalid channel name" << std::endl;
			// here we can send an error message to the client
			// ....
			if (keys.size() > 0)
				keys.erase(keys.begin());
			continue;
		}
		else
        {
			std::map<std::string, Channel>::iterator chnIt = server_channels.find(chnName);
			// if the channel doesn't exist, create a new one
			if (chnIt == server_channels.end())
			{
				if (createChannel(chnName, keys, client))
					continue;
				// here we can send a message to the client to inform him that the channel is created
				// ....
			}
			// if the channel already exist, check if the client is already in the channel
			else 
			{
				// if the client is already in the channel, do nothing
				if (chnIt->second.isClientExist(client.getsocket()) == true)
				{
					std::cout << "Error: you are already in the channel" << std::endl;
					// here we can send an error message to the client
					// ....
					continue;
				}
				// if the client is not in the channel, add the client to the channel
				else
				{
					// check if the channel has a key
					if (keys.size() > 0)
					{
						// if the key is correct
						if (keys[0] == chnIt->second.getKey())
						{
							// add the client to the channel
							chnIt->second.addUser(client);
							keys.erase(keys.begin());
							// here we can send a message to the client to inform him that he joined the channel
							// ....
						}
						// if the key is incorrect
						else
						{
							std::cout << "Error: incorrect channel key" << std::endl;
							// here we can send an error message to the client
							// ....
							keys.erase(keys.begin());
							continue;
						}
					}
					// if the channel doesn't have a key, add the client to the channel
					else
					{
						// if the channel has no key, add the client to the channel
						if (chnIt->second.getKey() == "")
						{
							chnIt->second.addUser(client);
							// here we can send a message to the client to inform him that he joined the channel
							// ....
						}
						else
						{
							std::cout << "Error: channel has a key" << std::endl;
							// here we can send an error message to the client
							// ....
							continue;
						}
					}
				}
			}

        }
    }
}

void Server::command_join(std::vector<std::string> &splitedMsg, Client &client) {

	std::vector<std::string> channels;
    std::vector<std::string> keys;
	// check if the message contains a channel name
    if (splitedMsg.size() > 0)
    {
		// split the channels and keys by comma
        channels = split(splitedMsg[0], ',');
		keys = split(splitedMsg[1], ',');
		// process the channels and keys
		processTheJoinArgs(channels, keys, client);
    }
    else
	{
		std::cout << "Error: empty args" << std::endl;
		// here we can send an error message to the client
		// ....
	}
}


void Server::parseCommand(std::string &message, Client &cling)
{
	std::vector<std::string> splitedMsg = split(message, ' ');
	std::string command;
	if (splitedMsg.begin() != splitedMsg.end())
	{
		command = *splitedMsg.begin();
		std::cout << "command = " << command << std::endl;
		if (command == "JOIN" || command == "join"){
			splitedMsg.erase(splitedMsg.begin());
			command_join(splitedMsg, cling);
		}
		else
			std::cout << "Error: invalid command" << std::endl;
	}
	else
		std::cout << "Error: empty command" << std::endl;

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
				// here we need to check if the message is finished or not
				// ........

				// if the message is not finished, we need to wait for the next part of the message
				std::string message(buffer);
				parseCommand(message, _clients.find(_fds[i].fd)->second);
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
