/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:34:23 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/14 19:04:21 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"

Servers::Servers(void)
{
	/*std::cout
		<< "Servers default constructor called"
		<< std::endl;*/
	return ;
}

Servers::Servers(const Servers& obj)
{
	/*std::cout
		<< "Servers copy constructor called"
		<< std::endl;*/
	*this = obj;
	return ;
}

Servers::~Servers(void)
{
	/*std::cout
		<< "Servers destructor called"
		<< std::endl;*/
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

void Servers::addServer(std::string serverName, Server server){

	this->_servers[serverName] =  server;
	
	//this->_Servers.insert("serverName", new Server("serverName"));
	std::cout
		<< "add" 
		<< serverName
		<< std::endl;
}

void	Servers::setServer(std::string serverName, Server server){
	this->_servers[serverName] =  server;
}

int Servers::countTabs(std::string line){
	int countTab = 0;
	for (unsigned int i = 0; i < line.length(); i++) {
        if (line[i] == '\t') {
            countTab++;
        } else {
			break;
		}
	}
	return countTab;
}

Server Servers::getServer(std::string serverName){
	return _servers[serverName];
}

void Servers::readFile(std::string fileName){
	std::ifstream myFile(fileName.c_str()); // abre o arquivo para leitura
	std::string line;
	std::string key, name;
	std::string lastServer;
	LocationServer LocationServer;

	int nivel;

    if (!myFile.is_open()) { // verifica se o arquivo foi aberto corretamente
        std::cerr << "Erro ao abrir arquivo" << std::endl;
    } else {
		while (!myFile.eof() )
		{
			getline(myFile, line);
			nivel = countTabs(line);
			if (nivel == 0 && line.size() > 1){
				std::istringstream iss(line); 
    			iss >> key;
				Server server(key);
				lastServer = key;
				addServer(key, server);
			}
			if (nivel == 1){
				
				Server serverEdit = getServer(lastServer);
				serverEdit.readLine(line);
				setServer(lastServer, serverEdit);
				
			}
			/*if (nivel == 2){
				std::istringstream iss(line);
				iss >> key >> name;
				if (key = "l")
				Server serverEdit = getServer(lastServer);
				serverEdit.setLocationServer()
			}*/

			
		}
	}
	myFile.close();
}


std::ostream&	operator<<(std::ostream& o, const Servers& i)
{
	
	o << "something"<< i;;
	return o;
}
