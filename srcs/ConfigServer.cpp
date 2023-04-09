/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:38:55 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/09 17:39:09 by anolivei         ###   ########.fr       */
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
	}
	return (*this);
}

std::ostream&	operator<<(std::ostream& o, const ConfigServer& i)
{
	o << "something";
	return o;
}
