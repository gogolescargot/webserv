/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:14:19 by lunagda           #+#    #+#             */
/*   Updated: 2024/09/08 14:38:38 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Server::Server()
{
    _maxBodySize = 0;
	allFilesExist = true;
	_errorPages.insert(std::pair<int, const std::string>(204, "error_pages/204.html"));
    _errorPages.insert(std::pair<int, const std::string>(400, "error_pages/400.html"));
    _errorPages.insert(std::pair<int, const std::string>(403, "error_pages/403.html"));
    _errorPages.insert(std::pair<int, const std::string>(404, "error_pages/404.html"));
    _errorPages.insert(std::pair<int, const std::string>(405, "error_pages/405.html"));
    _errorPages.insert(std::pair<int, const std::string>(408, "error_pages/408.html"));
    _errorPages.insert(std::pair<int, const std::string>(410, "error_pages/410.html"));
    _errorPages.insert(std::pair<int, const std::string>(413, "error_pages/413.html"));
    _errorPages.insert(std::pair<int, const std::string>(500, "error_pages/500.html"));
    _errorPages.insert(std::pair<int, const std::string>(502, "error_pages/502.html"));

	for (const auto& errorPage : _errorPages)
	{
		if (access(errorPage.second.c_str(), F_OK) == -1)
		{
			allFilesExist = false;
			break;
		}
	}	
}

Server::~Server()
{
	for (std::vector<Location *>::iterator it = _locations.begin(); it != _locations.end(); it++)
		delete *it;
}

void Server::setHostName(const std::string &hostname)
{
	std::istringstream stream(hostname);
	std::string line;
	
	while (std::getline(stream, line, ':'))
	{
		if (_hostname.empty())
			_hostname = line;
		else
			_port = std::atoi(line.c_str());
	}
}

void Server::setErrorPages(int errCode, const std::string &errorPages)
{
	if (_errorPages.find(errCode) != _errorPages.end())
		_errorPages.erase(errCode);
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

const std::string &Server::getHostName() const
{
	return _hostname;
}

const std::vector<Location *> &Server::getLocations() const
{
	return _locations;
}

const std::map<int, const std::string> &Server::getErrorPages() const
{
	return _errorPages;
}

const std::string &Server::getErrorPage(int ErrorCode) const
{
	if (_errorPages.find(ErrorCode) != _errorPages.end())
		return _errorPages.find(ErrorCode)->second;
	else 
		return _errorPages.find(404)->second;
}

// void Server::displayServer()
// {
// 	std::cout << "Host: " << this->getRootPath() << std::endl;
// 	std::cout << "AutoIndex: " << this->getAutoIndex() << std::endl;
// 	std::cout << "Upload Dir: " << this->getUploadDir() << std::endl;
// 	std::cout << "Redirect Path: " << this->getRedirectPath() << std::endl;
// }

// const std::map<int, const std::string> Server::getErrorPages(const std::string &errorPages)
// {
// 	return _errorPages;
// }

const int &Server::getPort() const
{
	return _port;
}

const std::string &Server::getRootPath() const
{
	return _root;
}

const std::vector<std::string> &Server::getIndexes() const
{
	return _indexes;
}

const bool &Server::getAutoIndex() const
{
	return _autoIndex;
}

const std::vector<std::string> &Server::getAllowMethods() const
{
	return _allowMethods;
}

const bool &Server::getRedirect() const
{
	return _redirect;
}

const bool &Server::getFilesExist() const
{
	return allFilesExist;
}

const std::string &Server::getRedirectPath() const
{
	return _redirectPath;
}

const int &Server::getRedirectCode() const
{
	return _redirectCode;
}

const std::map<std::string, std::string> &Server::getCGI() const
{
	return _cgi;
}

const std::string &Server::getUploadDir() const
{
	return _uploadDir;
}

void Server::setRootPath(std::istringstream &iss)
{
	std::string root;
	struct stat s;
	
	if (!(iss >> root))
		throw std::runtime_error("Error: invalid root path");
	if (stat(root.c_str(), &s) == -1)
		throw std::runtime_error("Error: invalid root path, root path does not exist");
	if (!(s.st_mode && (bool)S_IFDIR))
		throw std::runtime_error("Error: root path is not a directory");
	_root = root;
}

void Server::addIndex(std::istringstream &iss)
{
	std::string index;
	
	if (!(iss >> index))
		throw std::runtime_error("Error: invalid index");
	_indexes.push_back(index);
}

void Server::setAutoIndex(std::istringstream &iss)
{
	std::string autoIndex;
	
	if (!(iss >> autoIndex))
		throw std::runtime_error("Error: invalid autoindex");
	if (autoIndex == "on")
		_autoIndex = true;
	else if (autoIndex == "off")
		_autoIndex = false;
	else
		throw std::runtime_error("Error: invalid autoindex");
}

void Server::setAllowMethods(std::istringstream &iss)
{
	std::string methods;
	
	while (iss >> methods)
	{
		if (methods == "GET" || methods == "POST" || methods == "DELETE")
			_allowMethods.push_back(methods);
		else
			throw std::runtime_error("Error: invalid method");
	}
	if (!iss.eof())
		throw std::runtime_error("Error: error in index");
}

void Server::setRedirect(std::istringstream &iss)
{
	int code;
	std::string path;
	
	iss >> code >> path;
	if (iss.fail() || !iss.eof())
		throw std::runtime_error("Error: invalid redirect");
	if (code != 301 && code != 302)
		throw std::runtime_error("Error: invalid redirect code");
	_redirect = true;
	_redirectCode = code;
	_redirectPath = path;
}

void Server::addCGI(std::istringstream &iss)
{
	std::string extension;
	std::string path;
	
	if (!(iss >> extension >> path))
		throw std::runtime_error("Error: invalid CGI");
	_cgi[extension] = path;
}

void Server::setUploadDir(std::istringstream &iss)
{
	std::string uploadDir;
	struct stat s;

	if (!(iss >> uploadDir))
		throw std::runtime_error("Error: invalid upload directory");
	if (stat((getRootPath() + uploadDir).c_str(), &s) == -1)
	{
		std::cout << "_root + uploadDir: " << _root + uploadDir << std::endl;
		throw std::runtime_error("Error: invalid path, path does not exist");
	}
	if (!(s.st_mode && bool(S_IFDIR)))
		throw std::runtime_error("Error: path is not a directory");
	_uploadDir = uploadDir;
}