/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 22:23:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/14 23:09:28 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "Server.hpp"
#include "Servers.hpp"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char*argv[])
{

	if(argc != 2){
		std::cout << "Error: could not open file." << std::endl;
		return 1;
	}

	Server server;

	Servers servers;
	servers.readFile(argv[1]);

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
	for (it = ports.begin(); it != ports.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	

	
	
	
	
	
	


	/*ConfigServer configServer;
	configServer.setServeName("teste");
	configServer.setPorts(8081);
	configServer.setPorts(8080);

	LocationConfigServer locationConfigServer;
	locationConfigServer.setAllowedMethods("POST",true);
	configServer.setLocationConfigServer("teste", locationConfigServer);
	std::cerr << "confLocation" << configServer.getLocationConfigServer("teste") << " ";
	
	

	std::set<int> ports = configServer.getPorts();
	std::set<int>::iterator it;
	std::cerr << "Portas: ";
	for (it = ports.begin(); it != ports.end(); ++it) {
		std::cerr << *it << " ";
	}

	

	
	// Cria o socket TCP para a porta 8080
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	// Configura o endereço do socket
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);
	// Vincula o socket ao endereço
	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	// Espera por conexões de entrada
	listen(server_fd, 3);

	// Cria o socket TCP para a porta 8081
	int server_fd2 = socket(AF_INET, SOCK_STREAM, 0);
	// Configura o endereço do segundo socket
	struct sockaddr_in address2;
	int addrlen2 = sizeof(address2);
	memset(&address2, 0, sizeof(address2));
	address2.sin_family = AF_INET;
	address2.sin_addr.s_addr = INADDR_ANY;
	address2.sin_port = htons(8081);
	// Vincula o segundo socket ao endereço
	bind(server_fd2, (struct sockaddr *)&address2, sizeof(address2));
	// Espera por conexões de entrada no segundo socket
	listen(server_fd2, 3);

	while (true)
	{
		// Espera por uma conexão de entrada em um dos sockets
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(server_fd, &readfds);
		FD_SET(server_fd2, &readfds);
		int max_fd = std::max(server_fd, server_fd2);
		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if(activity < 0)
		{
			std::cerr << "Erro ao aguardar por eventos" << std::endl;
			continue;
		}

		// Se houver uma conexão de entrada no primeiro socket, aceita a conexão e cria um novo socket para o cliente
		if(FD_ISSET(server_fd, &readfds))
		{
			int client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			std::cout << "Nova conexão de entrada em " << server_fd << std::endl;
			// Lê e escreve dados com o cliente
			char buffer[1024] = {0};
			read(client_fd, buffer, 1024);
			std::cout << buffer << std::endl;
			const char* response = "HTTP/1.1 201 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
			write(client_fd, response, strlen(response));
			std::cout << "Mensagem enviada para o cliente" << std::endl;
			// Fecha a conexão com o cliente
			close(client_fd);
		}

		// Se houver uma conexão de entrada no segundo socket, aceita a conexão e cria um novo socket para o cliente
		if(FD_ISSET(server_fd2, &readfds)) {
			int client_fd2 = accept(server_fd2, (struct sockaddr *)&address2, (socklen_t*)&addrlen2);
			std::cout << "Nova conexão de entrada em " << server_fd2 << std::endl;
			// Lê e escreve dados com o cliente
			char buffer[1024] = {0};
			read(client_fd2, buffer, 1024);
			std::cout << buffer << std::endl;
			const char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
			write(client_fd2, response, strlen(response));
			std::cout << "Mensagem enviada para o cliente" << std::endl;
			// Fecha a conexão com o cliente
			close(client_fd2);
		}
	}*/
	return (0);
}
