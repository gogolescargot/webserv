/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Printer.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:54:25 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/25 16:07:28 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Printer::Printer()
{
	
}

void Printer::printLocation(const Server *server)
{
	const std::vector<Location *> &locations = server->getLocations();
	
	for (size_t i = 0; i < locations.size(); i++)
	{
		std::cout << "Location " << i + 1 << ":" << std::endl;
		std::cout << "Server: " << locations[i]->getServer() << std::endl;
		std::cout << "Path: " << locations[i]->getPath() << std::endl;
		std::cout << "Root: " << locations[i]->getRootPath() << std::endl;
		std::cout << "UploadDir: " << locations[i]->getUploadDir() << std::endl;
		printIndexes(locations[i]->getIndexes());
		printAllowMethods(locations[i]->getAllowMethods());
		printRedirect(locations[i]);
		printCGI(locations[i]->getCGI());
	}
}

void Printer::printHostNames(std::vector<std::string> hostNames)
{
	std::cout << "Hostnames: ";
	for (size_t i = 0; i < hostNames.size(); i++)
	{
		std::cout << hostNames[i];
		if (i + 1 < hostNames.size())
			std::cout << ", ";
	}
	std::cout << std::endl;
}

void Printer::printErrorPages(std::map<int, const std::string> errorPages)
{
	std::cout << "Error pages: " << std::endl;
	for (std::map<int, const std::string>::iterator it = errorPages.begin(); it != errorPages.end(); it++)
		std::cout << it->first << " -> " << it->second << std::endl;
}

void Printer::printIndexes(std::vector<std::string> indexes)
{
	std::cout << "Indexes: ";
	for (size_t i = 0; i < indexes.size(); i++)
	{
		std::cout << indexes[i];
		if (i + 1 < indexes.size())
			std::cout << ", ";
	}
	std::cout << std::endl;
}

void Printer::printAllowMethods(std::vector<std::string> allowMethods)
{
	std::cout << "Allow methods: ";
	for (size_t i = 0; i < allowMethods.size(); i++)
	{
		std::cout << allowMethods[i];
		if (i + 1 < allowMethods.size())
			std::cout << ", ";
	}
	std::cout << std::endl;
}

void Printer::printCGI(std::map<std::string, std::string> cgi)
{
	std::cout << "CGI: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = cgi.begin(); it != cgi.end(); it++)
		std::cout << it->first << " -> " << it->second << std::endl;
}

void Printer::printRedirect(Location *location)
{
	if (location->getRedirect())
		std::cout << "Redirect: " << location->getRedirectPath() << " " << location->getRedirectCode() << std::endl;
}

