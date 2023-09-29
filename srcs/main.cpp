/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 22:23:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/09/20 23:20:55 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include <csignal>

//WebServer web_server;

static void handle_signal(int signal)
{
	exit(signal);
}

int	main(int argc, char** argv)
{
	std::string fileName;
	
	if (argc > 2)
	{
		std::cout << "Invalid args" << std::endl;
		return 1;
	}
	if (argc == 1)
		fileName = "./conf/conf_sample";
	else
	{
		fileName = argv[1];
		std::ifstream file(fileName.c_str());
		if (!file.good())
		{
			std::cout << "File not found" << std::endl;
			return 1;
		}
	}
	WebServer webServer;
	webServer.loadFile(fileName);
	signal(SIGINT, handle_signal);
	webServer.handleSocketConnections();
	return (0);
}
