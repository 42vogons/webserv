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
	return ;
}

LocationServer::LocationServer(const LocationServer& obj)
{
	*this = obj;
	return ;
}

LocationServer::~LocationServer(void)
{
	return ;
}

LocationServer& LocationServer::operator=(const LocationServer& obj)
{
	if (this != &obj)
	{
		//this->_autoIndex = obj._autoIndex;
		//this->_root = obj._root;
		this->_allowedMethods = obj._allowedMethods;
		this->_pagesIndex = obj._pagesIndex;
		//this->_cgiPass = obj._cgiPass;
		//this->_redirect = obj._redirect;
		//this->_updatePath = obj._updatePath;
		this->_cgiParam = obj._cgiParam;
		this->_variables = obj._variables;
	}
	return (*this);
}

/*void	LocationServer::setAutoIndex(bool autoIndex)
{
	_autoIndex = autoIndex;
}

void	LocationServer::setRoot(std::string root)
{
	_root = root;
}

void	LocationServer::setCgiPass(std::string cgiPass)
{
	_cgiPass= cgiPass;
}

void	LocationServer::setRedirect(std::string redirect)
{
	_redirect= redirect;
}

void	LocationServer::setUpdatePath(std::string updatePath)
{
	_updatePath= updatePath;
}*/

void	LocationServer::setCgiParam(std::string key, std::string value)
{
	_cgiParam[key] = value;
}

void	LocationServer::setAllowedMethods(std::string name, bool status)
{
	_allowedMethods[name] = status;
}

void	LocationServer::setPagesIndex(std::string page)
{
	if(_pagesIndex.find(page) == _pagesIndex.end())
		_pagesIndex.insert(page);
}

/*
bool	LocationServer::getAutoIndex(void)
{
	return _autoIndex;
}

std::string	LocationServer::getRoot(void)
{
	return _root;
}*/

bool	LocationServer::getAllowedMethods(std::string methods)
{

	if (_allowedMethods.find(methods) != _allowedMethods.end())
		return _allowedMethods.find(methods)->second;
	else
		return 0;
}

std::set<std::string>	LocationServer::getPagesIndex(void)
{
	return _pagesIndex;
}

/*


std::string	LocationServer::getCgiPass(void)
{
	return _cgiPass;
}

std::string	LocationServer::getRedirect(void)
{
	return _redirect;
}

std::string	LocationServer::getUpdatePath(void)
{
	return _updatePath;
}*/

std::string	LocationServer::getCgiParm(std::string param)
{
	if (_cgiParam.find(param) != _cgiParam.end())
		return _cgiParam.find(param)->second;
	return NULL;
}

std::string	LocationServer::getField(std::string field)
{
	return _variables[field];
}

void	LocationServer::readLine(std::string line)
{
	std::string key, valueString, valueString2, value ;
	std::istringstream iss(line);
	iss >> key >> value ;
	if (key.empty() || key.substr(0, 1) == "#")
		return;

	if (key.find("allowed_methods") == 0)
	{
		key = value;
		iss >> value ;
	}	

	_variables[key] = value;
	if (key.find("cgi_param") == 0)
	{
		iss >> valueString >> valueString2;
		this->setCgiParam(valueString, valueString2);
	}
	if (key.find("index") == 0)
	{
		while (iss >> valueString)
			this->setPagesIndex(valueString);
	}

	

	

}

std::ostream&	operator<<(std::ostream& o, const LocationServer& i)
{
	(void)i;
	o << "something";
	return o;
}
