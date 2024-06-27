/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:36:48 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/27 14:07:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Location::Location(Server *server) : _server(server)
{
	_uploadDir = "";
    _autoIndex = false;
    _redirect = false;
    _redirectCode = 0;
    _path = "";
    _root = "";
    _indexes.clear();
    _allowMethods.clear();
    _cgi.clear();
    _errorPages.insert(std::pair<int, const std::string>(204, "error_pages/204.html"));
    _errorPages.insert(std::pair<int, const std::string>(400, "error_pages/400.html"));
    _errorPages.insert(std::pair<int, const std::string>(403, "error_pages/403.html"));
    _errorPages.insert(std::pair<int, const std::string>(404, "error_pages/404.html"));
    _errorPages.insert(std::pair<int, const std::string>(405, "error_pages/405.html"));
    _errorPages.insert(std::pair<int, const std::string>(410, "error_pages/410.html"));
    _errorPages.insert(std::pair<int, const std::string>(413, "error_pages/413.html"));
    _errorPages.insert(std::pair<int, const std::string>(500, "error_pages/500.html"));
}

Location::Location(const Location &copy)
{
	*this = copy;
}

Location::~Location()
{
}

Location &Location::operator=(const Location &copy)
{
	if (this == &copy)
		return *this;
	_path = copy._path;
	_root = copy._root;
	_indexes = copy._indexes;
	_autoIndex = copy._autoIndex;
	_allowMethods = copy._allowMethods;
	_redirect = copy._redirect;
	_redirectPath = copy._redirectPath;
	_redirectCode = copy._redirectCode;
	_server = copy._server;
	_cgi = copy._cgi;
	_uploadDir = copy._uploadDir;
	return *this;
}

const std::string &Location::getPath() const
{
	return _path;
}

const std::string &Location::getRootPath() const
{
	return _root;
}

const std::vector<std::string> &Location::getIndexes() const
{
	return _indexes;
}

const bool &Location::getAutoIndex() const
{
	return _autoIndex;
}

const std::vector<std::string> &Location::getAllowMethods() const
{
	return _allowMethods;
}

const bool &Location::getRedirect() const
{
	return _redirect;
}

const std::string &Location::getRedirectPath() const
{
	return _redirectPath;
}

const int &Location::getRedirectCode() const
{
	return _redirectCode;
}

Server *Location::getServer() const
{
	return _server;
}

const std::map<std::string, std::string> &Location::getCGI() const
{
	return _cgi;
}

const std::string &Location::getUploadDir() const
{
	return _uploadDir;
}

void Location::setPath(std::istringstream &iss)
{
	std::string path;

	if (!(iss >> path))
		throw std::runtime_error("Error: invalid path");
	_path = path;
}

void Location::setErrorPages(int errCode, const std::string &errorPages)
{
    if (_errorPages.find(errCode) != _errorPages.end())
    {
        _errorPages.erase(errCode);
    }
	_errorPages.insert(std::pair<int, const std::string>(errCode, errorPages));
}

void Location::setRootPath(std::istringstream &iss)
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

void Location::addIndex(std::istringstream &iss)
{
	std::string index;
	
	if (!(iss >> index))
		throw std::runtime_error("Error: invalid index");
	_indexes.push_back(index);
}

void Location::setAutoIndex(std::istringstream &iss)
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

void Location::setAllowMethods(std::istringstream &iss)
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

void Location::setRedirect(std::istringstream &iss)
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

void Location::addCGI(std::istringstream &iss)
{
	std::string extension;
	std::string path;
	
	if (!(iss >> extension >> path))
		throw std::runtime_error("Error: invalid CGI");
	_cgi[extension] = path;
}

void Location::setUploadDir(std::istringstream &iss)
{
	std::string uploadDir;
	struct stat s;

	if (!(iss >> uploadDir))
		throw std::runtime_error("Error: invalid upload directory");
	if (stat((_server->getRootPath() + uploadDir).c_str(), &s) == -1)
		throw std::runtime_error("Error: invalid path, path does not exist");
	if (!(s.st_mode && (bool)S_IFDIR))
		throw std::runtime_error("Error: path is not a directory");
	_uploadDir = uploadDir;
}