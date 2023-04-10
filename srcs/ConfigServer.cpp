/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:38:55 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/10 02:03:09 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigServer.hpp"

ConfigServer::ConfigServer(void)
{
	std::cout
		<< "ConfigServer default constructor called"
		<< std::endl;
	return ;
}

ConfigServer::ConfigServer(const ConfigServer& obj)
{
	std::cout
		<< "ConfigServer copy constructor called"
		<< std::endl;
	*this = obj;
	return ;
}

ConfigServer::~ConfigServer(void)
{
	std::cout
		<< "ConfigServer destructor called"
		<< std::endl;
	return ;
}

ConfigServer& ConfigServer::operator=(const ConfigServer& obj)
{
	if (this != &obj)
	{
		this->_serverName = obj._serverName;
		this->_errorPages = obj._errorPages;
		this->_ports = obj._ports;
		this->_clientMaxBodySize = obj._clientMaxBodySize;
		this->_locationConfigServer = obj._locationConfigServer;
		
	}
	return (*this);
}

void	ConfigServer::setServeName(std::string serverName){ this->_serverName = serverName;}
void	ConfigServer::setErrorPages(int code, std::string page){ 
	this->_errorPages[code] =  page;
}
void	ConfigServer::setPorts(int port){ 
	if (_ports.find(port) == _ports.end())
		_ports.insert(port);
}
void	ConfigServer::setClientMaxBodySize(int clientMaxBodySize){ this->_clientMaxBodySize = clientMaxBodySize;}
void	ConfigServer::setLocationConfigServer(std::string name, LocationConfigServer locattionConfigServer){
	this->_locationConfigServer[name] =  locattionConfigServer;
}
std::string	ConfigServer::getServerName(void){return _serverName;}
std::string	ConfigServer::getErrorPages(int code){
	// fazer verificação se não é nulo, se for estourar erro
	return _errorPages.find(code)->second;
}
std::set<int>	ConfigServer::getPorts(void){
	return _ports;
}

int	ConfigServer::getClientMaxBodySize(void){return _clientMaxBodySize;}

LocationConfigServer	ConfigServer::getLocationConfigServer(std::string name){
	return _locationConfigServer.find(name)->second;
}

std::ostream&	operator<<(std::ostream& o, const ConfigServer& i)
{
	(void)i;
	o << "something";
	return o;
}
