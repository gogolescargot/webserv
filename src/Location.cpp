/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:36:48 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/18 14:54:16 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(Server *server) : _server(server)
{
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

void Location::setRootPath(std::istringstream &iss)
{
	std::string root;
	
	if (!(iss >> root))
		throw std::runtime_error("Error: invalid root path");
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

void Location::addCGI(std::istringstream &iss)`
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
	
	if (!(iss >> uploadDir))
		throw std::runtime_error("Error: invalid upload directory");
	_uploadDir = uploadDir;
}