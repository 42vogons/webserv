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
		this->_headers = obj._headers;
	}
	return (*this);
}

void HandleRequest::readBuffer(std::string buffer)
{
	std::string::size_type start = 0;
	std::string::size_type end = 0;

	std::string line;
	std::string key;
	std::string value;
	std::stringstream file(buffer);

	std::cout << "in" << buffer << std::endl;

	std::getline(file, line);

	std::istringstream iss(line);
	std::string protocol;
	iss >> _headers["Method"] >> protocol >> _headers["Version"];
	std::size_t lastSlashPos = protocol.find_last_of("/");
	if (lastSlashPos != std::string::npos)
	{
		_headers["BaseUrl"] = protocol.substr(0, lastSlashPos + 1);
		_headers["Endpoint"] = protocol.substr(lastSlashPos + 1);
	}
	if (_headers["BaseUrl"] == "")
	{
		_headers["BaseUrl"] = "/";
		_headers["Endpoint"] = protocol;
	}
	
	// precisa melhorar essa função

	while (std::getline(file, line))
	{
		start = 0;
		end = line.find(':');
		key = line.substr(start, end - start);
		start = end + 1;
		end = line.size() - start - 1;
		// remove os espaços iniciais
		while (end > 0 && line[start] == ' ') {
            ++start;
            --end;
        }
		value = line.substr(start, end);

		std::cout << "key = *"<< key <<"* value=*"<< value << "*" << std::endl;
		_headers[key] = value;
	}
	start = 0;
	line = _headers["Host"];
	end = line.find(':');
	_headers["Host"] = line.substr(start, end - start);
	start = end + 1;
	end = line.size() - start - 1;
	_headers["Port"] = line.substr(start, end);
	return ;
}

std::string HandleRequest::getField(std::string field)
{
	return _headers[field];
}

std::ostream&	operator<<(std::ostream& o, const HandleRequest& i)
{
	(void)i;
	o << "something";
	return o;
}
