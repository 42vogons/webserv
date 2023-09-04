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

class Process
{
    public:
		void readImage(std::string filename, int code, std::string status, std::string& content);
		void readPage(std::string filename, int code, std::string status, std::string& content);
		void saveFile(void);

		void executeGet(std::string &response);
		void executeDelete(std::string &response);
		void executePost(std::string &response);


		void process(std::string &response);

    
};

#endif