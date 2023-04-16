/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:34:23 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/16 01:21:29 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "LocationServer.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

class Server
{
	public:
		Server(void);
		Server(std::string name);
		Server(const Server& obj);
		virtual ~Server(void);

		Server&	operator=(const Server& obj);

		void	readLine(std::string line);
		void	setServeName(std::string serverName);
		void	setErrorPages(int code, std::string page);
		void	setPorts(int port);
		void	setClientMaxBodySize(int clientMaxBodySize);
		void	setLocationServer(std::string name, LocationServer locationServer);

		std::string				getServerName(void);
		std::string				getErrorPages(int code);
		std::set<int>			getPorts(void) const;
		int						getClientMaxBodySize(void);
		LocationServer			getLocationServer(std::string name);
		std::string				getLastLocation(void);

	private:
		std::string									_serverName;
		std::map<int, std::string>					_errorPages;
		std::set<int>								_ports;
		int											_clientMaxBodySize;
		std::map<std::string, LocationServer>		_locationServer;
		std::string									_lastLocation;
	protected:
};

std::ostream&	operator<<(std::ostream& o, const Server& i);

#endif
