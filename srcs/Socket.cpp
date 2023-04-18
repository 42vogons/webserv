/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:26 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/17 22:59:10 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void) : _port(80), _server(NULL)
{
	createSocketTCP();
	configSocketAddress();
	bindSocketToAddress();
	waitConnection();
	return ;
}

Socket::Socket(int port, Server server) : _port(port), _server(server)
{
	createSocketTCP();
	configSocketAddress();
	bindSocketToAddress();
	waitConnection();
	return ;
}

Socket::Socket(const Socket& obj)
{
	*this = obj;
	return ;
}

Socket::~Socket(void)
{
	return ;
}

Socket& Socket::operator=(const Socket& obj)
{
	if (this != &obj)
	{
		this->_port = obj._port;
		this->_server_fd = obj._server_fd;
		this->_addrlen = obj._addrlen;
		this->_address = obj._address;
		this->_server = obj._server;
	}
	return (*this);
}

void	Socket::createSocketTCP(void)
{
	//address domain AF_INET: socket will be created for the IPv4 protocol
	//type of socket SOCK_STREAM: connection-oriented socket
	//0 the default protocol for the specified socket type should be used
	struct timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(this->_server_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

void	Socket::configSocketAddress(void)
{
	//sets the address length
	this->_addrlen = sizeof(this->_address);
	//fills the _address structure with zeros
	memset(&this->_address, 0, sizeof(this->_address));
	//sets the address family to AF_INET (indicating that the address is IPv4)
	this->_address.sin_family = AF_INET;
	//IP address to INADDR_ANY (indicating that the socket can connect to any available IP address on the local machine)
	this->_address.sin_addr.s_addr = INADDR_ANY;
	//htons -> converts the value of a port from host byte order to network byte order
	this->_address.sin_port = htons(this->_port);
}

void	Socket::bindSocketToAddress(void)
{
	bind(
		this->_server_fd,
		(struct sockaddr *)&this->_address,
		sizeof(this->_address)
	);
}

void	Socket::waitConnection(void)
{
	//queue: size of the pending connection queue, which defines the maximum
	//number of connections that can be queued for the socket before the system
	//starts rejecting new connections.
	int queue = 100;
	listen(this->_server_fd, queue);
}

int	Socket::getServerFd(void) const
{
	return (this->_server_fd);
}

void	Socket::acceptConnection(void)
{
	int maxBodySize = this->_server.getClientMaxBodySize();
	int client_fd = accept(this->_server_fd, (struct sockaddr *)&this->_address, (socklen_t*)&this->_addrlen);
	std::cout << "\033[0;32m\n\n\nNew connection on " << this->_server_fd << "\033[0m" << std::endl;
	char buffer[4096] = {0};
	read(client_fd, buffer, 4096);
	std::cout << buffer << std::endl;
	const char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	write(client_fd, response, strlen(response));
	std::cout << "Message sent to client" << std::endl;
	close(client_fd);
}

std::ostream&	operator<<(std::ostream& o, const Socket& i)
{
	o << "Socket: " << i.getServerFd();
	return o;
}
