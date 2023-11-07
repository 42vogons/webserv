/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Process.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:38:37 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/03 17:37:37 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Process.hpp"

void readImage(std::string filename, int code, std::string status, std::string& content, std::string errorPath, std::string extension) {
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
	// colocar uma logica aqui para quando não for esses tipos de arquivo, ele baixa o arquivo		
	
	
	fileContent = getContent(filename, code, status, errorPath);
	content = createResponse(code, status, fileContent, type);
}

void readPage(std::string filename, int code, std::string status, std::string& content, std::string errorPath) {
	std::string fileContent;
	fileContent = getContent(filename, code, status, errorPath);
	content = createResponse(code, status, fileContent, "text/html");
}

void executeGet(std::string& response, Server server, HandleRequest handleRequest) {
	LocationServer locationServer;
	locationServer = server.getLocationServer(handleRequest.getField("BaseUrl"));
	std::string rootPath = locationServer.getField("root");
	if (rootPath[0] == '/') {
		rootPath.erase(0, 1);
	}
	std::string basePath = handleRequest.getField("BaseUrl") + handleRequest.getField("Endpoint");
	std::string uploadPath = rootPath + locationServer.getField("upload_path");
	std::string redirect = locationServer.getField("redirection");
	if (!redirect.empty()) {
		response = "HTTP/1.1 301 Found\r\nLocation: http://" + redirect + "\r\n\r\n";
		return ;
	}
	if (locationServer.getAllowedMethods("GET") != true) {
		readPage(server.getErrorPages(405), 405, "Refused", response, server.getErrorPages(405));
		return ;
	}
	if (handleRequest.getField("LastPath") == "files.html") {
		generatePageFiles(uploadPath, response, rootPath, server.getErrorPages(404)) ;
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
		readImage(uploadPath +"/"+ handleRequest.getField("LastPath"), 200, "Ok", response, "", extension);
	}
}

void executeDelete(std::string& response, Server server, HandleRequest handleRequest) {
	LocationServer locationServer;
	std::string baseUrl = handleRequest.getField("BaseUrl");
	locationServer = server.getLocationServer(baseUrl);
	if (locationServer.getAllowedMethods("DELETE") != true) {
		readPage(server.getErrorPages(405), 405, "Refused", response, server.getErrorPages(405));
		return ;
	}
	if (baseUrl[0] == '/') {
		baseUrl.erase(0, 1);
	}
	std::string pathFile = baseUrl + "/" + handleRequest.getField("LastPath");
	const char *filename = pathFile.c_str();
	if (std::remove(filename) == 0) {
		createPage("File successfully deleted.",200, "Ok",response);
		std::printf("File successfully deleted.\n");
	} else {
		createPage("File deletion unsuccessful. File Not Found",404, "File Not Found",response);
	}
}

void saveFile(Server server, HandleRequest handlerRequest, std::string& response) {
	LocationServer locationServer = server.getLocationServer(handlerRequest.getField("BaseUrl"));
	std::string upload_dir = ".";
	std::string rootPath = locationServer.getField("root");
	if (rootPath[0] == '/') {
		rootPath.erase(0, 1);
	}
	std::string filePage = rootPath + "/files.html";
	std::string path = rootPath + locationServer.getField("upload_path");
	if (!directoryExists(path.c_str())) {
		if (!createDirectory(path.c_str()))
			std::cout << "Error creating directory" << std::endl;
	}
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

void executePost(std::string& response, Server server, HandleRequest handleRequest) {
	LocationServer locationServer = server.getLocationServer(handleRequest.getField("BaseUrl"));
	if (locationServer.getAllowedMethods("POST") != true) {
		readPage(server.getErrorPages(405), 405, "Refused", response, server.getErrorPages(405));
		return ;
	}
	std::string body = handleRequest.getBody();
	if (body.empty())
		body = locationServer.getAllCgiParm().c_str();
	if (handleRequest.getTypePost() == "File") {
		saveFile(server, handleRequest, response);
		return;
	}
	std::string cgiPath = "cgi/" + locationServer.getField("cgi");
	std::string cgiPass = locationServer.getField("cgi_pass");
	std::ifstream fileCGI(cgiPath.c_str());
	if (!fileCGI.good() || cgiPass != "pass") {
		createPage("CGI file not found or not allowed",200, "Ok",response);
		return;
	}
	locationServer.getAllCgiParm();
		int pipe_fd[2];
	if (pipe(pipe_fd) == -1) {
		perror("pipe");
		return ;
	}
	pid_t child_pid = fork();
	if (child_pid == -1) {
		perror("fork");
	}
	else if (child_pid == 0) {
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		const char *args[] = { "python", cgiPath.c_str(), body.c_str(), NULL, NULL };
		const char *env[] = { NULL };
		execve("/usr/bin/python3", const_cast<char* const*>(args), const_cast<char* const*>(env));
		perror("execve");
		_exit(1);
	}
	else {
		close(pipe_fd[1]);
		char buffer[4096];
		ssize_t bytesRead;
		while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
			write(STDOUT_FILENO, buffer, bytesRead);
		}
		int status;
		waitpid(child_pid, &status, 0);
		createPage(buffer, 200 ,"OK", response);
		close(pipe_fd[0]);
	}
}

void process(std::string& response, HandleRequest handlerRequest, Server server) {
	LocationServer locationServer;
	locationServer = server.getLocationServer(handlerRequest.getField("BaseUrl"));
	std::string method = handlerRequest.getField("Method");
	std::string version = handlerRequest.getField("Version");
	if (version.find("1.1") == std::string::npos) {
		createPage("Version is not HTTP 1.1",400,"Bad Request",response);
		return;
	}
	int bodySize = std::atoi(handlerRequest.getField("Content-Length").c_str());
	if ( bodySize > server.getClientMaxBodySize()) {
		readPage(server.getErrorPages(413), 413, "Payload Too Large", response, server.getErrorPages(413));
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
