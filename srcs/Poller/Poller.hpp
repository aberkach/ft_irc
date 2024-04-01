
#ifndef POLLER_HPP__
#define POLLER_HPP__

#include "IMonitor.hpp"
#include "../../Inc/define.hpp"

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

#endif