/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:38:55 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/14 18:27:49 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
	/*std::cout
		<< "Server default constructor called"
		<< std::endl;*/
	return ;
}

Server::Server(std::string name)
{
	_serverName = name;
	return ;
}

Server::Server(const Server& obj)
{
	/*std::cout
		<< "Server copy constructor called"
		<< std::endl;*/
	*this = obj;
	return ;
}

Server::~Server(void)
{
	/*std::cout
		<< "Server destructor called"
		<< std::endl;*/
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
		
	}
	return (*this);
}

void	Server::readLine(std::string line){

	std::string key, arg2;

	int value;
	
	std::istringstream iss(line); // cria um stream de string para separar a primeira palavra da linha
    iss >> key; // lê a primeira palavra da linha usando o stream de string
	if (key.find("listen") == 0) {
		while (iss >> value)
		{
			this->setPorts(value);
			std::cout << "listen" << value << std::endl;
		}
	}
	if (key.find("server_name") == 0){
		iss >> arg2;
		this->setServeName(arg2);
	}
	if (key.find("client_max_body_size") == 0){
		iss >> value;
		this->setClientMaxBodySize(value);
	}
	
	if (key.find("error_page") == 0){
		iss >> value >> arg2;
		this->setErrorPages(value, arg2);
	}
}

void	Server::setServeName(std::string serverName){ this->_serverName = serverName;}
void	Server::setErrorPages(int code, std::string page){ 
	this->_errorPages[code] =  page;
}
void	Server::setPorts(int port){
	if (_ports.find(port) == _ports.end()){
		_ports.insert(port);
	}
}
void	Server::setClientMaxBodySize(int clientMaxBodySize){ this->_clientMaxBodySize = clientMaxBodySize;}
void	Server::setLocationServer(std::string name, LocationServer locattionServer){
	this->_locationServer[name] =  locattionServer;
}
std::string	Server::getServerName(void){return _serverName;}
std::string	Server::getErrorPages(int code){
	// fazer verificação se não é nulo, se for estourar erro
	return _errorPages.find(code)->second;
}
std::set<int>	Server::getPorts(void) const{
	return _ports;
}

int	Server::getClientMaxBodySize(void){return _clientMaxBodySize;}

LocationServer	Server::getLocationServer(std::string name){
	return _locationServer.find(name)->second;
}

std::ostream&	operator<<(std::ostream& o, const Server& i)
{
	o << "something" << i;
	return o;
}
