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

LocationServer::LocationServer(void) {
	return ;
}

LocationServer::LocationServer(const LocationServer& obj) {
	*this = obj;
	return ;
}

LocationServer::~LocationServer(void) {
	return ;
}

LocationServer& LocationServer::operator=(const LocationServer& obj) {
	if (this != &obj) {
		this->_allowedMethods = obj._allowedMethods;
		this->_pagesIndex = obj._pagesIndex;
		this->_cgiParam = obj._cgiParam;
		this->_variables = obj._variables;
	}
	return (*this);
}

void	LocationServer::setCgiParam(std::string key, std::string value) {
	_cgiParam[key] = value;
}

void	LocationServer::setAllowedMethods(std::string name, bool status) {
	_allowedMethods[name] = status;
}

void	LocationServer::setPagesIndex(std::string page) {
	if(_pagesIndex.find(page) == _pagesIndex.end())
		_pagesIndex.insert(page);
}

bool	LocationServer::getAllowedMethods(std::string methods) {
	if (_allowedMethods.find(methods) != _allowedMethods.end())
		return _allowedMethods.find(methods)->second;
	else
		return 0;
}

std::set<std::string>	LocationServer::getPagesIndex(void) {
	return _pagesIndex;
}

std::string	LocationServer::getCgiParm(std::string param) {
	if (_cgiParam.find(param) != _cgiParam.end())
		return _cgiParam.find(param)->second;
	return NULL;
}

std::string LocationServer::getAllCgiParm(void) {
	std::string concatenatedArguments;
    for (std::map<std::string, std::string>::iterator it = _cgiParam.begin(); it != _cgiParam.end(); ++it)
		concatenatedArguments += it->first + "=" + it->second + "&";
    if (!concatenatedArguments.empty())
        concatenatedArguments.erase(concatenatedArguments.size() - 1);
    return concatenatedArguments;
}

std::string	LocationServer::getField(std::string field) {
	return _variables[field];
}

void	LocationServer::readLine(std::string line) {
	std::string key, valueString;
	std::istringstream iss(line);
	iss >> key >> valueString;
	if (key.empty() || key.substr(0, 1) == "#")
		return;
	if (key.find("allowed_methods") == 0) 	{
		key = valueString;
		iss >> valueString;
		if (valueString == "true")
			this->setAllowedMethods(key, true);
		else
			this->setAllowedMethods(key, false);
		return;
	}	

	if (key.find("cgi_param") == 0) {
		key = valueString;
		iss >> valueString;
		this->setCgiParam(key, valueString);
		return;
	}
	if (key.find("index") == 0) {
		this->setPagesIndex(valueString);
		while (iss >> valueString)
			this->setPagesIndex(valueString);
		return;
	}
	iss >> valueString;
	_variables[key] = valueString;
}

std::ostream&	operator<<(std::ostream& o, const LocationServer& i) {
	(void)i;
	o << "something";
	return o;
}
