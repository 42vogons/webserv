/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationServer.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:32:15 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/10 00:48:38 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationServer.hpp"

LocationServer::LocationServer(void)
{
	std::cout
		<< "LocationServer default constructor called"
		<< std::endl;
	return ;
}

LocationServer::LocationServer(const LocationServer& obj)
{
	std::cout
		<< "LocationServer copy constructor called"
		<< std::endl;
	*this = obj;
	return ;
}

LocationServer::~LocationServer(void)
{
	std::cout
		<< "LocationServer destructor called"
		<< std::endl;
	return ;
}

LocationServer& LocationServer::operator=(const LocationServer& obj)
{
	if (this != &obj)
	{
		this->_autoIndex = obj._autoIndex;
		this->_root = obj._root;
		this->_allowedMethods = obj._allowedMethods;
		this->_pagesIndex = obj._pagesIndex;
		this->_cgiPass = obj._cgiPass;
		
	}
	return (*this);
}

void	LocationServer::setAutoIndex(bool autoIndex){_autoIndex = autoIndex;}
void	LocationServer::setRoot(std::string root){_root = root;}
void	LocationServer::setCgiPass(std::string cgiPass){ _cgiPass= cgiPass;}
void	LocationServer::setRedirect(std::string redirect){ _redirect= redirect;}
void	LocationServer::setUpdatePath(std::string updatePath){ _updatePath= updatePath;}
void	LocationServer::setCgiParam(std::string key, std::string value){
	_cgiParam[key] = value;
}
void	LocationServer::setAllowedMethods(std::string name, bool status){
	_allowedMethods[name] = status;
}
void	LocationServer::setPagesIndex(std::string page){
	if(_pagesIndex.find(page) == _pagesIndex.end())
		_pagesIndex.insert(page);
}


bool	LocationServer::getAutoIndex(void){return _autoIndex;}
std::string	LocationServer::getRoot(void){return _root;}
bool	LocationServer::getAllowedMethods(std::string methods){
	// fazer verificação se não é nulo, se for estourar erro
	return _allowedMethods.find(methods)->second;
}
std::set<std::string>	LocationServer::getPagesIndex(void){return _pagesIndex;}
std::string	LocationServer::getCgiPass(void){return _cgiPass;}

std::string	LocationServer::getRedirect(void){return _redirect;}
std::string	LocationServer::getUpdatePath(void){return _updatePath;}
std::string	LocationServer::getCgiParm(std::string param){
	// fazer verificação se não é nulo se for extoura erro
	return _cgiParam.find(param)->second;
}

void	LocationServer::readLine(std::string line){

	std::string key, valueString, valueString2;
	bool valueBool;
	//int valueInt;
	
	std::istringstream iss(line); // cria um stream de string para separar a primeira palavra da linha
    iss >> key; // lê a primeira palavra da linha usando o stream de string
	if (key.find("redirection") == 0) {
		this->setRedirect(valueString);
	}
	if (key.find("root") == 0) {
		this->setRoot(valueString);
	}
	if (key.find("upload_path") == 0) {
		this->setUpdatePath(valueString);
	}
	if (key.find("cgi_pass") == 0) {
		this->setCgiPass(valueString);
	}
	if (key.find("autoindex")){
		iss >> valueBool;
		this->setAutoIndex(valueBool);
	}
	if (key.find("allowed_methods")){
		iss >> valueString >> valueBool;
		this->setAllowedMethods(valueString, valueBool);
	}
	if (key.find("cgi_param")){
		iss >> valueString >> valueString2;
		this->setCgiParam(valueString, valueString2);
	}
}




std::ostream&	operator<<(std::ostream& o, const LocationServer& i)
{
	o << "something" << i;
	return o;
}


