/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 22:23:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/20 13:20:28 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include <csignal>

bool globalVariable = true;


static void handle_signal(int signal) {
	restore();
	if (signal == 1)
    		std::cout << "n sei";
	std::cout << "saindo" << std::endl;
	globalVariable = false;
	exit(signal);
}

int main(int argc, char** argv) {
	std::string fileName;
	if (argc > 2) {
		std::cout << "ERROR: invalid args" << std::endl;
		return 1;
	}
	if (argc == 1)
		fileName = "./conf/conf_sample";
	else {
		fileName = argv[1];
		std::ifstream file(fileName.c_str());
		if (!file.good()) {
			std::cout << "ERROR: File not found" << std::endl;
			return 1;
		}
	}
	WebServer webServer;
	std::cout << webServer;
	webServer.loadFile(fileName);
	signal(SIGINT, handle_signal);
	//while (globalVariable)
	webServer.handleSocketConnections(globalVariable);
	return (0);
}
