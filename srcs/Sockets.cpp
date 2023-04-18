/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:36:11 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/17 22:39:18 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sockets.hpp"

Sockets::Sockets(void)
{
	this->_servers.readFile("./conf/conf_sample");
	this->_serversMap = this->_servers.getServers();
	return ;
}

Sockets::Sockets(const Sockets& obj)
{
	*this = obj;
	return ;
}

Sockets::~Sockets(void)
{
	return ;
}

Sockets& Sockets::operator=(const Sockets& obj)
{
	if (this != &obj)
	{
		this->_vecSocket = obj._vecSocket;
	}
	return (*this);
}

void	Sockets::createVecSocket(void)
{
	std::map<std::string, Server>::iterator itMap = this->_serversMap.begin();
	std::map<std::string, Server>::iterator iteMap = this->_serversMap.end();
	++itMap;
	--itMap;
	std::set<int>	ports;
	while (itMap != iteMap)
	{
		ports.clear();
		ports = itMap->second.getPorts();
		std::set<int>::iterator itSet = ports.begin();
		std::set<int>::iterator iteSet = ports.end();
		++itSet;
		--itSet;
		while (itSet != iteSet)
		{
			Socket socket(*itSet, &itMap->second);
			this->_vecSocket.push_back(socket);
			itSet++;
		}
		++itMap;
	}
}

void	Sockets::handleSocketConnections(void)
{
	this->createVecSocket();
	while (true)
	{
		fd_set readfds;
		FD_ZERO(&readfds);
		for (std::vector<int>::size_type i = 0; i < this->_vecSocket.size(); i++)
			FD_SET(this->_vecSocket[i].getServerFd(), &readfds);
		int max_fd = this->_vecSocket[0].getServerFd();
		for (std::vector<int>::size_type i = 1; i < this->_vecSocket.size(); ++i)
		{
			if (this->_vecSocket[i].getServerFd() > max_fd)
				max_fd = this->_vecSocket[i].getServerFd();
		}
		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if(activity < 0)
		{
			std::cerr << "Error waiting for events" << std::endl;
			continue;
		}
		for (std::vector<int>::size_type i = 0; i < this->_vecSocket.size(); i++)
		{
			if(FD_ISSET(this->_vecSocket[i].getServerFd(), &readfds))
				this->_vecSocket[i].acceptConnection();
		}
	}
}

std::ostream&	operator<<(std::ostream& o, const Sockets& i)
{
	(void)i;
	o << "something";
	return o;
}
