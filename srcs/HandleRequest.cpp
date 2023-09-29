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

///////////////////////////////// FUNCOES UTEIS ///////////////////
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

///////////////////////////////// FUNCOES UTEIS ///////////////////
std::string replaceAll(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    std::string::size_type pos = 0;
    
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }

    return result;
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
	
	std::string endpoint;
	std::string baseUrl;
	std::string protocol2 = replaceAll(protocol, "%2F", "/");




	std::vector<std::string> result = split(protocol2, '/');
	size_t i;
    for (i = 1; i < result.size() -1 ; ++i) {
		if (i > 1)
			endpoint += "/" + result[i];
		else
			baseUrl += "/" + result[i];
			
    }
	_headers["BaseUrl"] = baseUrl;
	_headers["Endpoint"] = endpoint;
	_headers["LastPath"] = result[i];
	
	
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
	/*std::cout << "Body ----------------" << std::endl;
	std::cout << _body << std::endl;
	std::cout << "Body ----------------" << std::endl;*/

	start = 0;
	line = _headers["Host"];
	end = line.find(':');
	_headers["Host"] = line.substr(start, end - start);
	start = end + 1;
	end = line.size() - start - 1;
	_headers["Port"] = line.substr(start, end + 1);
	return ;
}

bool checkHandler(void){
	return true;
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
