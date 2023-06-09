/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:34:23 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/24 23:44:56 by anolivei         ###   ########.fr       */
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

		Server&			operator=(const Server& obj);

		void			readLine(std::string line);

		void			setPorts(int port);
		void			setServeName(std::string serverName);
		void			setClientMaxBodySize(int clientMaxBodySize);
		void			setErrorPages(int code, std::string page);
		void			setLocationServer(std::string name, LocationServer locationServer);

		std::set<int>	getPorts(void) const;
		std::string		getServerName(void) const;
		int				getClientMaxBodySize(void) const;
		std::string		getErrorPages(int code) const;
		LocationServer	getLocationServer(std::string name) const;
		std::string		getLastLocation(void) const;

	private:
		std::set<int>							_ports;
		std::string								_serverName;
		int										_clientMaxBodySize;
		std::map<int, std::string>				_errorPages;
		std::map<std::string, LocationServer>	_locationServer;
		std::string								_lastLocation;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const Server& i);

#endif
