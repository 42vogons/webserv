/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 22:23:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/14 19:47:05 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
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
	Socket test(8080);
	std::cout << test << std::endl;
	Socket test1(8081);
	std::cout << test1 << std::endl;
	Socket test2(9000);
	std::cout << test2 << std::endl;
	Socket test3(7000);
	std::cout << test3 << std::endl;
	std::vector<Socket> vecSocket;
	vecSocket.push_back(test);
	vecSocket.push_back(test1);
	vecSocket.push_back(test2);
	vecSocket.push_back(test3);
	handleSocketConnections(vecSocket);
}
