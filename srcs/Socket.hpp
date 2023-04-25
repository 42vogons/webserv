/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/24 22:58:43 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Server.hpp"
#include "HandleRequest.hpp"
#include <iostream>
#include <unistd.h> //read write and close
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring> //strlen memset
#include <dirent.h> // para ler diretorios
#include <fcntl.h>

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
		void		closeServerFd(void);
		void		closeClientFd(void);

		void		setHandleRequest(HandleRequest HandleRequest);
		void		checkHost(std::string& response);
		void		readPage(std::string filename, int code, std::string status, std::string& content);
		void		autoIndex(std::string path);

		int			getServerFd(void) const;
		HandleRequest	getHandleRequest(void);

	private:
		int					_port;
		int					_server_fd;
		int					_client_fd;
		unsigned int		_addrlen;
		struct sockaddr_in	_address;
		Server				_server;
		HandleRequest			_HandleRequest;

	protected:
		class AcceptConnectionError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Error to accept new connection");
				}
		};
};

std::ostream&	operator<<(std::ostream& o, const Socket& i);

#endif
