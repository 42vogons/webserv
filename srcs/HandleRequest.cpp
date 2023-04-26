/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 22:37:12 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/24 00:04:18 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HandleRequest.hpp"

HandleRequest::HandleRequest(void)
{
	return ;
}

HandleRequest::HandleRequest(const HandleRequest& obj)
{
	*this = obj;
	return ;
}

HandleRequest::~HandleRequest(void)
{
	return ;
}

HandleRequest& HandleRequest::operator=(const HandleRequest& obj)
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

void HandleRequest::readBuffer(std::string buffer)
{
	std::string key;
	std::string value;

	//std::cout << "in" << buffer << std::endl;

	
	std::istringstream iss(buffer);
	std::string protocol;
	iss >> _method >> protocol >> _version;
	std::size_t lastSlashPos = protocol.find_last_of("/");
	if (lastSlashPos != std::string::npos)
	{
		_baseURL = protocol.substr(0, lastSlashPos + 1);
		_endpoint = protocol.substr(lastSlashPos + 1);
	}
	else
	{
		_baseURL = "";
		_endpoint = protocol;
	}
	if (_baseURL == "")
		_baseURL = "/";
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

		if (key.find("Content-Type:") == 0)
		{
			iss >> _contentType;
		}
		
		std::cout << "key" << key << std::endl;
		if (key.find("Content-Disposition:") == 0)
		{
			iss >> _contentDisposition;
			std::cout  <<"Content-Disposition=====" << _contentDisposition << std::endl;
		}

		
		if (key.find("Content-Length:") == 0)
		{
			iss >> _contentLength;
			//break;
		}
		
		
	}
	return ;
}

std::string HandleRequest::getHost(void)
{
	return (this->_host);
}

std::string HandleRequest::getMethod(void)
{
	return (this->_method);
}

std::string HandleRequest::getBaseURL(void)
{
	return (this->_baseURL);
}

std::string HandleRequest::getEndpoint(void)
{
	return (this->_endpoint);
}

std::ostream&	operator<<(std::ostream& o, const HandleRequest& i)
{
	(void)i;
	o << "something";
	return o;
}
