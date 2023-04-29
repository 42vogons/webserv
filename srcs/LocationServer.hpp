/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationServer.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:30:32 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/09 18:46:24 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONFIG_SERVER_HPP
#define LOCATION_CONFIG_SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <sstream>

class LocationServer
{
	public:
		LocationServer(void);
		LocationServer(const LocationServer& obj);
		virtual ~LocationServer(void);

		LocationServer&	operator=(const LocationServer& obj);

		void	setAutoIndex(bool autoIndex);
		void	setRoot(std::string root);
		void	setAllowedMethods(std::string name, bool status);
		void	setPagesIndex(std::string page);
		void	setCgiPass(std::string cgiPass);
		void	setRedirect(std::string redirect);
		void	setUpdatePath(std::string redirect);
		void	setCgiParam(std::string key, std::string value);
		std::string	getField(std::string field);

		void	readLine(std::string line);

		bool					getAutoIndex(void);
		std::string				getRoot(void);
		bool					getAllowedMethods(std::string methods);
		std::set<std::string>	getPagesIndex(void);
		std::string				getCgiPass(void);
		std::string				getRedirect(void);
		std::string				getUpdatePath(void);
		std::string				getCgiParm(std::string param);

	private:
		bool								_autoIndex;
		std::string							_root;
		std::map<std::string, bool>			_allowedMethods;
		std::set<std::string>				_pagesIndex;
		std::string							_cgiPass;
		std::string							_redirect;
		std::string							_updatePath;
		std::map<std::string, std::string>	_cgiParam;
		std::map<std::string, std::string>	_variables;
		

	protected:
};

std::ostream&	operator<<(std::ostream& o, const LocationServer& i);

#endif
