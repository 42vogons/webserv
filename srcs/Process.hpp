/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Process.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 18:11:27 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/15 23:18:02 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "LocationServer.hpp"
#include "HandleRequest.hpp"
#include "Utils.hpp"
#include "Pages.hpp"
#include "Server.hpp"

#include <string>
#include <sstream>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>


void readImage(std::string filename, int code, std::string status, std::string& content, std::string errorPath, std::string extension);
void readPage(std::string filename, int code, std::string status, std::string& content, std::string errorPath);
void executeGet(std::string& response, Server server, HandleRequest handleRequest);
void executeDelete(std::string& response, Server server, HandleRequest handleRequest);
void executePost(std::string& response, Server server, HandleRequest handleRequest);
void process(std::string& response, HandleRequest handlerRequest, Server server);
void saveFile(Server server, HandleRequest handlerRequest, std::string& response);

#endif
