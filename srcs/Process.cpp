/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Process.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:26 by anolivei          #+#    #+#             */
/*   Updated: 2023/09/03 22:24:21 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Process.hpp"

void	Process::readImage(std::string filename, int code, std::string status, std::string& content)
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

void	Process::readPage(std::string filename, int code, std::string status, std::string& content)
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

void	Process::executeGet(std::string& response)
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

void	Process::process(std::string& response)
{
	LocationServer locationServer;
	locationServer = _server.getLocationServer(this->_HandleRequest.getField("BaseUrl"));
	std::string method = this->_HandleRequest.getField("Method");
	std::cout << "base" << this->_HandleRequest.getField("BaseUrl") << std::endl;
	std::cout << "locationServer***" << locationServer.getField("GET") << "**" << std::endl;
	std::cout << "base*"<< this->_HandleRequest.getField("BaseUrl") << "*" << std::endl;
	std::cout << "method*"<< method << "*" << std::endl;
	std::cout << "client_max_body_size" << _server.getClientMaxBodySize() << std::endl;
	

	int bodySize = std::atoi(this->_HandleRequest.getField("Content-Length").c_str());
	// TODO: melhorar o context de resposta se methodo == post
	if ( bodySize > _server.getClientMaxBodySize())
	{
		createPage("Body max size error",400,"Bad Request",response);
		return;
	}
	if (method == "POST")
		executePost(response);
	if (method == "GET")
		executeGet(response);
	if (method == "DELETE")
		executeDelete(response);
	return ;
}

void Process::executeDelete(std::string& response){
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





void	Process::saveFile()
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

void	Process::executePost(std::string& response)
{
	LocationServer locationServer = _server.getLocationServer(this->_HandleRequest.getField("BaseUrl"));

	HandleRequest handleRequest = this->_HandleRequest;
	std::cout << "content type = " << handleRequest.getField("Content-Type") << std::endl;

	
	std::string cgiPath = "cgi/" + locationServer.getField("cgi");
	std::string body = this->_HandleRequest.getBody();

	// aqui client_max_body_size 
	
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
