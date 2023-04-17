/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 22:23:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/17 00:50:59 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Sockets.hpp"
#include "Server.hpp"
#include "Servers.hpp"
#include <vector>

void	confTests(void)
{
	Server server;
	Servers servers;
	servers.readFile("./conf/conf_sample");
	server = servers.getServer("sample_server_Z1");
	std::set<int> ports = server.getPorts();
	std::cout << "server_name: " << server.getServerName() << std::endl
		<< "301 " << server.getErrorPages(301) << std::endl
		<< "redirect " << server.getLocationServer("/images").getRedirect() << std::endl
		<< "POST " << server.getLocationServer("/images").getAllowedMethods("POST") << std::endl
		<< "GET " << server.getLocationServer("/images").getAllowedMethods("GET") << std::endl
		<< "P2 " << server.getLocationServer("/images").getCgiParm("p2") << std::endl
		<< "Ports: (" << ports.size()<<") ";
	std::set<int>::iterator it;
	for (it = ports.begin(); it != ports.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

int	main(void)
{
	confTests();
	Sockets sockets;
	sockets.handleSocketConnections();
	return (0);
}
