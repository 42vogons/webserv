/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 22:37:17 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/24 23:11:41 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLE_REQUEST_HPP
#define HANDLE_REQUEST_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <map>

class HandleRequest
{
	public:
		HandleRequest(void);
		HandleRequest(const HandleRequest& obj);
		virtual ~HandleRequest(void);

		HandleRequest&	operator=(const HandleRequest& obj);

		void	readBuffer(std::string buffer);

		std::string		getField(std::string field);

	private:
		std::map<std::string, std::string> _headers;
	protected:
};

std::ostream&	operator<<(std::ostream& o, const HandleRequest& i);

#endif
