/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:26 by anolivei          #+#    #+#             */
/*   Updated: 2023/08/31 22:26:40 by cpereira         ###   ########.fr       */
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
	this->_client_fd = accept(this->_server_fd, (struct sockaddr *)&this->_address, (socklen_t*)&this->_addrlen);
	if (this->_client_fd == -1)
		throw (AcceptConnectionError()); 
	std::cout << "\033[0;32m\n\n\nNew connection on " << this->_server_fd << "\033[0m" << std::endl;
	
	std::string response ;
	HandleRequest handleRequest;
	std::string header = handleRequest.receiveInformation(this->_client_fd);
	std::cout <<" receive----------" << header << " receive----------" << std::endl;
	handleRequest.readBuffer(header, this->_client_fd);
	//handleRequest.receiveInformation(this->_client_fd);
	
	/*if (this->findField(header, "GET") == ""){
		HandleRequest.setBody(receiveInformation());
		std::cout << "new body ----------------" << std::endl;
		std::cout << HandleRequest.getBody();
		std::cout << "fim new body ----------------" << std::endl;
	}*/
	setHandleRequest(handleRequest);
	process(response);
	send(_client_fd, response.c_str(), response.size(), 0);
	close(_client_fd);
	std::cout << "Closed connection" << std::endl;
}

std::string	Socket::findField(std::string src, std::string field)
{
	size_t pos = src.find(field);
	if (pos == std::string::npos)
		return "";
	pos += field.length();
	size_t end_pos = src.find("\r\n", pos);
	if (end_pos == std::string::npos)
	{
		end_pos = src.find(";", pos);
		if (end_pos == std::string::npos)
			return "";
	}
	if (src[pos] == '"')
		return src.substr(pos + 1, end_pos - pos - 2);
	return src.substr(pos, end_pos - pos);
}

bool	fdIsValid(int fd)
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

std::string createResponse(int code, std::string status, std::string fileContent, std::string type)
{
	std::stringstream response;
	response
		<< "HTTP/1.1 " << code << " " << status << std::endl
		<< "Content-Type: " << type << std::endl
		<< "Content-Length: " << fileContent.length() << std::endl
		<< std::endl
		<< fileContent;
	return response.str();
}

void	Socket::readImage(std::string filename, int code, std::string status, std::string& content)
{
	std::stringstream buffer;
	std::string fileContent;
	std::string extension;
	std::string errorPath;
	std::string type;


	// função para pegar extensão
	size_t dotPosition = filename.find_last_of(".");
    if (dotPosition != std::string::npos) {
        extension = filename.substr(dotPosition + 1);
    }
	else
	{
		extension = "";
	}

	
	
	if (extension == "png" || extension == "bmp" || extension == "jpeg" || extension == "tiff")
	{
		errorPath = "images/noPhoto.png";
		type = "image/"+ extension;
	}
	else if (this->_HandleRequest.getField("Accept").find("text/html") != std::string::npos)
	{
		
		type = "text/html";
		errorPath= _server.getErrorPages(404).c_str();
	}
	else
		filename = "images/desconhecido.jpg";
		//errorPath = 


	std::ifstream fileError(errorPath.c_str());
	std::ifstream file(filename.c_str());
	// verificar se o tipo de arquivo é imagem se 
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
	content = createResponse(code, status, fileContent, type);
	std::cout << "content:::" << content << std::endl;
	file.close();
}

void	Socket::readPage(std::string filename, int code, std::string status, std::string& content)
{
	std::ifstream file(filename.c_str());
	std::ifstream fileError(_server.getErrorPages(404).c_str());
	std::stringstream buffer;
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
	content = createResponse(code, status, fileContent, "text/html");
	std::cout << "content:::" << content << std::endl;
	file.close();
}

