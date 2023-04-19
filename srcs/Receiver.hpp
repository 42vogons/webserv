/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receiver.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 22:37:17 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/18 22:53:32 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

class Receiver
{
	public:
		Receiver(void);
		Receiver(const Receiver& obj);
		virtual ~Receiver(void);

		Receiver&	operator=(const Receiver& obj);

		void	readBuffer(char buffer[4096]);

		std::string		getHost(void);
		std::string		getMethod(void);
		std::string		getBaseURL(void);
		std::string		getEndpoint(void);

	private:
		std::string	_method;
		std::string	_baseURL;
		std::string	_endpoint;
		std::string	_version;
		std::string	_host;
		int			_contentLength;
		std::string _body;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const Receiver& i);

#endif
