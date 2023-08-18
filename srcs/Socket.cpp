/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:26 by anolivei          #+#    #+#             */
/*   Updated: 2023/08/18 18:35:50 by cpereira         ###   ########.fr       */
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
		this->_header= obj._header;
		this->_body = obj._body;
		
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

std::string Socket::receiveInformation(void){
	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	int bytes_received = 0;
	std::string received;

	while ((bytes_received = recv(_client_fd, buffer, sizeof(buffer), 0)) > 0) 
	{
		received.append(buffer, bytes_received);
		if (bytes_received < BUFFER_SIZE)
			break;
	}

	if (bytes_received == -1) {
		close(this->_client_fd);
		return "";
	}
	return received;
}

void	Socket::acceptConnection(void)
{
	this->_client_fd = accept(this->_server_fd, (struct sockaddr *)&this->_address, (socklen_t*)&this->_addrlen);
	if (this->_client_fd == -1)
		throw (AcceptConnectionError()); 
	std::cout << "\033[0;32m\n\n\nNew connection on " << this->_server_fd << "\033[0m" << std::endl;

	this->_header = receiveInformation();
	if (this->findField(_header, "GET") == "")
		this->_body = receiveInformation();
	
	std::string response ;
	HandleRequest HandleRequest;
	HandleRequest.readBuffer(_header);
	setHandleRequest(HandleRequest);
	process(response);

	send(_client_fd, response.c_str(), response.size(), 0);

	close(_client_fd);
	std::cout << "Conexão fechada" << std::endl;
}

std::string	Socket::findField(std::string src, std::string field){
	
	size_t pos = src.find(field);
    if (pos == std::string::npos) {
        return "";
    }
    pos += field.length();
    size_t end_pos = src.find("\r\n", pos);
    if (end_pos == std::string::npos) {
        end_pos = src.find(";", pos);
        if (end_pos == std::string::npos) {
            return "";
        }
    }
	if (src[pos] == '"')
		return src.substr(pos + 1, end_pos - pos - 2);
	return src.substr(pos, end_pos - pos);
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

void	Socket::executeGet(std::string& response){

	LocationServer locationServer;
	locationServer = _server.getLocationServer(this->_HandleRequest.getField("BaseUrl"));
	std::string redirect = locationServer.getField("redirection");
	if (!redirect.empty())
	{
		response = "HTTP/1.1 301 Found\r\nLocation: http://" + redirect + "\r\n\r\n";
		return ;
	}
		
	if (locationServer.getField(this->_HandleRequest.getField("Method")) != "true")
	{
		readPage(_server.getErrorPages(403), 403, "Refused", response);
		return ;
	}

	if (this->_HandleRequest.getField("Endpoint") == "")
	{
		std::set<std::string> pages = locationServer.getPagesIndex();
		for (std::set<std::string>::iterator it = pages.begin(); it != pages.end(); ++it) 
		{
			std::string page = *it;
			std::string endpoint = locationServer.getField("root") + "/" + page;
			std::ifstream file(endpoint.c_str());
			if (file.good()){
				readPage(endpoint, 200, "Ok", response);
				file.close();
				return ;
			}
		}
		
		if (locationServer.getField("autoindex") == "true")
		{
			autoIndex(locationServer.getField("root"));
			std::string endpoint = locationServer.getField("root") + "/autoIndex.html" ;
			readPage(endpoint, 200, "Ok", response);
			remove(endpoint.c_str());
			return ;
		}
		else
			readPage(_server.getErrorPages(404), 404, "Not Found", response);
		return ;
	}
	if (this->_server.getServerName() != this->_HandleRequest.getField("Host"))
	{
		std::cout << "ko" << std::endl;
		return ;
	}
	if (this->_HandleRequest.getField("Endpoint").find(".png") == std::string::npos)
	{
		std::string endpoint = locationServer.getField("root") + "/" + this->_HandleRequest.getField("Endpoint");
		std::ifstream file(endpoint.c_str());
		readPage(endpoint, 200, "Ok", response);
		file.close();	
	}	

}

void	Socket::process(std::string& response)
{
	
	LocationServer locationServer;
	locationServer = _server.getLocationServer(this->_HandleRequest.getField("BaseUrl"));
	std::string method = this->_HandleRequest.getField("Method");

	std::cout << "base" << this->_HandleRequest.getField("BaseUrl") << std::endl;
	std::cout << "locationServer***" << locationServer.getField("GET") << "**" << std::endl;
	std::cout << "base*"<< this->_HandleRequest.getField("BaseUrl") << "*" << std::endl;
	std::cout << "method*"<< method << "*" << std::endl;
	

	
	// melhorar o context de resposta
	// se methodo == post
	if (method == "POST")
		executePost();
	
	if (method == "GET")
	{
		executeGet(response);
		
	}
	return ;
}


void	Socket::executePost(){

	std::cout <<" inicio" << std::endl;

LocationServer locationServer = _server.getLocationServer(this->_HandleRequest.getField("BaseUrl"));

	std::cout << "cgi" << locationServer.getField("cgi") << std::endl;

	std::string cgiPath = "cgi/" + locationServer.getField("cgi");
	const char *cgi = cgiPath.c_str();

	const char *variables = locationServer.getAllCgiParm().c_str();

	std::cout << "para" << locationServer.getAllCgiParm() << std::endl;
	
	
	//char *cgi = "cgi/" + locationServer.getField("cgi");
	
	char *const args[] = { (char*)"python", (char*)cgi, (char*)variables, (char*)"num3=3", NULL };
    char *const env[] = { NULL };

    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");

    }
    else if (child_pid == 0)  // Processo filho
    {
        if (execve("/usr/bin/python3", args, env) == -1)
        {
            perror("execve");

        }
    }
    else  // Processo pai
    {
        // Código para o processo pai, se necessário
    }



	////
	
	/*
	std::string upload_dir = ".";
	LocationServer locationServer = _server.getLocationServer(this->_HandleRequest.getField("BaseUrl"));
	
	std::string fileName = locationServer.getField("upload_path") + "/" + findField(_header, "filename=");
	
	std::cout << "fileName" << fileName << std::endl;
	std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary);
	if (file.is_open()) {
		file.write(_body.data(), _body.size());
		file.close(); 
		std::ifstream infile(fileName.c_str());
		std::string file_contents((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
		if (file_contents == _body) {
			std::cout << "Arquivo salvo corretamente!" << std::endl;
		} else {
			std::cout << "Erro ao salvar o arquivo!" << std::endl;
		}
	} else {
		std::cout << "Erro ao abrir o arquivo para escrita!" << std::endl;
	}
	std::string endpoint = locationServer.getField("root") + "/sucess.html";
	readPage(endpoint, 200, "Ok", response);*/
	
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
