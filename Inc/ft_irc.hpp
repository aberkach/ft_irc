#ifndef FT_IRC_HPP__
#define FT_IRC_HPP__

#include "colors.hpp"
#include "define.hpp"


class health 
{
    private:
        static uint16_t valid_port(char *port_arg);
        static bool valid_passord(char *password);
    public :
        static uint16_t arg_checker(int ac, char **av);
};

class	IMonitor
{
	public:
		virtual	void	push(int fd, short events) = 0;
		virtual void	pop(int fd) = 0;
		virtual bool	is_readable(int fd) = 0;
		virtual bool	is_writeable(int fd) = 0;
		virtual bool	wait(int timeout = 1) = 0;
		virtual         ~IMonitor() {};
};


class	Poller : public IMonitor
{
	private:
		std::vector<struct pollfd>	_pfds;
		std::set<int>		_readable_fds;
    	std::set<int>		_writeable_fds;
	public:
		Poller( void );
		virtual ~Poller( void );
		virtual void	push(int fd, short events);
		virtual void 	pop(int fd);
		virtual bool	is_readable(int fd);
		virtual bool	is_writeable(int fd);
		virtual bool	wait(int timeout = 1);
};

class Server 
{
  private:
    unsigned int            _port;
    std::string             _password;
    int                     _listen_sd;
    struct sockaddr_in      _addr;
	struct pollfd         	_fds[5000];
    int                     _nfds;
    // std::map<int, Clients>  _clients;

  public:
        Server();
        ~Server();
        
        void setPort(unsigned int port);
        void setPassword(char *password);
        
        unsigned int getPort() const { return _port; }
        std::string getPassword() const { return _password; }

        int createServer();
        // void updateFileDescrior(int *ng);
};

class client {

};

#endif // SERVER_HPP