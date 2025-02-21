# Modern IRC Server Implementation

A robust Internet Relay Chat (IRC) server implementation built in C++98, following modern IRC standards while maintaining compatibility with classic IRC clients. This project provides a fully-featured IRC server that supports multiple simultaneous client connections, channel operations, and user management.

## Features

### Core Functionality
- **Multi-client Support**: Handles multiple concurrent client connections using non-blocking I/O operations
- **TCP/IP Protocol**: Supports both IPv4 and IPv6 connections
- **Authentication System**: Secure password-based connection authentication
- **Real-time Communication**: Efficient message broadcasting and private messaging capabilities

### Channel Operations
- **Channel Management**: Create, join, and leave channels
- **Private Messaging**: Direct message support between users
- **Message Broadcasting**: Efficient message distribution to channel members

### Operator Commands
- **KICK**: Remove users from channels
- **INVITE**: Send channel invitations to users
- **TOPIC**: Manage channel topics
- **MODE**: Configure channel settings:
  - `i` - Toggle invite-only status
  - `t` - Restrict topic changes to operators
  - `k` - Set/remove channel password
  - `o` - Assign/remove operator privileges
  - `l` - Set/remove user limits

### Advanced Features
- **File Transfer Support**: Direct client-to-client file sharing capabilities
- **IRC Bot**: Automated assistant for channel management and user interaction

## Technical Specifications

- Language: C++98
- Build System: Make
- Network Protocol: TCP/IP (IPv4/IPv6)
- I/O Handling: Non-blocking operations using poll() (or equivalent)
- Error Handling: Comprehensive error checking and graceful failure recovery

## Prerequisites

- C++ Compiler with C++98 support
- Make build system
- POSIX-compliant operating system

## Building the Server

```bash
# Clone the repository
git clone https://github.com/yourusername/irc-server.git
cd irc-server

# Build the server
make

# Clean build files
make clean

# Rebuild the project
make re
```

## Usage

Start the server by providing a port number and connection password:

```bash
./ircserv <port> <password>
```

Example:
```bash
./ircserv 6667 myserverpassword
```

### Connecting with a Client

You can connect to the server using any standard IRC client. Here are some recommended clients:

- HexChat
- irssi
- WeeChat
- mIRC (Windows)

Connection settings:
- Server: your-server-address
- Port: [specified port]
- Password: [specified password]

## Implementation Details

### Network Architecture
- Single-threaded event-driven design
- Non-blocking I/O operations
- Efficient memory management
- Robust error handling

### Standards Compliance
- Implements core IRC protocol features
- Compatible with modern IRC clients
- Follows RFC 1459 and RFC 2812 specifications

### Security Features
- Password-protected server access
- Channel-level security controls
- Operator privilege management
- Input validation and sanitization

## Development Guidelines

- Code follows C++98 standard
- No external libraries used (except standard C++ libraries)
- Comprehensive error handling
- Non-blocking I/O operations only
- Clean and maintainable code structure

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

## Notes

- The server is designed for educational purposes and may not include all features found in production IRC servers
- Performance optimizations focus on handling multiple clients efficiently while maintaining stability
- Security features are implemented with educational use in mind
