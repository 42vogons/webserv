/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:38:53 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/15 23:57:52 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Pages.hpp"
#include "LocationServer.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <vector>
#include <sys/wait.h>

std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);
std::vector<std::string> split(const std::string &s, char delimiter);
std::string getContent (std::string filename, int &code, std::string status, std::string errorPath);
void executeCGI(LocationServer locationServer, std::string& response, std::string method, std::string body);
bool directoryExists(const char* path);
bool createDirectory(const char* path);

#endif
