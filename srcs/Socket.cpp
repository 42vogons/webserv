/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:26 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/09 17:39:27 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void)
{
	std::cout
		<< "Socket default constructor called"
		<< std::endl;
	return ;
}

Socket::Socket(const Socket& obj)
{
	std::cout
		<< "Socket copy constructor called"
		<< std::endl;
	*this = obj;
	return ;
}

Socket::~Socket(void)
{
	std::cout
		<< "Socket destructor called"
		<< std::endl;
	return ;
}

Socket& Socket::operator=(const Socket& obj)
{
	if (this != &obj)
	{
		this->XXX = obj.XXX();
	}
	return (*this);
}

std::ostream&	operator<<(std::ostream& o, const Socket& i)
{
	o << "something";
	return o;
}
