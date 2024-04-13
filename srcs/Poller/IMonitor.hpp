
#ifndef IMONITOR_HPP__
#define IMONITOR_HPP__

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

#endif
