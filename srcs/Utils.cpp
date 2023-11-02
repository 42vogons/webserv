/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:38:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/02 18:23:54 by anolivei         ###   ########.fr       */
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
