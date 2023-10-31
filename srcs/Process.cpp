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


void	readImage(std::string filename, int code, std::string status, std::string& content, std::string errorPath, std::string extension) {
	std::stringstream buffer;
	std::string fileContent;
	std::string type;

	if (extension == "png" || extension == "bmp" || extension == "jpeg" || extension == "tiff" || extension == "jpg") {
		type = "image/"+ extension;
	}
	else if (extension == "mpeg" || extension == "wav")
		type = "audio/"+extension;
	else if (extension == "pdf") {
		type = "application/"+extension;
	} 
	else if (extension == "ico") {
		type = "image/png";
		filename = "images/icon.png";
	}
	else
		type = "text/html";
	
	std::ifstream fileError(errorPath.c_str());
	std::ifstream file(filename.c_str());
	if (file.good()) {
		buffer << file.rdbuf();
		fileContent = buffer.str();
	}
	else {
		if (fileError.good()) {
			buffer << fileError.rdbuf();
			fileContent = buffer.str();
			type = "image/png";
		}
		else {
			fileContent = "Page not Found";
			code = 404;
			status = "Not Found";
		}
	}
	content = createResponse(code, status, fileContent, type);
	file.close();
}

void	readPage(std::string filename, int code, std::string status, std::string& content, std::string errorPath) {
	std::ifstream file(filename.c_str());
	std::ifstream fileError(errorPath.c_str());
	std::stringstream buffer;
	std::string fileContent;
	
	if (file.good()) {
		buffer << file.rdbuf();
		fileContent = buffer.str();
	}
	else {
		if (fileError.good()) {
			buffer << fileError.rdbuf();
			fileContent = buffer.str();
		}
		else {
			fileContent = "Page not Found";
			code = 404;
			status = "Not Found";
		}
	}
	content = createResponse(code, status, fileContent, "text/html");
	file.close();
}

void	executeGet(std::string& response, Server server, HandleRequest handleRequest) {
	LocationServer locationServer;
	locationServer = server.getLocationServer(handleRequest.getField("BaseUrl"));
	std::string rootPath2 = locationServer.getField("root");
	if (rootPath2[0] == '/'){
		rootPath2.erase(0, 1);
	}
	std::string basePath = handleRequest.getField("BaseUrl") + handleRequest.getField("Endpoint");
	std::string rootPath = rootPath2 ;//+ handleRequest.getField("Endpoint");
	std::string uploadPath = rootPath2 + locationServer.getField("upload_path");
	std::string redirect = locationServer.getField("redirection");
	if (!redirect.empty()) {
		response = "HTTP/1.1 301 Found\r\nLocation: http://" + redirect + "\r\n\r\n";
		return ;
	}
	if (locationServer.getAllowedMethods("GET") != true) {
		readPage(server.getErrorPages(403), 403, "Refused", response, server.getErrorPages(403));
		return ;
	}

	if (handleRequest.getField("LastPath") == "files.html") {
		generatePageFiles(uploadPath, response, rootPath, server.getErrorPages(404), basePath + locationServer.getField("upload_path")) ;
		return;	
	}

	std::string extension;
	size_t dotPosition = handleRequest.getField("LastPath").find_last_of(".");
    if (dotPosition != std::string::npos) {
        extension = handleRequest.getField("LastPath").substr(dotPosition + 1);
    }
	else {
		extension = "";
	}
	
	if (extension == "") {
		std::set<std::string> pages = locationServer.getPagesIndex();
		for (std::set<std::string>::iterator it = pages.begin(); it != pages.end(); ++it) {
			std::string page = *it;
			std::string endpoint = rootPath + "/" + handleRequest.getField("LastPath") + "/" + page;
			std::ifstream file(endpoint.c_str());
			if (file.good()) {
				readPage(endpoint, 200, "Ok", response,server.getErrorPages(404));
				file.close();
				return ;
			}
		}
		if (locationServer.getField("autoindex") == "true") {
			autoIndex(rootPath);
			std::string endpoint = rootPath + "/autoIndex.html" ;
			readPage(endpoint, 200, "Ok", response,server.getErrorPages(404));
			remove(endpoint.c_str());
			return ;
		}
		else
			readPage(server.getErrorPages(404), 404, "Not Found", response, server.getErrorPages(404));
		return ;
	}
	
	if (handleRequest.getField("Accept").find("text/html") != std::string::npos && (extension == "html" || extension == "")) {
		std:: string pathError = server.getErrorPages(404).c_str();
		std::string endpoint = rootPath + "/" + handleRequest.getField("LastPath");
		readPage(endpoint, 200, "Ok", response, pathError);
	} 
	else {
		std::cout << "vamos abrir imagem" << std::endl;
		readImage(uploadPath +"/"+ handleRequest.getField("LastPath"), 200, "Ok", response, "images/noPhoto.png", extension);
	}
}

