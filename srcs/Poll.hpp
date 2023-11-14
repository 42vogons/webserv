/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 14:58:20 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/02 18:29:08 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
#define POLL_HPP

#include "Socket.hpp"

#include <iostream>
#include <vector>
#include <poll.h>

class Poll {
	public:
		Poll(void);
		Poll(const Poll& obj);
		virtual ~Poll(void);
		void	start(std::vector<Socket *> &sockets);
		void	exec(void);
		short	getReturnEvents(size_t index);
		Socket*	getSocket(size_t index);
		Poll&	operator=(const Poll& obj);

	private:
		size_t					_size;
		std::vector<Socket *>	_sockets;
		struct pollfd			*_socketsToWatch;

	protected:
		class PollError : public std::exception {
			public:
				virtual const char* what() const throw() {
					return ("poll error");
				}
		};
};

std::ostream&	operator<<(std::ostream& o, const Poll& i);

#endif
