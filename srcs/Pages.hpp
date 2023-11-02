/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pages.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/02 16:59:32 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PAGES_HPP
#define PAGES_HPP

#include <string>
#include <sstream>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include "Utils.hpp"

void		createPage(std::string newPage, int code, std::string status, std::string& content);
void		autoIndex(std::string path);
void		generatePageFiles(std::string path, std::string& content, std::string pathDir, std::string pathFileErro);
std::string createResponse(int code, std::string status, std::string fileContent, std::string type);

#endif
