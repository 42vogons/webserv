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


void	readImage(std::string filename, int code, std::string status, std::string& content, std::string errorPath, std::string extension)
{
	std::stringstream buffer;
	std::string fileContent;
	std::string type;

	if (extension == "png" || extension == "bmp" || extension == "jpeg" || extension == "tiff" || extension == "jpg")
	{
		type = "image/"+ extension;
	}
	else if (extension == "mpeg" || extension == "wav")
		type = "audio/"+extension;
	else if (extension == "pdf") {
		type = "application/"+extension;
	}
	else if (extension == "ico")
	{
		type = "image/png";
		filename = "images/icon.png";
	}
	else
		type = "text/html";
		//filename = "images/desconhecido.jpg";
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
	///std::cout << "content:::" << content << std::endl;
	file.close();
}

void	readPage(std::string filename, int code, std::string status, std::string& content, std::string errorPath)
{
	std::ifstream file(filename.c_str());
	std::ifstream fileError(errorPath.c_str());
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
	////std::cout << "content:::" << content << std::endl;
	file.close();
}

void	executeGet(std::string& response, Server server, HandleRequest handleRequest)
{
	LocationServer locationServer;
	std::string fullPath;

	locationServer = server.getLocationServer(handleRequest.getField("BaseUrl"));

	//essa função abaixo deve ir para a checagem dos config
	std::string root = locationServer.getField("root");
	if (root[0] == '/')
		root.erase(0, 1);  


	
	std::string redirect = locationServer.getField("redirection");
	if (!redirect.empty())
	{
		response = "HTTP/1.1 301 Found\r\nLocation: http://" + redirect + "\r\n\r\n";
		return ;
	}
	if (locationServer.getAllowedMethods("GET") != true)
	{
		readPage(server.getErrorPages(403), 403, "Refused", response, server.getErrorPages(403));
		return ;
	}

	//

	// ajustar essa parte do codigo, pois a imagem nao esta sendo encontrada;git
	fullPath = handleRequest.getField("BaseUrl") + handleRequest.getField("Endpoint");

	
	if (handleRequest.getField("LastPath") == "files.html")
	{

		////std::string filePage =root + "/files.html";
		std::string pathUpload = locationServer.getField("root") + locationServer.getField("upload_path");
		fullPath = root + handleRequest.getField("Endpoint");
		std::string baseUrl = handleRequest.getField("BaseUrl") + handleRequest.getField("Endpoint");
		//std::string path = root + locationServer.getField("upload_path");

		generatePageFiles(pathUpload, response, fullPath, server.getErrorPages(404), baseUrl);
		return;	
	}

	std::string extension;
	// função para pegar extensão
	size_t dotPosition = handleRequest.getField("LastPath").find_last_of(".");
    if (dotPosition != std::string::npos) {
        extension = handleRequest.getField("LastPath").substr(dotPosition + 1);
    }
	else
	{
		extension = "";
	}
	
	if (extension == "") /*handleRequest.getField("Endpoint") == "/")*/
	{
		std::set<std::string> pages = locationServer.getPagesIndex();
		for (std::set<std::string>::iterator it = pages.begin(); it != pages.end(); ++it) 
		{
			std::string page = *it;
			std::string endpoint = fullPath + "/" + page;
			std::ifstream file(endpoint.c_str());
			if (file.good()){
				readPage(endpoint, 200, "Ok", response,server.getErrorPages(404));
				file.close();
				return ;
			}
		}
		if (locationServer.getField("autoindex") == "true")
		{
			autoIndex(fullPath);
			std::string endpoint = fullPath + "/autoIndex.html" ;
			readPage(endpoint, 200, "Ok", response,server.getErrorPages(404));
			remove(endpoint.c_str());
			return ;
		}
		else
			readPage(server.getErrorPages(404), 404, "Not Found", response, server.getErrorPages(404));
		return ;
	}
	if (server.getServerName() != handleRequest.getField("Host"))
	{
		std::cout << "ko" << std::endl;
		return ;
	}
	
	/*std::string endpoint = locationServer.getField("root") + "/" + handleRequest.getField("Endpoint");

	std::string extension;
	// função para pegar extensão
	size_t dotPosition = endpoint.find_last_of(".");
    if (dotPosition != std::string::npos) {
        extension = endpoint.substr(dotPosition + 1);
    }
	else
	{
		extension = "";
	}*/

	
	if (handleRequest.getField("Accept").find("text/html") != std::string::npos && (extension == "html" || extension == "") ) 
	{
		std:: string pathError = server.getErrorPages(404).c_str();
		readPage(fullPath, 200, "Ok", response, pathError);
	} 
	else
	{
		std::cout << "vamos abrir imagem" << std::endl;
		readImage(fullPath, 200, "Ok", response, "images/noPhoto.png", extension);
		
	}
	
	
}


