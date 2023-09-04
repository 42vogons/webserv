/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pages.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/09/03 22:26:21 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PAGES_HPP
#define PAGES_HPP

#include <string>
#include <sstream>
#include <dirent.h> // para ler diretorios
#include <iostream>
#include <fstream>

class Pages
{
    public:
        void    readPage(std::string filename, int code, std::string status, std::string& content);
		void	readImage(std::string filename, int code, std::string status, std::string& content);
		void	createPage(std::string newPage, int code, std::string status, std::string& content);
		void	autoIndex(std::string path);
		void	generatePageFiles(std::string path, std::string& content);

    
    
};

#endif