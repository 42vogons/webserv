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
		this->_body = obj._body;
		this->_typePost = obj._typePost;
	}
	return (*this);
}

void HandleRequest::readBody(std::string body){

	bool isBody = false;

	std::string::size_type start = 0;
	std::string::size_type end = 0;
	std::string line;
	std::string key;
	std::string value;
	std::stringstream file(body);
	while (std::getline(file, line))
	{
		if (isBody == true){
			//if (line.find("------WebKitFormBoundary") == std::string::npos) {
            	_body += line;
				continue;
        	//}
			
		}
		
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
		if (value == "")
			isBody = true;
		else
			_headers[key] = value;
	}


}

void HandleRequest::readBuffer(std::string buffer, int client_fd)
{
	bool isBody = false;

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
	std::cout << "last" << protocol.size();
	if (lastSlashPos != std::string::npos)
	{
		if (protocol.size() - 1 != lastSlashPos)
		{
			_headers["BaseUrl"] = protocol.substr(0, lastSlashPos + 1);
			_headers["Endpoint"] = protocol.substr(lastSlashPos + 1);
		}
		else
		{
			_headers["BaseUrl"] = protocol.substr(0, lastSlashPos);
			_headers["Endpoint"] = protocol.substr(lastSlashPos + 1);
		}
		
	}
	if (_headers["BaseUrl"] == "")
	{
		_headers["BaseUrl"] = "/";
		_headers["Endpoint"] = protocol;
	}

	while (std::getline(file, line))
	{
		if (isBody == true){
				_body += line;
				continue;
		}
		
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
		if (value == "")
			isBody = true;
		else
			_headers[key] = value;

	}
	
	if(_headers["Content-Type"].find("multipart/form-data")!= std::string::npos)
	{
		receiveFile(client_fd);
		_typePost = "File";

		// colocar o receiveFile aqui
	}
	
	//_body += "lines = " +qtd_lines ;
	std::cout << "Body ----------------" << std::endl;
	std::cout << _body << std::endl;
	std::cout << "Body ----------------" << std::endl;


	start = 0;
	line = _headers["Host"];
	end = line.find(':');
	_headers["Host"] = line.substr(start, end - start);
	start = end + 1;
	end = line.size() - start - 1;
	_headers["Port"] = line.substr(start, end);
	return ;
}

std::string HandleRequest::getTypePost(void){
	return _typePost;
}

std::string HandleRequest::receiveInformation(int client_fd){

	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	int bytes_received = 0;
	std::string received;
	while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) 
	{
		received.append(buffer, bytes_received);
		if (bytes_received < BUFFER_SIZE)
			break;
	}
	/*if (bytes_received == -1)
	{
		close(this->_client_fd);
		return "";
	} */
	return received;
}

void HandleRequest::receiveFile(int client_fd)
{
	
	std::string buffer;
	buffer = receiveInformation(client_fd);

	std::string contentDisposition = "Content-Disposition: form-data; name=\"file\"; filename=\"";
	size_t fileNameStart = buffer.find(contentDisposition);
	fileNameStart += contentDisposition.length();
	size_t fileNameEnd = buffer.find("\"", fileNameStart);
	this->_headers["fileName"] = buffer.substr(fileNameStart, fileNameEnd - fileNameStart); 
	std::string delimiter = "\r\n\r\n";
	size_t start = buffer.find(delimiter) + delimiter.length();
	this->setBody(buffer.substr(start));
}

void	HandleRequest::setBody(std::string body){
	_body = body;
}

std::string HandleRequest::getField(std::string field)
{
	return _headers[field];
}

std::string HandleRequest::getBody(void){
	return _body;
}

std::ostream&	operator<<(std::ostream& o, const HandleRequest& i)
{
	(void)i;
	o << "something";
	return o;
}
