/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Processss.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/09/03 22:29:35 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <sstream>
#include <dirent.h> // para ler diretorios
#include <iostream>
#include <fstream>
#include "LocationServer.hpp"
#include "HandleRequest.hpp"
#include "Pages.hpp"
#include "Server.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


	void readImage(std::string filename, int code, std::string status, std::string& content, std::string errorPath, std::string extension);
	void readPage(std::string filename, int code, std::string status, std::string& content, std::string errorPath);
	void saveFile(Server server, HandleRequest handlerRequest);
	

	void executeGet(std::string& response, Server server, HandleRequest handleRequest);
	void executeDelete(std::string& response, LocationServer locationServer, HandleRequest handleRequest);
	void executePost(std::string& response, Server server, HandleRequest handleRequest);

	void process(std::string& response, HandleRequest handlerRequest, Server server);


#endif