void	Socket::executeGet(std::string& response)
{
	LocationServer locationServer;
	locationServer = _server.getLocationServer(this->_HandleRequest.getField("BaseUrl"));
	std::string redirect = locationServer.getField("redirection");
	if (!redirect.empty())
	{
		response = "HTTP/1.1 301 Found\r\nLocation: http://" + redirect + "\r\n\r\n";
		return ;
	}
	if (locationServer.getAllowedMethods("GET") != true)
	{
		readPage(_server.getErrorPages(403), 403, "Refused", response);
		return ;
	}
	//std::string endpoint2 = this->_HandleRequest.getField("Endpoint");


	if (this->_HandleRequest.getField("Endpoint") == "files.html")
	{
		generatePageFiles("pages/site1/uploads", response);
		//createPage(generatePageFiles("pages/site1/uploads"),200, "OK", response);
		return;	
	}
	
	if (this->_HandleRequest.getField("Endpoint") == "/")
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
	
	std::string endpoint = locationServer.getField("root") + "/" + this->_HandleRequest.getField("Endpoint");
	std::ifstream file(endpoint.c_str());
	readImage(endpoint, 200, "Ok", response);
	file.close();
	
	/*if (this->_HandleRequest.getField("Accept").find("text/html") != std::string::npos)
	{
		std::string endpoint = locationServer.getField("root") + "/" + this->_HandleRequest.getField("Endpoint");
		std::ifstream file(endpoint.c_str());
		readPage(endpoint, 200, "Ok", response);
		file.close();	
	} 
	else
	{
		std::cout << "vamos abrir imagem" << std::endl;
		std::string endpoint = locationServer.getField("root") + "/" + this->_HandleRequest.getField("Endpoint");
		std::ifstream file(endpoint.c_str());
		readImage(endpoint, 200, "Ok", response);
		file.close();	
	}*/
	
	
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

	// TODO: melhorar o context de resposta se methodo == post
	if (method == "POST")
		executePost(response);
	if (method == "GET")
		executeGet(response);
	if (method == "DELETE")
		executeDelete(response);
	return ;
}

void Socket::executeDelete(std::string& response){
	std::cout << "Vamos deletar" << std::endl;
	LocationServer locationServer;
	locationServer = _server.getLocationServer("/");
	

	std::string endpoint = locationServer.getField("root") + "/uploads/" + this->_HandleRequest.getField("Endpoint");
	
	//if ()

	std::cout << "nome*" << this->_HandleRequest.getField("Endpoint") << std::endl;
 	//const char *filename = this->_HandleRequest.getField("Endpoint").c_str();
	const char *filename = endpoint.c_str();

	if (std::remove(filename) == 0) {
		createPage("Arquivo excluído com sucesso",200, "Ok",response);
        std::printf("Arquivo excluído com sucesso.\n");
    } else {
		createPage("Erro ao excluir o arquivo",500, "Internal Server Error",response);
        std::perror("Erro ao excluir o arquivo");
    }

	std::cout << "Deletamos" << std::endl;
	//response = 1;
}



void Socket::createPage(std::string newPage, int code, std::string status, std::string& content)
{
	std::stringstream buffer;
	std::string fileContent;
	fileContent = newPage;
	content = createResponse(code, status, fileContent, "text/html");
	std::cout << "content:::" << content << std::endl;
}