void executeDelete(std::string& response, Server server, HandleRequest handleRequest) {
	LocationServer locationServer;
	std::string baseUrl = handleRequest.getField("BaseUrl");
	locationServer = server.getLocationServer(baseUrl);

	if (locationServer.getAllowedMethods("DELETE") != true) {
		readPage(server.getErrorPages(403), 403, "Refused", response, server.getErrorPages(403));
		return ;
	}
	if (baseUrl[0] == '/'){
		baseUrl.erase(0, 1);
	}
	std::string pathFile = baseUrl + "/" + handleRequest.getField("LastPath");
	const char *filename = pathFile.c_str();
	
	if (std::remove(filename) == 0) {
		createPage("Arquivo excluído com sucesso",200, "Ok",response);
        std::printf("Arquivo excluído com sucesso.\n");
    } else {
		createPage("Erro ao excluir o arquivo",500, "Internal Server Error",response);
        std::perror("Erro ao excluir o arquivo");
    }
}

void	saveFile(Server server, HandleRequest handlerRequest, std::string& response) {
	LocationServer locationServer = server.getLocationServer(handlerRequest.getField("BaseUrl"));
	std::string upload_dir = ".";

	std::string rootPath = locationServer.getField("root");
	if (rootPath[0] == '/') {
		rootPath.erase(0, 1);
	}
	
	std::string filePage = rootPath + "/files.html";
	std::string path = rootPath + locationServer.getField("upload_path");
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
	response = "HTTP/1.1 301 Found\r\nLocation: /" + filePage + "\r\n\r\n";
}

void	executePost(std::string& response, Server server, HandleRequest handleRequest) {
	LocationServer locationServer = server.getLocationServer(handleRequest.getField("BaseUrl"));
	if (locationServer.getAllowedMethods("POST") != true) {
		readPage(server.getErrorPages(403), 403, "Refused", response, server.getErrorPages(403));
		return ;
	}
	std::cout << "content type = " << handleRequest.getField("Content-Type") << std::endl;
	std::string cgiPath = "cgi/" + locationServer.getField("cgi");
	std::string body = handleRequest.getBody();

	// aqui client_max_body_size 
	
	if (body.empty())
		body = locationServer.getAllCgiParm().c_str();
	
	if (handleRequest.getTypePost() == "File"){
		saveFile(server, handleRequest, response);
		return;
	}
		
	//std::cout << "body ="<< body << std::endl;
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
		createPage(buffer, 200 ,"OK", response);
		close(pipe_fd[0]);
	}
}

void	process(std::string& response, HandleRequest handlerRequest, Server server) {
	LocationServer locationServer;
	locationServer = server.getLocationServer(handlerRequest.getField("BaseUrl"));
	std::string method = handlerRequest.getField("Method");
	//std::cout << "base" << handlerRequest.getField("BaseUrl") << std::endl;
	//std::cout << "locationServer***" << locationServer.getField("GET") << "**" << std::endl;
	//std::cout << "base*"<< handlerRequest.getField("BaseUrl") << "*" << std::endl;
	//std::cout << "method*"<< method << "*" << std::endl;
	//std::cout << "client_max_body_size" << server.getClientMaxBodySize() << std::endl;
	

	int bodySize = std::atoi(handlerRequest.getField("Content-Length").c_str());

	//std::cout << "bodySize" << bodySize << std::endl;

	// TODO: melhorar o context de resposta se methodo == post
	if ( bodySize > server.getClientMaxBodySize())
	{
		createPage("Body max size error",400,"Bad Request",response);
		return;
	}
	std::set<int>::iterator it = server.getPorts().find(atoi(handlerRequest.getField("Ports").c_str()));
	if (it == server.getPorts().end()){
		readPage(server.getErrorPages(403), 403, "Refused", response, server.getErrorPages(403));
	} else {
		if (method == "POST")
		executePost(response, server, handlerRequest);
		if (method == "GET")
		executeGet(response, server, handlerRequest);
		if (method == "DELETE")
		executeDelete(response, server, handlerRequest);
	}
	return ;
}
