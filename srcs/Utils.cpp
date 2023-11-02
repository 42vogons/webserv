/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:38:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/02 16:39:58 by anolivei         ###   ########.fr       */
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
