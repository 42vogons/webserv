/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:38:55 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/16 00:16:30 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) : _serverName("default") {
	_clientMaxBodySize = 0;
	_sizeLocation = 0;
	_status = true;
	return ;
}

Server::Server(std::string name) : _serverName(name) {
	_clientMaxBodySize = 0;
	_sizeLocation = 0;
	_status = true;
	return ;
}

Server::Server(const Server& obj) {
	*this = obj;
	return ;
}

Server::~Server(void) {
	return ;
}

Server& Server::operator=(const Server& obj) {
	if (this != &obj) {
		this->_ports = obj._ports;
		this->_serverName = obj._serverName;
		this->_clientMaxBodySize = obj._clientMaxBodySize;
		this->_errorPages = obj._errorPages;
		this->_locationServer = obj._locationServer;
		this->_lastLocation = obj._lastLocation;
		this->_sizeLocation = obj._sizeLocation;
		this->_hostNames = obj._hostNames;
		this->_status = obj._status;
	}
	return (*this);
}

void Server::readLine(std::string line) {
	std::string			key;
	std::string			valueString;
	int					valueInt;
	std::istringstream	iss(line);
	iss >> key;
	if (key.empty() || key.substr(0, 1) == "#")
		return;
	if (key.find("listen") == 0) {
		while (iss >> valueInt)
			this->setPorts(valueInt);
	}
	if (key.find("server_name") == 0) {
		while(iss >> valueString) {
			this->setHostNames(valueString);
			addHostServerName(valueString,"127.0.0.1");	
		}
		iss >> valueString;
		this->setServeName(valueString);
	}
	if (key.find("client_max_body_size") == 0) {
		iss >> valueInt;
		this->setClientMaxBodySize(valueInt);
	}
	if (key.find("error_page") == 0) {
		iss >> valueInt >> valueString;
		this->setErrorPages(valueInt, valueString);
	}
	if (key.find("location") == 0) {
		iss >> valueString;
		LocationServer locationServer;
		this->_lastLocation = valueString;
		this->setLocationServer(valueString, locationServer);
	}
}

void Server::setHostNames(std::string hostName) {
	if (this->_hostNames.find(hostName)== this->_hostNames.end())
		this->_hostNames.insert(hostName);
}

std::set<std::string> Server::getHostNames(void) {
	return _hostNames;
}

void Server::setPorts(int port) {
	if (this->_ports.find(port) == this->_ports.end())
		this->_ports.insert(port);
}

void Server::setServeName(std::string serverName) {
	this->_serverName = serverName;
}

void Server::setClientMaxBodySize(int clientMaxBodySize) {
	this->_clientMaxBodySize = clientMaxBodySize;
}

void Server::setErrorPages(int code, std::string page) {
	this->_errorPages[code] =  page;
}

void Server::setLocationServer(std::string name, LocationServer locationServer) {
	this->_locationServer[name] =  locationServer;
	this->_sizeLocation += 1;
}

void Server::setStatus(bool status){
	this->_status = status;
}

std::set<int> Server::getPorts(void) const {
	return (this->_ports);
}

std::string Server::getServerName(void) const {
	return (this->_serverName);
}

int Server::getClientMaxBodySize(void) const {
	return (this->_clientMaxBodySize);
}

std::string Server::getErrorPages(int code) const {
	if (this->_errorPages.find(code) != this->_errorPages.end())
		return (this->_errorPages.find(code)->second);
	return (this->_errorPages.find(404)->second);
}

LocationServer Server::getLocationServer(std::string name) const {
	if (this->_locationServer.find(name) != this->_locationServer.end())
		return (this->_locationServer.find(name)->second);
	return LocationServer();
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

std::string Server::getLastLocation(void) const {
	return (this->_lastLocation);
}

bool Server::getStatus(void) {
	return (this->_status);
}

int Server::getSizeLocation(void) {
	return this->_sizeLocation;
}

std::ostream& operator<<(std::ostream& o, const Server& i) {
	(void)i;
	o << "server: " << i.getServerName();
	return o;
}
