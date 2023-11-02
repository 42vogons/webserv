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

HandleRequest::HandleRequest(void) {
	return ;
}

HandleRequest::HandleRequest(const HandleRequest& obj) {
	*this = obj;
	return ;
}

HandleRequest::~HandleRequest(void) {
	return ;
}

HandleRequest& HandleRequest::operator=(const HandleRequest& obj) {
	if (this != &obj) {
		this->_headers = obj._headers;
		this->_body = obj._body;
		this->_typePost = obj._typePost;
	}
	return (*this);
}

void HandleRequest::readBody(std::string buffer, int client_fd){
	size_t header_end = buffer.find("\r\n\r\n");
	std::string header2 = buffer.substr(0, header_end);
	std::string body = buffer.substr(header_end + 4);

	//std::cout << "body HTTP:\n" << body.length() << std::endl;
	//std::cout << "Content-Length HTTP:\n" << _headers["Content-Length"] << std::endl;
	while (static_cast<int>(body.length()) < std::atoi(_headers["Content-Length"].c_str())){
		//std::cout << "Continua lendo:\n"  << std::endl;
		body += receiveBody(client_fd);
	}
	
	if (body.length()> 0){
		
		if (buffer.find("boundary") != std::string::npos) {
			std::string contentDisposition = "Content-Disposition: form-data; name=\"file\"; filename=\"";
			size_t fileNameStart = buffer.find(contentDisposition);
			fileNameStart += contentDisposition.length();
			size_t fileNameEnd = buffer.find("\"", fileNameStart);
			this->_headers["fileName"] = buffer.substr(fileNameStart, fileNameEnd - fileNameStart); 
			_typePost = "File";
			size_t headerEndPos = body.find("\r\n\r\n");
			std::string binaryContent = body.substr(headerEndPos + 4);
			this->setBody(binaryContent);
		} else {
			this->setBody(body);
		}
	}
}


void HandleRequest::readBuffer(std::string buffer, int client_fd)
{
	std::string::size_type start = 0;
	std::string::size_type end = 0;
	std::string line;
	std::string key;
	std::string value;
	std::stringstream file(buffer);

	//std::cout << "in" << buffer << std::endl;

	std::getline(file, line);

	std::istringstream iss(line);
	std::string protocol;
	iss >> _headers["Method"] >> protocol >> _headers["Version"];
	
	std::string baseUrl;
	std::string protocolConverted = replaceAll(protocol, "%2F", "/");
	std::vector<std::string> result = split(protocolConverted, '/');
	if (result.size() == 0)
		return;
	size_t i;
    for (i = 1; i < result.size() -1 ; ++i) {
		if (i > result.size())
			std::cout<< "algum erro ocorreu" << protocolConverted << "*" << result.size() << "*" << std::endl;
		baseUrl += "/" + result[i];
    }
	if (baseUrl[0] == '/' && baseUrl.size() > 1 && baseUrl[1] == '/' )
		baseUrl.erase(0, 1);
	
	_headers["BaseUrl"] = baseUrl;
	_headers["LastPath"] = result[i];
	
	if (_headers["BaseUrl"] == "")
		_headers["BaseUrl"] = "/";
	
	while (std::getline(file, line)) {
		start = 0;
		end = line.find(':');
		key = line.substr(start, end - start);
		start = end + 1;
		end = line.size() - start - 1;
		while (end > 0 && line[start] == ' ') {
            ++start;
            --end;
        }
		value = line.substr(start, end);

		std::string delimiter = "\r\n\r\n";

		if (key == "\r")
			break;
		else
			_headers[key] = value;
	}

	readBody(buffer, client_fd);
	getHostAndPort(_headers["Host"]);
}

void HandleRequest::getHostAndPort(std::string protocol){

	int start = 0;
	int end = protocol.find(':');
	_headers["Host"] = protocol.substr(start, end - start);
	start = end + 1;
	end = protocol.size() - start - 1;
	_headers["Port"] = protocol.substr(start, end + 1);
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
	
	bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
	received.append(buffer, bytes_received);
	return received;
}

std::string HandleRequest::receiveBody(int client_fd){

	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	std::string received;

	while(true) {
		int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
		if (bytes_received <= 0)
			return "";
		received.append(buffer, bytes_received);
		if (bytes_received < BUFFER_SIZE) 
            break;
        
	}
	return received;
}


void HandleRequest::receiveFile(int client_fd) {
	
	std::string buffer;
	buffer = receiveBody(client_fd);

	std::string contentDisposition = "Content-Disposition: form-data; name=\"file\"; filename=\"";
	size_t fileNameStart = buffer.find(contentDisposition);
	fileNameStart += contentDisposition.length();
	size_t fileNameEnd = buffer.find("\"", fileNameStart);
	this->_headers["fileName"] = buffer.substr(fileNameStart, fileNameEnd - fileNameStart); 
	std::string delimiter = "\r\n\r\n";
	size_t start = buffer.find(delimiter) + delimiter.length();
	this->setBody(buffer.substr(start));

	//std::cout << "filename=" <<  buffer.substr(fileNameStart, fileNameEnd - fileNameStart) << "**" << std::endl;
}

void	HandleRequest::setBody(std::string body){
	_body = body;
}

std::string HandleRequest::getField(std::string field) {
	return _headers[field];
}

std::string HandleRequest::getBody(void){
	return _body;
}

std::ostream&	operator<<(std::ostream& o, const HandleRequest& i) {
	(void)i;
	o << "something";
	return o;
}
