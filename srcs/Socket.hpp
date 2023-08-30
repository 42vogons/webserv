/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/08/29 18:20:27 by cpereira         ###   ########.fr       */
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <fstream>
#include <string>

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
		void		process(std::string& response);
		void		readPage(std::string filename, int code, std::string status, std::string& content);
		void		readImage(std::string filename, int code, std::string status, std::string& content);
		void		createPage(std::string newPage, int code, std::string status, std::string& content);
		void		autoIndex(std::string path);
	
		void		executeGet(std::string& response);
		void		executePost(std::string& response);
		void		executeDelete(std::string& response);
		
		void		saveFile(void);

		//std::string	receiveInformation(void);
		std::string	findField(std::string src, std::string field);
		

		int			getServerFd(void) const;
		HandleRequest	getHandleRequest(void);


	private:
		int					_port;
		int					_server_fd;
		int					_client_fd;
		unsigned int		_addrlen;
		struct sockaddr_in	_address;
		Server				_server;
		HandleRequest		_HandleRequest;
		//std::map<std::string, std::string> _variables;

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