void executeDelete(std::string& response, LocationServer locationServer, HandleRequest handleRequest){
	std::cout << "Vamos deletar" << std::endl;
	//LocationServer locationServer;
	
	//locationServer = _server.getLocationServer("/");
	

	std::string endpoint = locationServer.getField("root") + "/uploads/" + handleRequest.getField("Endpoint");
	
	//if ()

	std::cout << "nome*" << handleRequest.getField("Endpoint") << std::endl;
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

void	saveFile(Server server, HandleRequest handlerRequest)
{
	LocationServer locationServer = server.getLocationServer(handlerRequest.getField("BaseUrl"));
	std::string upload_dir = ".";
	
	std::string filePage = locationServer.getField("root") + "/files.html";
	//std::string path = locationServer.getField("root") + "/uploads";

	// analisar essa linha, está estranha alias está errada.
	// 
	//std::string path = locationServer.getField("upload_path") + "pages/site1/uploads/";
	std::string path = locationServer.getField("root") + locationServer.getField("upload_path");

	std::string body = handlerRequest.getBody();
	std::string fileName = path + "/" + handlerRequest.getField("fileName");
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

void	executePost(std::string& response, Server server, HandleRequest handleRequest)
{
	LocationServer locationServer = server.getLocationServer(handleRequest.getField("BaseUrl"));

	//HandleRequest handleRequest = this->_HandleRequest;
	std::cout << "content type = " << handleRequest.getField("Content-Type") << std::endl;

	
	std::string cgiPath = "cgi/" + locationServer.getField("cgi");
	std::string body = handleRequest.getBody();

	// aqui client_max_body_size 
	
	if (body.empty())
		body = locationServer.getAllCgiParm().c_str();
	
	if (handleRequest.getTypePost() == "File"){
		saveFile(server, handleRequest);
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

void	process(std::string& response, HandleRequest handlerRequest, Server server)
{
	LocationServer locationServer;

	locationServer = server.getLocationServer(handlerRequest.getField("BaseUrl"));

	std::string method = handlerRequest.getField("Method");
	std::cout << "base" << handlerRequest.getField("BaseUrl") << std::endl;
	std::cout << "locationServer***" << locationServer.getField("GET") << "**" << std::endl;
	std::cout << "base*"<< handlerRequest.getField("BaseUrl") << "*" << std::endl;
	std::cout << "method*"<< method << "*" << std::endl;
	std::cout << "client_max_body_size" << server.getClientMaxBodySize() << std::endl;
	

	int bodySize = std::atoi(handlerRequest.getField("Content-Length").c_str());

	std::cout << "bodySize" << bodySize << std::endl;

	// TODO: melhorar o context de resposta se methodo == post
	if ( bodySize > server.getClientMaxBodySize())
	{
		createPage("Body max size error",400,"Bad Request",response);
		return;
	}
	if (method == "POST")
		executePost(response, server, handlerRequest);
	if (method == "GET")
		executeGet(response, server, handlerRequest);
	if (method == "DELETE")
		executeDelete(response, server.getLocationServer("/"), handlerRequest);
	return ;
}
