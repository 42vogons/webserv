/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:26 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/28 15:38:31 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void) : _port(80), _server(NULL)
{
	createSocketTCP();
	configSocketAddress();
	bindSocketToAddress();
	waitConnection();
	return ;
}

Socket::Socket(int port, Server server) : _port(port), _server(server)
{
	createSocketTCP();
	configSocketAddress();
	bindSocketToAddress();
	waitConnection();
	return ;
}

Socket::Socket(const Socket& obj)
{
	*this = obj;
	return ;
}

Socket::~Socket(void)
{
	this->closeClientFd();
	this->closeServerFd();
	return ;
}

Socket& Socket::operator=(const Socket& obj)
{
	if (this != &obj)
	{
		this->_port = obj._port;
		this->_server_fd = obj._server_fd;
		this->_client_fd = obj._client_fd;
		this->_addrlen = obj._addrlen;
		this->_address = obj._address;
		this->_server = obj._server;
		this->_HandleRequest = obj._HandleRequest;
	}
	return (*this);
}

void	Socket::setHandleRequest(HandleRequest HandleRequest)
{
	this->_HandleRequest = HandleRequest;
}

HandleRequest	Socket::getHandleRequest(void)
{
	return (this->_HandleRequest);
}

void	Socket::createSocketTCP(void)
{
	int opt = 1;
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int));
}

void	Socket::configSocketAddress(void)
{
	this->_addrlen = sizeof(this->_address);
	memset(&this->_address, 0, sizeof(this->_address));
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);
}

void	Socket::bindSocketToAddress(void)
{
	bind(
		this->_server_fd,
		(struct sockaddr *)&this->_address,
		sizeof(this->_address)
	);
}

void	Socket::waitConnection(void)
{
	int queue = 100;
	listen(this->_server_fd, queue);
}

int	Socket::getServerFd(void) const
{
	return (this->_server_fd);
}

void	Socket::acceptConnection(void)
{

	const int BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE];
	std::string upload_dir = "/home/user/documents/";

	this->_client_fd = accept(this->_server_fd, (struct sockaddr *)&this->_address, (socklen_t*)&this->_addrlen);
	if (this->_client_fd == -1)
		throw (AcceptConnectionError()); 
	std::cout << "\033[0;32m\n\n\nNew connection on " << this->_server_fd << "\033[0m" << std::endl;
	
    int bytes_received = recv(_client_fd, buffer, sizeof(buffer), 0);
	if (bytes_received == -1) {
		close(this->_client_fd);
		return;
	}

	std::string http_request(buffer, buffer + bytes_received);
    std::stringstream ss(http_request);
    std::string request_line;
    getline(ss, request_line);

	std::string filename;
	std::string content_disposition = http_request.substr(http_request.find("Content-Disposition"));
    size_t filename_start = content_disposition.find("filename=") + 10;
    size_t filename_end = content_disposition.find("\"", filename_start);
    if (filename_start != std::string::npos && filename_end != std::string::npos) {
        filename = content_disposition.substr(filename_start, filename_end - filename_start);
    }
	// Check if file was uploaded
	size_t content_length_start = http_request.find("Content-Length: ") + 16;
	size_t content_length_end = http_request.find("\r\n", content_length_start);
	if (content_length_start != std::string::npos && content_length_end != std::string::npos) {
		int content_length = atoi(http_request.substr(content_length_start, content_length_end - content_length_start).c_str());
		if (content_length == 0) {
			std::cerr << "Arquivo não enviado pelo cliente" << std::endl;
			close(_client_fd);
			return;
		}
		int bytes_left = content_length;
		/*std::ifstream file(upload_dir + filename, std::ifstream::in);
		        //std::ofstream file(upload_dir + filename, std::ios_base::out | std::ios_base::binary);
        if (!file.is_open()) {
            std::cerr << "Erro ao criar arquivo " << filename << std::endl;
            close(_client_fd);
            return;
        }*/

        while (bytes_left > 0) {
            int bytes_to_read = std::min(bytes_left, BUFFER_SIZE);
            bytes_received = recv(_client_fd, buffer, bytes_to_read, 0);
            if (bytes_received == -1) {
                std::cerr << "Erro ao receber dados do cliente" << std::endl;
                close(_client_fd);
                break;
            }

            //file.write(buffer, bytes_received);
            bytes_left -= bytes_received;
        }

        //file.close();
        std::cout << "Arquivo " << filename << " recebido e salvo em " << upload_dir << std::endl;
    }
	std::string http_response = "HTTP/1.1 200 OK\r\n\r\n";
    send(_client_fd, http_response.c_str(), http_response.length(), 0);
	
	
	/*char c = {0};
	int bytes_read = 1;
	int sum_bytes = 0;
	std::string buffer;
	while (bytes_read > 0)
	{
		bytes_read = recv(this->_client_fd, &c, 1, 0);
		if(bytes_read <=0)
			break;
		buffer.append(1,c);
		if (buffer.find("\r\n\r\n") != std::string::npos || bytes_read == 0 )
			break;
		sum_bytes += bytes_read;
	}*/
	/*HandleRequest HandleRequest;
	std::string response ;
	HandleRequest.readBuffer(buffer);
	setHandleRequest(HandleRequest);
	checkHost(response);
	write(this->_client_fd, response.c_str(), response.length());
	std::cout << "Message sent to client" << std::endl;*/
	this->closeClientFd();
}

