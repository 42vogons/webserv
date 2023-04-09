/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:34:23 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/09 17:40:59 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <LocationConfigServer.hpp>

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

		std::string					getServerName(void);
		std::map<int, std::string>	getErrorPorts(void);
		std::set<int>				getPorts(void);
		int							getClientMaxBodySize(void);

	private:
		std::string					_serverName;
		std::map<int, std::string>	_errorPages;
		std::set<int>				_ports;
		int							_clientMaxBodySize;
		LocationConfigServer		_locationConfigServer;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const ConfigServer& i);

#endif
