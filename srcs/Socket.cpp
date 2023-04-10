/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:26 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/09 21:00:35 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void) : _port(80)
{
	createSocketTCP();
	configSocketAddress();
	bindSocketToAddressAndWait();
	waitConnection();
	return ;
}

Socket::Socket(int port) : _port(port)
{
	createSocketTCP();
	configSocketAddress();
	bindSocketToAddressAndWait();
	waitConnection();
	return ;
}

Socket::Socket(const Socket& obj)
{
	std::cout
		<< "Socket copy constructor called"
		<< std::endl;
	*this = obj;
	return ;
}

Socket::~Socket(void)
{
	std::cout
		<< "Socket destructor called"
		<< std::endl;
	return ;
}

Socket& Socket::operator=(const Socket& obj)
{
	if (this != &obj)
	{
		this->_port = obj._port;
		this->_server_fd = obj._server_fd;
		this->_addrlen = obj._addrlen;
	}
	return (*this);
}

void	Socket::createSocketTCP(void)
{
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
}

void	Socket::configSocketAddress(void)
{
	this->_addrlen = sizeof(this->_address);
	memset(&this->_address, 0, sizeof(this->_address));
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);
}

void	Socket::bindSocketToAddressAndWait(void)
{
	bind(
		this->_server_fd,
		(struct sockaddr *)&this->_address,
		sizeof(this->_address)
	);
}

void	Socket::waitConnection(void)
{
	listen(this->_server_fd, 3);
}

int	Socket::getServerFd(void)
{
	return (this->_server_fd);
}

void	Socket::acceptConnection(void)
{
	int client_fd = accept(this->_server_fd, (struct sockaddr *)&this->_address, (socklen_t*)&this->_addrlen);
	std::cout << "New connection on " << this->_server_fd << std::endl;
	char buffer[1024] = {0};
	read(client_fd, buffer, 1024);
	std::cout << buffer << std::endl;
	const char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	write(client_fd, response, strlen(response));
	std::cout << "Message sent to client" << std::endl;
	close(client_fd);
}

std::ostream&	operator<<(std::ostream& o, const Socket& i)
{
	(void)i;
	o << "something";
	return o;
}
