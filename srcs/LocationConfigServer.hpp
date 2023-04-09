/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfigServer.hpp                           :+:      :+:    :+:   */
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

class LocationConfigServer
{
	public:
		LocationConfigServer(void);
		LocationConfigServer(const LocationConfigServer& obj);
		virtual ~LocationConfigServer(void);

		LocationConfigServer&	operator=(const LocationConfigServer& obj);

		void	setAutoIndex(bool autoIndex);
		void	setRoot(std::string root);
		void	setAllowedMethods(std::string name, bool status);
		void	setPagesIndex(std::string page);
		void	setCgi(std::string cgi);

		bool					getAutoIndex(void);
		std::string				getRoot(void);
		bool					getAllowedMethods(std::string methods);
		std::set<std::string>	getPagesIndex(void);
		std::string				getCgi(void);

	private:
		bool						_autoIndex;
		std::string					_root;
		std::map<std::string, bool>	_allowedMethods;
		std::set<std::string>		_pagesIndex;
		std::string					_cgi;

	protected:
};

std::ostream&	operator<<(std::ostream& o, const LocationConfigServer& i);

#endif
