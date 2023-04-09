/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfigServer.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:32:15 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/09 17:33:45 by anolivei         ###   ########.fr       */
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
	}
	return (*this);
}

std::ostream&	operator<<(std::ostream& o, const LocationConfigServer& i)
{
	o << "something";
	return o;
}
