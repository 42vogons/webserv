/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:34:23 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/09 18:28:39 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "LocationConfigServer.hpp"

class ConfigServer
{
	public:
		ConfigServer(void);
		ConfigServer(const ConfigServer& obj);
		virtual ~ConfigServer(void);

		ConfigServer&	operator=(const ConfigServer& obj);

		void	setServeName(std::string serverName);
		void	setErrorPages(int code, std::string page);
		void	setPorts(int port);
		void	setClientMaxBodySize(int clientMaxBodySize);
		void	setLocationConfigServer(std::string, LocationConfigServer);

		std::string				getServerName(void);
		std::string				getErrorPages(int code);
		std::set<int>			getPorts(void);
		int						getClientMaxBodySize(void);
		LocationConfigServer	getLocationConfigServer(std::string name);
		

	private:
		std::string									_serverName;
		std::map<int, std::string>					_errorPages;
		std::set<int>								_ports;
		int											_clientMaxBodySize;
		std::map<std::string, LocationConfigServer>	_locationConfigServer;
	protected:
};

std::ostream&	operator<<(std::ostream& o, const ConfigServer& i);

#endif
