/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/17 22:49:02 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Server.hpp"
#include <iostream>
#include <unistd.h> //read write and close
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring> //strlen memset

class Socket
{
	public:
		Socket(void);
		Socket(int port, Server server);
		Socket(const Socket& obj);
		virtual ~Socket(void);

		Socket&	operator=(const Socket& obj);
		void	createSocketTCP(void);
		void	configSocketAddress(void);
		void	bindSocketToAddress(void);
		void	waitConnection(void);
		void	acceptConnection(void);

		int		getServerFd(void) const;

	private:
		int					_port;
		int					_server_fd;
		int					_addrlen;
		struct sockaddr_in	_address;
		Server				_server;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const Socket& i);

#endif
