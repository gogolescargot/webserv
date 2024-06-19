/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:07:12 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/19 15:40:56 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Location;

class Server
{
	public:
		Server();
		~Server();
		void setHostName(const std::string &hostname);
		void setErrorPages(int errCode, const std::string &errorPages);
		void setMaxBodySize(size_t maxBodySize);
		void addLocation(Location *location);
		size_t getMaxBodySize() const;
		const std::vector<std::string> &getHostNames() const;
		const std::vector<Location *> &getLocations() const;
		const std::map<int, const std::string> getErrorPages(const std::string &errorPages);
	private:
		std::vector<std::string>			_hostnames;
		std::map<int, const std::string>	_errorPages;
		std::vector<Location *>				_locations;
		size_t								_maxBodySize;
};
