/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receiver.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 22:37:12 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/24 23:11:21 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Receiver.hpp"

Receiver::Receiver(void)
{
	return ;
}

Receiver::Receiver(const Receiver& obj)
{
	*this = obj;
	return ;
}

Receiver::~Receiver(void)
{
	return ;
}

Receiver& Receiver::operator=(const Receiver& obj)
{
	if (this != &obj)
	{
		this->_method = obj._method;
		this->_baseURL = obj._baseURL;
		this->_endpoint = obj._endpoint;
		this->_version = obj._version;
		this->_host = obj._host;
		this->_contentLength = obj._contentLength;
		this->_body = obj._body;
	}
	return (*this);
}

void Receiver::readBuffer(std::string buffer)
{
	std::string key;
	std::string value;

	std::cout << "in" << buffer << std::endl;

	
	std::istringstream iss(buffer);
	std::string protocol;
	iss >> _method >> protocol >> _version;
	std::size_t lastSlashPos = protocol.find_last_of("/");
	if (lastSlashPos != std::string::npos)
	{
		std::cout << "1" << std::endl;
		_baseURL = protocol.substr(0, lastSlashPos + 1);
		_endpoint = protocol.substr(lastSlashPos + 1);
	}
	else
	{
		_baseURL = "";
		_endpoint = protocol;
	}
	std::cout << "protocol" << protocol << std::endl;
	std::cout << "_baseURL" << _baseURL << std::endl;
	std::cout << "_endpoint" << _endpoint << std::endl;

	
	while (std::getline(iss, key))
	{
		iss >> key;
		if (key.find("Host:") == 0)
		{
			iss >> value;
			std::cout << "host full: " << value << std::endl;
			std::size_t x = value.find(':');
			std::cout << "x: " << x << std::endl;
			_host = value.substr(0, value.find(':'));
			std::cout << "host: " << _host << std::endl;
		}

		if (key.find("Content-Type") == 0)
		{
			iss >> _contentType;
		}

		if (key.find("Content-Disposition"))
		{
			iss >> _contentDisposition;
			std::cout << _contentDisposition << std::endl;
		}

		
		if (key.find("Content-Length:") == 0)
		{
			iss >> _contentLength;
			//break;
		}
		
		
	}
	return ;
}

std::string Receiver::getHost(void)
{
	return (this->_host);
}

std::string Receiver::getMethod(void)
{
	return (this->_method);
}

std::string Receiver::getBaseURL(void)
{
	return (this->_baseURL);
}

std::string Receiver::getEndpoint(void)
{
	return (this->_endpoint);
}

std::ostream&	operator<<(std::ostream& o, const Receiver& i)
{
	(void)i;
	o << "something";
	return o;
}