bool fdIsValid(int fd)
{
	return (fcntl(fd, F_GETFD) != -1 || errno != EBADF);
}

void	Socket::closeServerFd(void)
{
	if (fdIsValid(this->_server_fd))
		close(this->_server_fd);
}

void	Socket::closeClientFd(void)
{
	if (fdIsValid(this->_client_fd))
		close(this->_client_fd);
}

std::ostream&	operator<<(std::ostream& o, const Socket& i)
{
	o << "Socket: " << i.getServerFd();
	return o;
}

/*
** cpereira things
*/

void	Socket::readPage(std::string filename, int code, std::string status, std::string& content)
{
	std::ifstream file(filename.c_str());
	std::ifstream fileError(_server.getErrorPages(404).c_str());
	std::stringstream buffer;
	std::stringstream response;
	std::string fileContent;
	if (file.good())
	{
		buffer << file.rdbuf();
		fileContent = buffer.str();
	}
	else
	{
		if (fileError.good())
		{
			buffer << fileError.rdbuf();
			fileContent = buffer.str();
		}
		else
		{
			fileContent = "Page not Found";
			code = 404;
			status = "Not Found";
		}
	}
	response << "HTTP/1.1 " << code << " " << status << "\nContent-Type: text/html\nContent-Length: ";
	response << fileContent.length() << "\n\n" << fileContent;	
	content = response.str();
	file.close();
}

void	Socket::checkHost(std::string& response)
{
	LocationServer locationServer;
	locationServer = _server.getLocationServer(this->_HandleRequest.getBaseURL());
	std::string redirect = locationServer.getRedirect();
	if (!redirect.empty())
	{
		response = "HTTP/1.1 301 Found\r\nLocation: http://" + redirect + "\r\n\r\n";
		return ;
	}
	if (! locationServer.getAllowedMethods(this->_HandleRequest.getMethod()))
	{
		readPage(_server.getErrorPages(403), 403, "Refused", response);
		return ;
	}
	if (locationServer.getRoot() == "")
	{
		readPage(_server.getErrorPages(404), 404, "Not Found", response);
		return ;
	}
	if (this->_server.getServerName() != this->_HandleRequest.getHost())
	{
		std::cout << "ko" << std::endl;
		return ;
	}
	if (_HandleRequest.getEndpoint() == "")
	{
		std::set<std::string> pages = locationServer.getPagesIndex();
		for (std::set<std::string>::iterator it = pages.begin(); it != pages.end(); ++it) 
		{
			std::string page = *it;
			std::string endpoint = locationServer.getRoot() + "/" + page;
			std::ifstream file(endpoint.c_str());
			if (file.good()){
				readPage(endpoint, 200, "Ok", response);
				file.close();
				return ;
			}
		}
		if (locationServer.getAutoIndex())
		{
			autoIndex(locationServer.getRoot());
			std::string endpoint = locationServer.getRoot() + "/autoIndex.html" ;
			readPage(endpoint, 200, "Ok", response);
			remove(endpoint.c_str());
			return ;
		}
		else
			readPage(_server.getErrorPages(404), 404, "Not Found", response);
		return ;
	}
	std::string endpoint = locationServer.getRoot() + "/" + _HandleRequest.getEndpoint();
	std::ifstream file(endpoint.c_str());
	readPage(endpoint, 200, "Ok", response);
	file.close();
	return ;
}

void	Socket::autoIndex(std::string path)
{
	std::ofstream os;
	os.open((path + "/autoIndex.html").c_str());
	DIR *dir;
	struct dirent *ent;
	os << "<html><head><title>Autoindex</title></head><body>" << std::endl;
	os << "<h1>Autoindex</h1>" << std::endl;
	if ((dir = opendir(path.c_str())) != NULL)
	{
		os << "<ul>" << std::endl;
		while ((ent = readdir(dir)) != NULL)
		{
			if (std::string(ent->d_name) == "autoIndex.html")
			{
				continue;
			}
			os << "<li><a href=\"" << ent->d_name << "\">" << ent->d_name << "</a></li>" << std::endl;
		}
		os << "</ul>" << std::endl;
		closedir(dir);
	}
	else
	{
		os << "<p>Erro ao abrir o diretório.</p>" << std::endl;
	}
	os << "</body></html>" << std::endl;
	os.close();
}
