/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMonitor.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 05:08:32 by ZakariaElbo       #+#    #+#             */
/*   Updated: 2024/03/30 05:26:01 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define EVENT_READ 1 << 0
#define EVENT_WRITE 1 << 1
#define EVENT_RW (EVENT_WRITE | EVENT_READ)

class	IMonitor
{
	public:
		virtual ~IMonitor() {};
		virtual	void	push(int fd, short events) = 0;
		virtual void	pop(int fd) = 0;
		virtual bool	is_readable(int fd) = 0;
		virtual bool	is_writeable(int fd) = 0;
		virtual bool	wait(int timeout = 1) = 0;
};

