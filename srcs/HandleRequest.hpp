/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandleRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:47:16 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/12 00:06:50 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLE_REQUEST_HPP
#define HANDLE_REQUEST_HPP

#include "Utils.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>

class HandleRequest {
	public:
		HandleRequest(void);
		HandleRequest(const HandleRequest& obj);
		virtual ~HandleRequest(void);
		HandleRequest&	operator=(const HandleRequest& obj);
		void	readBuffer(std::string buffer, int client_fd);
		void	readBody(std::string buffer, int client_fd);
		void	getHostAndPort(std::string protocol);
		void	setBody(std::string body);
		void	setUser(std::string user);
		void	setCookies(void);
		
		std::string		getField(std::string field);
		std::string		getBody(void);
		std::string		receiveBody(int client_fd);
		std::string		getTypePost(void);
		std::string		getCookie(std::string name);
		std::string		getUser(void);

	private:
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string	_typePost;
		std::map<std::string, std::string> _cookies;
		std::string	_user;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const HandleRequest& i);

#endif
