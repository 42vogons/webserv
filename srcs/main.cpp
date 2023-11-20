/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 22:23:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/20 20:04:52 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include <csignal>

bool monitor = true;

static void handle_signal(int signal) {
	restore();
	std::cout << "terminating program, signal: " << signal << std::endl;
	monitor =false;
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
	webServer.handleSocketConnections(monitor);
	return (0);
}
