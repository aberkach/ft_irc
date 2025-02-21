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

### Commands

- **PASS**: Authenticate connection with server password. Must be sent before NICK and USER during initial registration.
- **NICK**: Set or change a client's nickname. The nickname must be unique across the server and follows specific format rules.
- **USER**: Set username and real name during initial registration. Format: `USER username 0 * :Real Name`.
- **PRIVMSG**: Send messages to channels or other users. Can target either channels (`#channel`) or specific users.
- **PING/PONG**: Connection keepalive mechanism. Server sends PING, client must respond with PONG to prove connection is alive.
- **NAMES**: List all visible users in specified channels. Shows user status (operators marked with @) in channel.
- **LIST**: Display list of all visible channels with their topics and user counts.
- **PART**: Leave a channel with optional part message. Must specify channel name user wants to leave.
- **QUIT**: Disconnect from server with optional quit message. Closes connection and informs all relevant channels.
- **JOIN**: Enter a channel or create it if it doesn't exist. Can join multiple channels with comma-separated list.
- **KICK**: Remove users from channels. Only channel operators can use this command.
- **INVITE**: Send channel invitations to users. Required for joining invite-only channels.
- **TOPIC**: Manage channel topics. Can be restricted to operators only.
- **MODE**: Configure channel settings:
  - `i` - Toggle invite-only status
  - `t` - Restrict topic changes to operators
  - `k` - Set/remove channel password
  - `o` - Assign/remove operator privileges
  - `l` - Set/remove user limits

### Advanced Features

- **File Transfer Support**: Direct client-to-client file sharing capabilities
- **IRC Bot**: Automated assistant for channel management and user interaction

## Prerequisites

- C++ Compiler with C++98 support
- Make build system
- POSIX-compliant operating system

## Building the Server

```bash
# Clone the repository
git clone https://github.com/ElmehdiBennix/ft_irc_42.git
cd ft_irc_42

# Build the server
make
# Build bot client
make bonus
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

- TCP/IP (IPv4/IPv6)
- Single-threaded event-driven design
- Non-blocking I/O operations using poll()
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

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

## Notes

- The server is designed for educational purposes and may not include all features found in production IRC servers
- Performance optimizations focus on handling multiple clients efficiently while maintaining stability
- Security features are implemented with educational use in mind
