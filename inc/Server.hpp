/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:07:12 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/27 17:29:45 by ggalon           ###   ########.fr       */
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
		void setName(std::istringstream &iss);
		void setHostName(const std::string &hostname);
		void setErrorPages(int errCode, const std::string &errorPages);
		void setMaxBodySize(size_t maxBodySize);
		void addLocation(Location *location);
		size_t getMaxBodySize() const;
		const std::string &getHostName() const;
		const std::vector<Location *> &getLocations() const;
		const std::string &getErrorPage(int ErrorCode) const;

		void setRootPath(std::istringstream &iss);
		void addIndex(std::istringstream &iss);
		void setAutoIndex(std::istringstream &iss);
		void setAllowMethods(std::istringstream &iss);
		void setRedirect(std::istringstream &iss);
		void addCGI(std::istringstream &iss);
		void setUploadDir(std::istringstream &iss);

		const std::string &getName();
		const int &getPort() const;
		const std::string &getRootPath() const;
		const std::vector<std::string> &getIndexes() const;
		const bool &getAutoIndex() const;
		const std::vector<std::string> &getAllowMethods() const;
		const bool &getRedirect() const;
		const std::string &getRedirectPath() const;
		const int &getRedirectCode() const;
		const std::map<std::string, std::string> &getCGI() const;
		const std::string &getUploadDir() const;

		void displayServer();

	private:
		std::string							_name;
		std::string							_hostname;
		std::map<int, const std::string>	_errorPages;
		std::vector<Location *>				_locations;
		size_t								_maxBodySize;

		std::string							_root;
		std::vector<std::string>			_indexes;
		bool								_autoIndex;
		std::vector<std::string>			_allowMethods;
		bool								_redirect;
		std::string							_redirectPath;
		int									_redirectCode;
		std::map<std::string, std::string>	_cgi;
		std::string							_uploadDir;
		int									_port;
};
