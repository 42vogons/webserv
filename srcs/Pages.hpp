/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pages.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/02 13:03:39 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PAGES_HPP
#define PAGES_HPP

#include <string>
#include <sstream>
#include <dirent.h> // para ler diretorios
#include <iostream>
#include <fstream>
#include "Util.hpp"



	void		createPage(std::string newPage, int code, std::string status, std::string& content);
	void		autoIndex(std::string path);
	void		generatePageFiles(std::string path, std::string& content, std::string pathDir, std::string pathFileErro, std::string baseUrl);
	std::string createResponse(int code, std::string status, std::string fileContent, std::string type);


#endif