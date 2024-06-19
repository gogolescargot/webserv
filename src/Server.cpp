/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:14:19 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/19 15:41:35 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Server::Server() : _maxBodySize(0)
{
}

Server::~Server()
{
	for (std::vector<Location *>::iterator it = _locations.begin(); it != _locations.end(); it++)
		delete *it;
}

void Server::setHostName(const std::string &hostname)
{
	_hostnames.push_back(hostname);
}

void Server::setErrorPages(int errCode, const std::string &errorPages)
{
	_errorPages.insert(std::pair<int, const std::string>(errCode, errorPages));
}

void Server::setMaxBodySize(size_t maxBodySize)
{
	_maxBodySize = maxBodySize;
}

void Server::addLocation(Location *location)
{
	_locations.push_back(location);
}

size_t Server::getMaxBodySize() const
{
	return _maxBodySize;
}

const std::vector<std::string> &Server::getHostNames() const
{
	return _hostnames;
}

const std::vector<Location *> &Server::getLocations() const
{
	return _locations;
}

// const std::map<int, const std::string> Server::getErrorPages(const std::string &errorPages)
// {
// 	return _errorPages;
// }

