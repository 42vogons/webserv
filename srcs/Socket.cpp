/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:26 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/20 23:42:36 by cpereira         ###   ########.fr       */
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
	return ;
}

Socket& Socket::operator=(const Socket& obj)
{
	if (this != &obj)
	{
		this->_port = obj._port;
		this->_server_fd = obj._server_fd;
		this->_addrlen = obj._addrlen;
		this->_address = obj._address;
		this->_server = obj._server;
		this->_receiver = obj._receiver;
	}
	return (*this);
}

void	Socket::setReceiver(Receiver receiver)
{
	this->_receiver = receiver;
}

Receiver	Socket::getReceiver(void)
{
	return (this->_receiver);
}

void	Socket::createSocketTCP(void)
{
	//address domain AF_INET: socket will be created for the IPv4 protocol
	//type of socket SOCK_STREAM: connection-oriented socket
	//0 the default protocol for the specified socket type should be used
	struct timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(this->_server_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

void	Socket::configSocketAddress(void)
{
	//sets the address length
	this->_addrlen = sizeof(this->_address);
	//fills the _address structure with zeros
	memset(&this->_address, 0, sizeof(this->_address));
	//sets the address family to AF_INET (indicating that the address is IPv4)
	this->_address.sin_family = AF_INET;
	//IP address to INADDR_ANY (indicating that the socket can connect to any available IP address on the local machine)
	this->_address.sin_addr.s_addr = INADDR_ANY;
	//htons -> converts the value of a port from host byte order to network byte order
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
	//queue: size of the pending connection queue, which defines the maximum
	//number of connections that can be queued for the socket before the system
	//starts rejecting new connections.
	int queue = 100;
	listen(this->_server_fd, queue);
}

int	Socket::getServerFd(void) const
{
	return (this->_server_fd);
}

void	Socket::acceptConnection(void)
{
	Receiver receiver;
	std::string response ;
	int client_fd = accept(this->_server_fd, (struct sockaddr *)&this->_address, (socklen_t*)&this->_addrlen);
	std::cout << "\033[0;32m\n\n\nNew connection on " << this->_server_fd << "\033[0m" << std::endl;
	char buffer[4096] = {0};
	read(client_fd, buffer, 4096);
	receiver.readBuffer(buffer);
	setReceiver(receiver);
	checkHost(response);
	write(client_fd, response.c_str(), response.length());
	std::cout << "Message sent to client" << std::endl;
	close(client_fd);
}

void	Socket::readPage(std::string filename, int code, std::string status, std::string& content)
{
	std::ifstream file(filename.c_str());
	std::ifstream fileError(_server.getErrorPages(404).c_str());
	std::stringstream buffer;
	std::stringstream response;
	std::string fileContent;

	// verifica se consegue abrir o arquivo
	if (file.good())
	{
		buffer << file.rdbuf();
		fileContent = buffer.str();
	}
	else
	{
		if (fileError.good()){
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
	// monta a resposta que será enviada para o client
	response << "HTTP/1.1 " << code << " " << status << "\nContent-Type: text/html\nContent-Length: ";
	response << fileContent.length() << "\n\n" << fileContent;	
	content = response.str();
	file.close();
}



void	Socket::checkHost(std::string& response)
{
	LocationServer locationServer;
	locationServer = _server.getLocationServer(this->_receiver.getBaseURL());

	std::string redirect = locationServer.getRedirect();
	if (!redirect.empty()){
		response = "HTTP/1.1 301 Found\r\nLocation: http://" + redirect + "\r\n\r\n";
		return ;
	}

	if (! locationServer.getAllowedMethods(this->_receiver.getMethod()))
	{
		readPage(_server.getErrorPages(403), 403, "Refused", response);
		return ;
	}

	if (locationServer.getRoot() == "")
	{
		readPage(_server.getErrorPages(404), 404, "Not Found", response);
		return ;
	}
	
	if (this->_server.getServerName() != this->_receiver.getHost())
	{
		std::cout << "ko" << std::endl;
		return ;
	}

	// verifica se está na pasta e se endpoint é vazio
	if (_receiver.getEndpoint() == "")
	{
		// percorre todos os getPagesIndex e tenta localizar algum válido
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
		
			
		
			
		
		// verifica se o autoindex está ligado, se tiver ele vai monstar o autoindex se não tiver vai mandar 404
		if (locationServer.getAutoIndex()){
			// cria um indextemporário o qual é apagado depois de printar na tela
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
	
	std::string endpoint = locationServer.getRoot() + "/" + _receiver.getEndpoint();
	std::ifstream file(endpoint.c_str());
	readPage(endpoint, 200, "Ok", response);
	file.close();
	return ;
}

void	Socket::autoIndex(std::string path){

	std::ofstream os;
	os.open((path + "/autoIndex.html").c_str());
	DIR *dir;
	struct dirent *ent;

	os << "<html><head><title>Autoindex</title></head><body>" << std::endl;
	os << "<h1>Autoindex</h1>" << std::endl;

	if ((dir = opendir(path.c_str())) != NULL) {
		os << "<ul>" << std::endl;
		while ((ent = readdir(dir)) != NULL) {
			if (std::string(ent->d_name) == "autoIndex.html") {
				continue;
			}
			os << "<li><a href=\"" << ent->d_name << "\">" << ent->d_name << "</a></li>" << std::endl;
		}
		os << "</ul>" << std::endl;
		closedir(dir);
	} else {
		os << "<p>Erro ao abrir o diretório.</p>" << std::endl;
	}

	os << "</body></html>" << std::endl;
	os.close();
}

std::ostream&	operator<<(std::ostream& o, const Socket& i)
{
	o << "Socket: " << i.getServerFd();
	return o;
}
