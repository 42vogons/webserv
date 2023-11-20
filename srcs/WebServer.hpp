/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 20:07:28 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/20 20:09:57 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "ServersMap.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Poll.hpp"

#include <iostream>
#include <vector>
#include <set>
#include <map>

#define WEBSERV "\n\n\n \e[38;2;0;186;188m\
	██╗  ██╗██████╗     ██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗ \n \e[38;2;0;186;188m\
	██║  ██║╚════██╗    ██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║ \n \e[38;2;0;186;188m\
	███████║ █████╔╝    ██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║ \n \e[38;2;0;186;188m\
	╚════██║██╔═══╝     ██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝ \n \e[38;2;0;186;188m\
	     ██║███████╗    ╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝  \n \e[38;2;0;186;188m\
	     ╚═╝╚══════╝     ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝   \n\n\033[0m"

class WebServer {
	public:
		WebServer(void);
		WebServer(const WebServer& obj);
		~WebServer(void);
		WebServer&	operator=(const WebServer& obj);
		void		handleSocketConnections(bool &monitor);
		void		createVecSocket(void);
		void		loadFile(std::string file);
		volatile sig_atomic_t 			signalReceived;

	private:
		void	_checkEvent(Poll &poll, size_t index);
		bool	_checkEventMask(short revents);
		void	_connect(Socket *socket);
		std::vector<Socket *>			_vecSocket;
		ServersMap						_servers;
		std::map<std::string, Server>	_serversMap;
		Poll							_poll;
		std::set<int>			 		_portsAccepted;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const WebServer& i);

#endif
