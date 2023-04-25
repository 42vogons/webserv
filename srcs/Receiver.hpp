/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Receiver.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 22:37:17 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/24 23:11:41 by cpereira         ###   ########.fr       */
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

		void	readBuffer(std::string buffer);

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
		std::string	_body;
		std::string	_contentType;
		// file
		std::string	_boundary;
		std::string	_contentDisposition;
		std::string	_name;
		std::string	_fileName;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const Receiver& i);

#endif
