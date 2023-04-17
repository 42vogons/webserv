/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:38:55 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/16 22:30:15 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) : _serverName("default")
{
	return ;
}

Server::Server(std::string name) : _serverName(name)
{
	return ;
}

Server::Server(const Server& obj)
{
	*this = obj;
	return ;
}

Server::~Server(void)
{
	return ;
}

Server& Server::operator=(const Server& obj)
{
	if (this != &obj)
	{
		this->_serverName = obj._serverName;
		this->_errorPages = obj._errorPages;
		this->_ports = obj._ports;
		this->_clientMaxBodySize = obj._clientMaxBodySize;
		this->_locationServer = obj._locationServer;
		this->_lastLocation = obj._lastLocation;
	}
	return (*this);
}

void	Server::readLine(std::string line)
{
	std::string key, valueString;
	int valueInt;
	std::istringstream iss(line);
	iss >> key;
	if (key.find("listen") == 0) 
	{
		while (iss >> valueInt)
			this->setPorts(valueInt);
	}
	if (key.find("server_name") == 0)
	{
		iss >> valueString;
		this->setServeName(valueString);
		addHostServerName(valueString,"127.0.0.1");
	}
	if (key.find("client_max_body_size") == 0)
	{
		iss >> valueInt;
		this->setClientMaxBodySize(valueInt);
	}
	if (key.find("error_page") == 0)
	{
		iss >> valueInt >> valueString;
		this->setErrorPages(valueInt, valueString);
	}
	if (key.find("location") == 0)
	{
		iss >> valueString;
		
		LocationServer locationServer;
		this->_lastLocation = valueString;
		this->setLocationServer(valueString, locationServer);
	}
}

std::string	Server::getLastLocation(void)
{
	return _lastLocation;
}



void	Server::setServeName(std::string serverName)
{
	this->_serverName = serverName;
}

void	Server::setErrorPages(int code, std::string page)
{
	this->_errorPages[code] =  page;
}

void	Server::setPorts(int port)
{
	if (_ports.find(port) == _ports.end())
		_ports.insert(port);
}

void	Server::setClientMaxBodySize(int clientMaxBodySize)
{
	this->_clientMaxBodySize = clientMaxBodySize;
}

void	Server::setLocationServer(std::string name, LocationServer locationServer)
{
	this->_locationServer[name] =  locationServer;
}

std::string	Server::getServerName(void)
{
	return (_serverName);
}

std::string	Server::getErrorPages(int code)
{
	//todo: fazer verificação se não é nulo, se for estourar erro
	return _errorPages.find(code)->second;
}

std::set<int>	Server::getPorts(void) const
{
	return (_ports);
}

int	Server::getClientMaxBodySize(void)
{
	return _clientMaxBodySize;
}

LocationServer	Server::getLocationServer(std::string name)
{
	return _locationServer.find(name)->second;
}

void	Server::addHostServerName(std::string serverName, std::string ipAddress){
	std::ifstream fileHost("/etc/hosts");
	std::string line;
	std::string context;
	int add = 0;
	
	while (getline(fileHost, line)){
		if (line.find(serverName) != std::string::npos)
			continue;
		if (line.find("127.0.0.1") == std::string::npos && add == 0){
			add = 1;
			context += ipAddress + "\t" + serverName + "\n";
		}
		context += line + "\n";
	}
	fileHost.close();
	std::ofstream newFile("/etc/hosts");
	newFile << context.c_str();
	newFile.close();
}

std::ostream&	operator<<(std::ostream& o, const Server& i)
{
	(void)i;
	o << "something";
	return o;
}
