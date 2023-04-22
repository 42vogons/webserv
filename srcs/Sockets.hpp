/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:35:12 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/22 16:00:21 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "Servers.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Poll.hpp"

class Sockets
{
	public:
		Sockets(void);
		Sockets(const Sockets& obj);
		virtual ~Sockets(void);

		Sockets&	operator=(const Sockets& obj);
		void		handleSocketConnections(void);
		void		createVecSocket(void);

	private:
		void	_checkEvent(Poll &poll, size_t index);
		bool	_checkEventMask(short revents);
		void	_connect(Socket *socket);
	
		std::vector<Socket *>			_vecSocket;
		Servers							_servers;
		std::map<std::string, Server>	_serversMap;
		Poll							_poll;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const Sockets& i);

#endif
