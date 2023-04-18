/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:34:23 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/16 23:30:33 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
#define SERVERS_HPP

#include <map>
#include "Server.hpp"

class Servers
{
	public:
		Servers(void);
		Servers(const Servers& obj);
		virtual ~Servers(void);

		Servers&	operator=(const Servers& obj);

		void	addServer(std::string serverName, Server server);
		void	readFile(std::string fileName);
		int		countTabs(std::string line);
		Server	getServer(std::string serverName);
		void	setServer(std::string serverName, Server server);
		std::map<std::string, Server>	getServers(void);

	private:
		std::map<std::string, Server>	_servers;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const Servers& i);

#endif
