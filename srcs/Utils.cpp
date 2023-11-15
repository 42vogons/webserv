/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:38:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/15 01:38:15 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

bool directoryExists(const char* path) {
	struct stat info;
	return (stat(path, &info) == 0 && S_ISDIR(info.st_mode));
}

bool createDirectory(const char* path) {
	if (mkdir(path, 0777) == 0) {
		std::cout << "Folder created with success" << std::endl;
		return (true);
	}
	std::cout << "Error creating the folder" << std::endl;
	return (false);
}

std::string replaceAll(const std::string& str, const std::string& from, const std::string& to) {
	std::string result = str;
	std::string::size_type pos = 0;
	
	while ((pos = result.find(from, pos)) != std::string::npos) {
		result.replace(pos, from.length(), to);
		pos += to.length();
	}
	return result;
}

std::vector<std::string> split(const std::string &s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

std::string getContent (std::string filename, int &code, std::string status, std::string errorPath) {
	std::ifstream file(filename.c_str());
	std::ifstream fileError(errorPath.c_str());
	std::stringstream buffer;
	std::string fileContent;
	std::string type;

	
	

	if (file.good()) {
		buffer << file.rdbuf();
		fileContent = buffer.str();
	}
	else {
		if (fileError.good()) {
			if (errorPath.find("autoIndex.html") == std::string::npos){
				code = 404;
				status = "Not Found";
			} 
			buffer << fileError.rdbuf();
			fileContent = buffer.str();
		}
		else {
			fileContent = "Page not Found";
			code = 404;
			status = "Not Found";
		}
	}
	file.close();
	return fileContent;
}

void executeCGI(LocationServer locationServer, std::string& response, std::string method, std::string body)
{
	std::string cgiPath = "cgi/" + locationServer.getField("cgi");
	std::ifstream fileCGI(cgiPath.c_str());
	if (!fileCGI.good()) {
		createPage("CGI file not found",200, "Ok",response);
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
		//std::string aaa = "cgi/random/random.go";
		if (method == "POST"){
			const char *args[] = { "python", cgiPath.c_str(), body.c_str(), NULL, NULL };
			const char *env[] = { NULL };
			execve("/usr/bin/python3", const_cast<char* const*>(args), const_cast<char* const*>(env));	
		} else {
			const char *args[] = {cgiPath.c_str(), body.c_str(), NULL};
			const char *env[] = { NULL };
			execve(cgiPath.c_str(),const_cast<char* const*>(args), const_cast<char* const*>(env));
		}
		
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
