/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfigServer.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:32:15 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/10 00:48:38 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfigServer.hpp"

LocationConfigServer::LocationConfigServer(void)
{
	std::cout
		<< "LocationConfigServer default constructor called"
		<< std::endl;
	return ;
}

LocationConfigServer::LocationConfigServer(const LocationConfigServer& obj)
{
	std::cout
		<< "LocationConfigServer copy constructor called"
		<< std::endl;
	*this = obj;
	return ;
}

LocationConfigServer::~LocationConfigServer(void)
{
	std::cout
		<< "LocationConfigServer destructor called"
		<< std::endl;
	return ;
}

LocationConfigServer& LocationConfigServer::operator=(const LocationConfigServer& obj)
{
	if (this != &obj)
	{
		this->_autoIndex = obj._autoIndex;
		this->_root = obj._root;
		this->_allowedMethods = obj._allowedMethods;
		this->_pagesIndex = obj._pagesIndex;
		this->_cgi = obj._cgi;
		
	}
	return (*this);
}

void	LocationConfigServer::setAutoIndex(bool autoIndex){_autoIndex = autoIndex;}
void	LocationConfigServer::setRoot(std::string root){_root = root;}
void	LocationConfigServer::setAllowedMethods(std::string name, bool status){
	_allowedMethods[name] = status;
}
void	LocationConfigServer::setPagesIndex(std::string page){
	if(_pagesIndex.find(page) == _pagesIndex.end())
		_pagesIndex.insert(page);
}
void	LocationConfigServer::setCgi(std::string cgi){ _cgi= cgi;}

bool	LocationConfigServer::getAutoIndex(void){return _autoIndex;}
std::string	LocationConfigServer::getRoot(void){return _root;}
bool	LocationConfigServer::getAllowedMethods(std::string methods){
	// fazer verificação se não é nulo, se for estourar erro
	return _allowedMethods.find(methods)->second;
}
std::set<std::string>	LocationConfigServer::getPagesIndex(void){return _pagesIndex;}
std::string	LocationConfigServer::getCgi(void){return _cgi;}

std::ostream&	operator<<(std::ostream& o, const LocationConfigServer& i)
{
	o << "something" << i;
	return o;
}
