/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 05:13:11 by ZakariaElbo       #+#    #+#             */
/*   Updated: 2024/03/30 17:22:20 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <set>
#include <vector>
#include <sys/poll.h>
#include "../IMonitor/IMonitor.hpp"

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
