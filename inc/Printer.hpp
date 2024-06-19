/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Printer.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 14:46:37 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/19 14:56:22 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include "Location.hpp"
#include "Server.hpp"

class Location;
class Server;

class Printer
{
	public:
		void	printLocation(const Server *server);
		void	printHostNames(std::vector<std::string> hostNames);
		void 	printErrorPages(std::map<int, const std::string> errorPages);
		void	printIndexes(std::vector<std::string> indexes);
		void	printAllowMethods(std::vector<std::string> allowMethods);
		void	printCGI(std::map<std::string, std::string> cgi);
		void	printRedirect(Location *location);
	private:
		Printer();
};