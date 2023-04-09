/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:39:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/09 17:40:15 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>

class Socket
{
	public:
		Socket(void);
		Socket(const Socket& obj);
		virtual ~Socket(void);

		Socket&	operator=(const Socket& obj);

	private:

	protected:
};

std::ostream&	operator<<(std::ostream& o, const Socket& i);

#endif
