/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/21 00:17:35 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Server.hpp"
#include "Receiver.hpp"
#include <iostream>
#include <unistd.h> //read write and close
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring> //strlen memset
#include <dirent.h> // para ler diretorios

class Socket
{
	public:
		Socket(void);
		Socket(int port, Server server);
		Socket(const Socket& obj);
		virtual ~Socket(void);

		Socket&	operator=(const Socket& obj);

		void		createSocketTCP(void);
		void		configSocketAddress(void);
		void		bindSocketToAddress(void);
		void		waitConnection(void);
		void		acceptConnection(void);
		void		setReceiver(Receiver receiver);
		void		checkHost(std::string& response);
		void		readPage(std::string filename, int code, std::string status, std::string& content);
		void		autoIndex(std::string path);

		int			getServerFd(void) const;
		Receiver	getReceiver(void);

	private:
		int					_port;
		int					_server_fd;
		int					_addrlen;
		struct sockaddr_in	_address;
		Server				_server;
		Receiver			_receiver;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const Socket& i);

#endif