void	Socket::saveFile()
{
	LocationServer locationServer = _server.getLocationServer(this->_HandleRequest.getField("BaseUrl"));
	std::string upload_dir = ".";
	std::string path = locationServer.getField("upload_path") + "pages/site1/uploads/";
	std::string body = this->_HandleRequest.getBody();
	std::string fileName = path + this->_HandleRequest.getField("fileName");
	std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary);
	if (file.is_open()) {
		file.write(body.data(), body.size());
		file.close(); 
		std::ifstream infile(fileName.c_str());
		std::string file_contents((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
		std::cout << "File saved successfully!" << std::endl;
	}
	else
		std::cout << "Error opening the file for writing!" << std::endl;
}

void	Socket::executePost(std::string& response)
{
	LocationServer locationServer = _server.getLocationServer(this->_HandleRequest.getField("BaseUrl"));

	HandleRequest handleRequest = this->_HandleRequest;
	std::cout << "content type = " << handleRequest.getField("Content-Type") << std::endl;

	
	std::string cgiPath = "cgi/" + locationServer.getField("cgi");
	std::string body = this->_HandleRequest.getBody();
	if (body.empty())
		body = locationServer.getAllCgiParm().c_str();
	
	if (handleRequest.getTypePost() == "File"){
		saveFile();
		createPage("Salvo com sucesso", 200 ,"OK", response);
		return;
		// colocar que foi salvo com sucesso!!
	}
		
	std::cout << "body ="<< body << std::endl;
	locationServer.getAllCgiParm();
	// Cria um pipe para capturar a saída do processo filho
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1) {
		perror("pipe");
		return ; //1
	}
	pid_t child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
	}
	else if (child_pid == 0) // Processo filho
	{
		// Fecha o descritor de leitura do pipe, pois o filho irá escrever nele
		close(pipe_fd[0]);
		// Redireciona a saída padrão para o descritor de escrita do pipe
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		// Executa o script Python
		const char *args[] = { "python", cgiPath.c_str(), body.c_str(), NULL, NULL };
		const char *env[] = { NULL };
		execve("/usr/bin/python3", const_cast<char* const*>(args), const_cast<char* const*>(env));
		// Se execve() falhar, o erro será exibido na saída padrão (que agora é o pipe)
		perror("execve");
		_exit(1);
	}
	else // Processo pai
	{
		// Fecha o descritor de escrita do pipe, pois o pai irá ler dele
		close(pipe_fd[1]);
		// Lê e exibe a saída do processo filho a partir do pipe
		char buffer[4096];
		ssize_t bytesRead;
		while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
			// Escreve os dados lidos na saída padrão (ou faça o que quiser)
			write(STDOUT_FILENO, buffer, bytesRead);
		}
		// Aguarda o término do processo filho
		int status;
		waitpid(child_pid, &status, 0);
		std::cout << "Page --------" << std::endl;
		std::cout << buffer << std::endl;
		std::cout << "Page --------" << std::endl;
		createPage(buffer, 200 ,"OK", response);
		//readPage(buffer, 200, "OK", response);
		// Fecha o descritor de leitura do pipe
		close(pipe_fd[0]);
		// Código para o processo pai, se necessário
	}
}

void	Socket::autoIndex(std::string path)
{
	std::ofstream os;
	os.open((path + "/autoIndex.html").c_str());
	DIR *dir;
	struct dirent *ent;
	os
		<< "<html><head><title>Autoindex</title></head><body>" << std::endl
		<< "<h1>Autoindex</h1>" << std::endl;
	if ((dir = opendir(path.c_str())) != NULL)
	{
		os << "<ul>" << std::endl;
		while ((ent = readdir(dir)) != NULL)
		{
			if (std::string(ent->d_name) == "autoIndex.html")
				continue;
			os << "<li><a href=\"" << ent->d_name << "\">" << ent->d_name << "</a></li>" << std::endl;
		}
		os << "</ul>" << std::endl;
		closedir(dir);
	}
	else
		os << "<p>Error opening the directory.</p>" << std::endl;
	os << "</body></html>" << std::endl;
	os.close();
}

void	Socket::generatePageFiles(std::string path, std::string& content)
{
	///
	std::string filePage =  "pages/site1/files.html";
	std::ifstream file(filePage.c_str());
	std::ifstream fileError(_server.getErrorPages(404).c_str());
	std::stringstream buffer;
	std::string fileContent;
	int code = 200;
	std::string status = "Ok";


	DIR *dir;
	struct dirent *ent;
	int col = 0;
	std::string html;
	html = "<table border = 1><tr><td colspan=5>Files</td></tr><tr>";
		
	if ((dir = opendir(path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string fileName = ent->d_name;
			std::string filePath = path +"/" +fileName;
			
			if (fileName == (".") || fileName == (".."))
				continue;

			if (col == 4)
			{
				html += "</tr><tr>";
				col = -1;
			}
			col ++;
			
			html += "<td>";
			html += "<div class='image-container'>";
			html += "<a href='" + filePath+"' target='_blank'>";
			html += "<img src='" + filePath + "' alt='"+ fileName+"' height='100' width='100'></a><br>";
			html += " <span class=\"delete-icon\" onclick=\"deleteImage('"+fileName+"')\"><img src=\"images/lixeira.png\" height=\"20\" width=\"20\"></span></div></td>";
		}
		closedir(dir);
	}
	else
		html +="<p>Error opening the directory.</p>";
	html += "</tr></table>";


	

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
	std::string text = "{{pages}}";
	size_t pos = fileContent.find(text);
	
	fileContent.replace(pos, text.length(), html);
	
	content = createResponse(code, status, fileContent, "text/html");
	std::cout << "content:::" << content << std::endl;
	file.close();
	///
	//html += "</body></html>";
	//return html;
}

