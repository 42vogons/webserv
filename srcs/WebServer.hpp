/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:35:12 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/24 22:50:33 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "ServersMap.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Poll.hpp"

class WebServer
{
	public:
		WebServer(void);
		WebServer(const WebServer& obj);
		virtual ~WebServer(void);

		WebServer&	operator=(const WebServer& obj);
		void		handleSocketConnections(void);
		void		createVecSocket(void);
		void		loadFile(std::string file);
		

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
