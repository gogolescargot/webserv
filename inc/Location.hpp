/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:17:35 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/20 20:37:06 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "Server.hpp"
#include <sys/stat.h>

class Server;

class Location
{
	public:
		Location(Server *server);
		Location(const Location &copy);
		~Location();
		Location &operator=(const Location &copy);
		
		void setPath(std::istringstream &iss);
		void setErrorPages(int errCode, const std::string &errorPages);
		void setRootPath(std::istringstream &iss);
		void addIndex(std::istringstream &iss);
		void setAutoIndex(std::istringstream &iss);
		void setAllowMethods(std::istringstream &iss);
		void setRedirect(std::istringstream &iss);
		void addCGI(std::istringstream &iss);
		void setUploadDir(std::istringstream &iss);
		
		const std::string &getPath() const;
		const std::string &getRootPath() const;
		const std::vector<std::string> &getIndexes() const;
		const bool &getAutoIndex() const;
		const std::vector<std::string> &getAllowMethods() const;
		const bool &getRedirect() const;
		const std::string &getRedirectPath() const;
		const int &getRedirectCode() const;
		Server *getServer() const;
		const std::map<std::string, std::string> &getCGI() const;
		const std::string &getUploadDir() const;

	private:
		std::string							_path;
		std::string							_root;
		std::map<int, const std::string>	_errorPages;
		std::vector<std::string>			_indexes;
		bool								_autoIndex;
		std::vector<std::string>			_allowMethods;
		bool								_redirect;
		std::string							_redirectPath;
		int									_redirectCode;
		Server								*_server;
		std::map<std::string, std::string>	_cgi;
		std::string							_uploadDir;
};