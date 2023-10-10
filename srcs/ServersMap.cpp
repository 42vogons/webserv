/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersMap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:34:23 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/16 23:31:12 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServersMap.hpp"

ServersMap::ServersMap(void)
{
	return ;
}

ServersMap::ServersMap(const ServersMap& obj)
{
	*this = obj;
	return ;
}

ServersMap::~ServersMap(void)
{
	return ;
}

ServersMap& ServersMap::operator=(const ServersMap& obj)
{
	if (this != &obj){
		this->_serversMap = obj._serversMap;
		this->_portsAccepted = obj._portsAccepted;
	}
		
		
	return (*this);
}

void ServersMap::addServer(std::string serverName, Server server)
{
	this->_serversMap[serverName] = server;
}

std::set<int>	ServersMap::checkServers(void){

	std::map<std::string, Server> servers = this->getServersMap();

	

	for (std::map<std::string, Server>::iterator it = servers.begin(); it != servers.end(); ++it) 
	{
		std::string errors = "";
		std::cout << "Server: " << it->first;
		Server server = it->second;
		if (server.getClientMaxBodySize()  < 0)
			errors += " Max body size is inválid ";
		if (server.getPorts().empty())
			errors += " Invalid ports ";
		if (server.getSizeLocation() == 0)
			errors += " No Locations registred ";
		if (errors != ""){
			server.setStatus(false);
			std::cout << errors << "\r\n" << std::endl;
		} else {
			server.setStatus(true);
			std::cout << "OK\r\n" << std::endl;
		}
		std::set<int> ports = server.getPorts();
		for (std::set<int>::iterator pt = ports.begin(); pt != ports.end(); ++pt) {
			this->_portsAccepted.insert(*pt);
		}
	}
	return this->_portsAccepted;
}


void	ServersMap::readFile(std::string fileName)
{
	std::ifstream	myFile(fileName.c_str());
	std::string		line;
	std::string		key, name;
	std::string		lastServer;
	LocationServer	locationServer;
	int				level;

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

int ServersMap::countTabs(std::string line)
{
	int countTab = 0;
	for (unsigned int i = 0; i < line.length(); i++)
	{
		if (line[i] == '\t')
			countTab++;
		else
			break;
	}
	return (countTab);
}

void	ServersMap::setServer(std::string serverName, Server server)
{
	this->_serversMap[serverName] =  server;
}

Server	ServersMap::getServer(std::string serverName)
{
	return (this->_serversMap[serverName]);
}

std::map<std::string, Server>	ServersMap::getServersMap(void)
{
	return (this->_serversMap);
}

std::ostream&	operator<<(std::ostream& o, const ServersMap& i)
{
	(void)i;
	o << "something";
	return o;
}
