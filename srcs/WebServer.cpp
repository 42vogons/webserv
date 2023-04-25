/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:36:11 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/24 22:50:06 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer(void)
{
	this->_servers.readFile("./conf/conf_sample");
	this->_serversMap = this->_servers.getServersMap();
	return ;
}

WebServer::WebServer(const WebServer& obj)
{
	*this = obj;
	return ;
}

WebServer::~WebServer(void)
{
	for (size_t i = 0; i < this->_vecSocket.size(); i++)
	{
		if (this->_vecSocket[i])
		{
			this->_vecSocket[i]->closeServerFd();
			this->_vecSocket[i]->closeClientFd();
			delete this->_vecSocket[i];
		}
	}
	return ;
}

WebServer& WebServer::operator=(const WebServer& obj)
{
	if (this != &obj)
	{
		this->_vecSocket = obj._vecSocket;
		this->_servers = obj._servers;
		this->_serversMap = obj._serversMap;
		this->_poll = obj._poll;
	}
	return (*this);
}

void	WebServer::createVecSocket(void)
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
			Socket *socket = new Socket(*itSet, itMap->second);
			this->_vecSocket.push_back(socket);
			itSet++;
		}
		++itMap;
	}
}

void	WebServer::handleSocketConnections(void)
{
	this->createVecSocket();
	this->_poll.start(this->_vecSocket);
	while (true)
	{
		this->_poll.exec();
		/*fd_set readfds;
		FD_ZERO(&readfds);
		for (std::vector<int>::size_type i = 0; i < this->_vecSocket.size(); i++)
			FD_SET(this->_vecSocket[i]->getServerFd(), &readfds);
		int max_fd = this->_vecSocket[0]->getServerFd();
		for (std::vector<int>::size_type i = 1; i < this->_vecSocket.size(); ++i)
		{
			if (this->_vecSocket[i]->getServerFd() > max_fd)
				max_fd = this->_vecSocket[i]->getServerFd();
		}
		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if(activity < 0)
		{
			std::cerr << "Error waiting for events" << std::endl;
			continue;
		}*/
		for (std::vector<int>::size_type i = 0; i < this->_vecSocket.size(); i++)
		{
			this->_checkEvent(this->_poll, i);
		}
	}
}

void	WebServer::_checkEvent(Poll &poll, size_t index)
{
	if (this->_checkEventMask(poll.getReturnEvents(index)))
		this->_connect(poll.getSocket(index));
}

bool	WebServer::_checkEventMask(short revents)
{
	if ((revents & POLLIN) == POLLIN)
		return (true);
	if ((revents & POLLPRI) == POLLPRI)
		return (true);
	if ((revents & POLLOUT) == POLLOUT)
		return (true);
	return (false);
}

void	WebServer::_connect(Socket *socket)
{
	try
	{
		socket->acceptConnection();
	}
	catch (std::exception& e)
	{
		return ;
	}
}

std::ostream&	operator<<(std::ostream& o, const WebServer& i)
{
	(void)i;
	o << "something";
	return o;
}
