/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/09 20:40:07 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

class Socket
{
	public:
		Socket(void);
		Socket(int port);
		Socket(const Socket& obj);
		virtual ~Socket(void);

		Socket&	operator=(const Socket& obj);
		void	createSocketTCP(void);
		void	configSocketAddress(void);
		void	bindSocketToAddressAndWait(void);
		void	waitConnection(void);
		void	acceptConnection(void);

		int		getServerFd(void);

	private:
		int					_port;
		int					_server_fd;
		int					_addrlen;
		struct sockaddr_in	_address;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const Socket& i);

#endif
