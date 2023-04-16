/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 22:23:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/15 23:49:40 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Server.hpp"
#include "Servers.hpp"
#include <vector>

void	handleSocketConnections(std::vector<Socket> vecSocket)
{
	while (true)
	{
		fd_set readfds;
		FD_ZERO(&readfds);
		for (std::vector<int>::size_type i = 0; i < vecSocket.size(); i++)
			FD_SET(vecSocket[i].getServerFd(), &readfds);
		int max_fd = vecSocket[0].getServerFd();
		for (std::vector<int>::size_type i = 1; i < vecSocket.size(); ++i)
		{
			if (vecSocket[i].getServerFd() > max_fd)
				max_fd = vecSocket[i].getServerFd();
		}
		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if(activity < 0)
		{
			std::cerr << "Error waiting for events" << std::endl;
			continue;
		}
		for (std::vector<int>::size_type i = 0; i < vecSocket.size(); i++)
		{
			if(FD_ISSET(vecSocket[i].getServerFd(), &readfds))
				vecSocket[i].acceptConnection();
		}
	}
}

int	main()
{
	Server server;
	Servers servers;
	servers.readFile("./conf/conf_sample");
	server = servers.getServer("sample_server_Z1");
	std::set<int> ports = server.getPorts();
	std::cout << "Server Name ==" << server.getServerName() << std::endl
		<< "301" << server.getErrorPages(301) << std::endl
		<< "redirect " << server.getLocationServer("/images").getRedirect() << std::endl
		<< "POST " << server.getLocationServer("/images").getAllowedMethods("POST") << std::endl
		<< "GET " << server.getLocationServer("/images").getAllowedMethods("GET") << std::endl
		<< "P2 " << server.getLocationServer("/images").getCgiParm("p2") << std::endl
		
		<< "Ports :(" << ports.size()<<")";
	std::set<int>::iterator it;
	for (it = ports.begin(); it != ports.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	Socket test(8080);
	Socket test1(8081);
	Socket test2(9000);
	Socket test3(7000);
	std::vector<Socket> vecSocket;
	vecSocket.push_back(test);
	vecSocket.push_back(test1);
	vecSocket.push_back(test2);
	vecSocket.push_back(test3);
	handleSocketConnections(vecSocket);
	return (0);
}
