/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersMap.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:34:23 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/16 23:30:33 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_MAP_HPP
#define SERVERS_MAP_HPP

#include <map>
#include "Server.hpp"

class ServersMap
{
	public:
		ServersMap(void);
		ServersMap(const ServersMap& obj);
		virtual ~ServersMap(void);

		ServersMap&	operator=(const ServersMap& obj);

		void	addServer(std::string serverName, Server server);
		void	readFile(std::string fileName);
		int		countTabs(std::string line);

		void							setServer(std::string serverName, Server server);
		Server							getServer(std::string serverName);
		std::map<std::string, Server>	getServersMap(void);

	private:
		std::map<std::string, Server>	_serversMap;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const ServersMap& i);

#endif
