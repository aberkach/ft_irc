/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ZakariaElbouzkri <elbouzkri9@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 05:20:40 by ZakariaElbo       #+#    #+#             */
/*   Updated: 2024/03/21 16:42:55 by ZakariaElbo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poller.hpp"

Poller::Poller( void )
{
}

Poller::~Poller( void )
{
}

void	Poller::push(int fd, short events)
{
	struct pollfd	pfd;
	short pollevents = 0;
	if (events & EVENT_READ)
		pollevents |= POLLIN;
	if (events & EVENT_WRITE)
		pollevents |= POLLOUT;
	pfd.events = pollevents;
	pfd.fd = fd;
	pfd.revents = 0;
	_pfds.push_back(pfd);
}


void	Poller::pop(int fd)
{
	std::vector<struct pollfd>::iterator	it = _pfds.begin();
	for (it = _pfds.begin(); it != _pfds.end(); it++)
	{
		if ((*it).fd == fd)
			break;
	}
	if (it != _pfds.end())
		_pfds.erase(it);
}

bool	Poller::is_readable(int fd)
{
	return _readable_fds.find(fd) != _readable_fds.end();
}

bool	Poller::is_writeable(int fd)
{
	return _writeable_fds.find(fd) != _writeable_fds.end();
}

bool	Poller::wait(int timeout)
{
	_readable_fds.clear();
	_writeable_fds.clear();
	int ret = poll(_pfds.data(), _pfds.size(), timeout);
	if (ret == -1)
		return false;
	for (std::size_t i = 0; i < _pfds.size(); i++) {
		if (_pfds[i].revents & POLLIN) {
			_readable_fds.insert(_pfds[i].fd);
		}
		if (_pfds[i].revents & POLLOUT) {
			_writeable_fds.insert(_pfds[i].fd);
		}
	}
	return ret > 0;
}
