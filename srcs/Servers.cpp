/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:34:23 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/16 23:31:12 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"

Servers::Servers(void)
{
	return ;
}

Servers::Servers(const Servers& obj)
{
	*this = obj;
	return ;
}

Servers::~Servers(void)
{
	return ;
}

Servers& Servers::operator=(const Servers& obj)
{
	if (this != &obj)
	{
		this->_servers = obj._servers;
	}
	return (*this);
}

void Servers::addServer(std::string serverName, Server server)
{
	this->_servers[serverName] = server;
}

void	Servers::setServer(std::string serverName, Server server)
{
	this->_servers[serverName] =  server;
}

int Servers::countTabs(std::string line)
{
	int countTab = 0;
	for (unsigned int i = 0; i < line.length(); i++)
	{
		if (line[i] == '\t')
			countTab++;
		else
			break;
	}
	return countTab;
}

Server Servers::getServer(std::string serverName)
{
	return _servers[serverName];
}

std::map<std::string, Server>	Servers::getServers(void)
{
	return (this->_servers);
}

void Servers::readFile(std::string fileName)
{
	std::ifstream myFile(fileName.c_str());
	std::string line;
	std::string key, name;
	std::string lastServer;
	LocationServer locationServer;
	int level;
	if (!myFile.is_open())
		std::cerr << "Error opening file" << std::endl;
	else
	{
		while (!myFile.eof() )
		{
			getline(myFile, line);
			level = countTabs(line);
			if (level == 0 && line.size() > 1)
			{
				std::istringstream iss(line);
				iss >> key;
				Server server(key);
				lastServer = key;
				addServer(key, server);
			}
			if (level == 1)
			{
				Server serverEdit = getServer(lastServer);
				serverEdit.readLine(line);
				setServer(lastServer, serverEdit);
			}
			if (level == 2)
			{
				Server serverEdit = getServer(lastServer);
				locationServer = serverEdit.getLocationServer(serverEdit.getLastLocation());
				locationServer.readLine(line);
				serverEdit.setLocationServer(serverEdit.getLastLocation(), locationServer);
				setServer(lastServer, serverEdit);
			}
		}
	}
	myFile.close();
}

std::ostream&	operator<<(std::ostream& o, const Servers& i)
{
	(void)i;
	o << "something";
	return o;
}
