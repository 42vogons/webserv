/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationServer.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:49:53 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/02 18:27:40 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONFIG_SERVER_HPP
#define LOCATION_CONFIG_SERVER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>

class LocationServer {
	public:
		LocationServer(void);
		LocationServer(const LocationServer& obj);
		virtual ~LocationServer(void);
		LocationServer&	operator=(const LocationServer& obj);
		void	setAllowedMethods(std::string name, bool status);
		void	setPagesIndex(std::string page);
		void	setCgiParam(std::string key, std::string value);
		std::string	getField(std::string field);
		void	readLine(std::string line);
		bool					getAllowedMethods(std::string methods);
		std::set<std::string>	getPagesIndex(void);
		std::string				getCgiParm(std::string param);
		std::string				getAllCgiParm(void);

	private:
		std::map<std::string, bool>			_allowedMethods;
		std::set<std::string>				_pagesIndex;
		std::map<std::string, std::string>	_cgiParam;
		std::map<std::string, std::string>	_variables;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const LocationServer& i);

#endif
