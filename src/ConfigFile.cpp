/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:47:07 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/27 17:32:30 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

ConfigFile::ConfigFile(const std::string &path) : _path(path)
{

}

ConfigFile::~ConfigFile()
{
	
}

std::string &ConfigFile::getContent()
{
	return (_content);
}

int ConfigFile::open()
{
	_stream.open(_path.c_str());
	
	if (_stream.fail())
	{
		std::string errorOutput(strerror(errno));
		throw std::runtime_error(errorOutput + ": " + _path);
		return (1);
	}
	
	return (0);
}

int ConfigFile::read()
{
	std::string line;

	while (getline(_stream, line))
	{
		_content += line + '\n';
	}

	return (0);
}

int ConfigFile::close()
{
	_stream.close();
	
	return (0);
}

int ConfigFile::token()
{
	for (size_t i = 0; i < _content.length(); i++)
	{
		if (!std::isspace(_content[i]))
		{
			if (_content[i] != '#')
			{
				_tokens.push_back(word(i));
			}
			else
			{
				comment(i);
			}
		}
	}

	return (0);
}

std::string ConfigFile::word(size_t &i)
{
	size_t iTemp = i;
	size_t j = 0;

	if (isSeparator(_content[iTemp + j]))
	{
		return (_content.substr(iTemp, 1));
	}

	while (iTemp + j < _content.length() && !isSeparator(_content[iTemp + j]) && !std::isspace(_content[iTemp + j]))
	{
		i++;
		j++;
	}

	i--;

	return (_content.substr(iTemp, j));
}

int ConfigFile::comment(size_t &i)
{

	while (i < _content.length() && _content[i] != '\n')
	{
		i++;
	}

	return (0);
}

int ConfigFile::parse(std::vector<Server *> &serverList)
{
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		if (_tokens[i] == "server")
		{
			if (createServer(i, serverList))
			{
				return (1);
			}
		}
	}
	return (0);
}

int ConfigFile::checkToken(const size_t &i, const std::string &str)
{
	if (i < _tokens.size() && _tokens[i] == str)
	{
		return (1);
	}

	return (0);
}

int ConfigFile::checkTokenFunction(const size_t &i, int (*func)(int))
{
	if (i >= _tokens.size())
	{
		return (0);
	}

	for (size_t j = 0; j < _tokens[i].length(); j++)
	{
		if (!func(_tokens[i][j]))
		{
			return (0);
		}
	}
	
	return (1);
}

int ConfigFile::isKeyword(size_t &i)
{
	const std::string keyword_single[] = {"hostname", "root", "client_max_body_size", "server_name", "listen", "autoindex", "upload_dir"};
	const std::string keyword_multiple[] = {"allow_methods", "error_page", "return", "index", "cgi"};

	if (i >= _tokens.size())
	{
		return (0);
	}

	for (size_t j = 0; j < 7; j++)
	{
		if (_tokens[i] == keyword_single[j])
		{
			return (1);
		}
	}

	for (size_t j = 0; j < 5; j++)
	{
		if (_tokens[i] == keyword_multiple[j])
		{
			return (2);
		}
	}

	if (_tokens[i] == "location")
	{
		return (3);
	}

	return (0);
}

int ConfigFile::getArgument(size_t &i, Server &server)
{
	const int key = i;

	i++;

	if (checkTokenFunction(i, isSeparator))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}
	
	std::istringstream iss(_tokens[i]);

	if (checkToken(key, "listen"))
	{
		server.setHostName(_tokens[i]);
	}
	else if (checkToken(key, "client_max_body_size"))
	{
		server.setMaxBodySize(std::atoi(_tokens[i].c_str()));
	}
	else if (checkToken(key, "autoindex"))
	{
		server.setAutoIndex(iss);
	}
	else if (checkToken(key, "root"))
	{
		server.setRootPath(iss);
	}
	else if (checkToken(key, "upload_dir"))
	{
		server.setUploadDir(iss);
	}
	else if (checkToken(key, "server_name"))
	{
		server.setName(iss);
	}

	i++;

	if (!checkToken(i, ";"))
	{
		throw std::runtime_error("Formatting error: Too many argument");
	}

	i++;
	
	return (0);
}

int ConfigFile::checkErrorPages(size_t i)
{
	if (!checkTokenFunction(i, isdigit))
	{
		throw std::runtime_error("Formatting error: Invalid error code");
	}

	i++;
	
	if (checkTokenFunction(i, isSeparator))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}

	i++;

	if (!checkToken(i, ";"))
	{
		throw std::runtime_error("Formatting error: Too many argument");
	}

	return (0);
}

int ConfigFile::checkIndex(size_t &i, Server &server)
{
	if (checkToken(i, ";"))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}
	while (!checkToken(i, ";"))
	{
		std::istringstream iss(_tokens[i]);
		server.addIndex(iss);
		i++;
	}
	if (i >= _tokens.size())
	{
		throw std::runtime_error("Formatting error: Index error");
	}

	i++;

	return (0);
}

int ConfigFile::checkIndexLocation(size_t &i, Location &location)
{
	if (checkToken(i, ";"))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}
	while (!checkToken(i, ";"))
	{
		std::istringstream iss(_tokens[i]);
		location.addIndex(iss);
		i++;
	}
	if (i >= _tokens.size())
	{
		throw std::runtime_error("Formatting error: Index error");
	}

	i++;

	return (0);
}

int ConfigFile::checkRedirect(size_t i)
{
	if (!checkTokenFunction(i, isdigit))
	{
		throw std::runtime_error("Formatting error: Invalid error code");
	}

	i++;
	
	if (checkTokenFunction(i, isSeparator))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}

	i++;

	if (!checkToken(i, ";"))
	{
		throw std::runtime_error("Formatting error: Too many argument");
	}

	return (0);
}

int ConfigFile::checkMethods(size_t &i, Server &server)
{
	size_t j = 0;

	while (!checkToken(i, ";"))
	{
		std::istringstream iss(_tokens[i]);
		server.setAllowMethods(iss);
		j++;
		i++;
	}


	if (j == 0 && checkToken(i, ";"))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}

	i++;
	
	return (0);
}

int ConfigFile::checkMethodsLocation(size_t &i, Location &location)
{
	size_t j = 0;

	while (!checkToken(i, ";"))
	{
		std::istringstream iss(_tokens[i]);
		location.setAllowMethods(iss);
		j++;
		i++;
	}


	if (j == 0 && checkToken(i, ";"))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}

	i++;
	
	return (0);
}

int ConfigFile::checkCgi(size_t i)
{
	if (checkTokenFunction(i, isSeparator))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}

	i++;
	
	if (checkTokenFunction(i, isSeparator))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}

	i++;

	if (!checkToken(i, ";"))
	{
		throw std::runtime_error("Formatting error: Too many argument");
	}

	return (0);
}

int ConfigFile::getMultipleArgument(size_t &i, Server &server)
{
	const int key = i;

	i++;

	if (checkToken(key, "error_page"))
	{
		if (checkErrorPages(i))
		{
			return (1);
		}
		server.setErrorPages(std::atoi(_tokens[i].c_str()), _tokens[i + 1]);

		i += 3;
	}
	else if (checkToken(key, "index"))
	{
		if (checkIndex(i, server))
		{
			return (1);
		}
	}
	else if (checkToken(key, "cgi"))
	{
		if (checkCgi(i))
		{
			return (1);
		}
		
		std::istringstream iss(_tokens[i] + " " + _tokens[i + 1]);
		server.addCGI(iss);

		i += 3;
	}
	else if (checkToken(key, "return"))
	{
		if (checkRedirect(i))
		{
			return (1);
		}
		
		std::istringstream iss(_tokens[i] + " " + _tokens[i + 1]);
		server.setRedirect(iss);

		i += 3;
	}
	else if (checkToken(key, "allow_methods"))
	{
		if (checkMethods(i, server))
		{
			return (1);
		}
	}
	
	return (0);
}

int ConfigFile::getArgumentLocation(size_t &i, Location &location)
{
	const int key = i;

	i++;

	if (checkTokenFunction(i, isSeparator))
	{
		throw std::runtime_error("Formatting error: Not enough argument");
	}

	std::istringstream iss(_tokens[i]);

	if (checkToken(key, "autoindex"))
	{
		location.setAutoIndex(iss);
	}
	else if (checkToken(key, "root"))
	{
		location.setRootPath(iss);
	}
	else if (checkToken(key, "upload_dir"))
	{
		location.setUploadDir(iss);
	}
	else if (checkToken(key, "index"))
	{
		location.addIndex(iss);
	}

	i++;

	if (!checkToken(i, ";"))
	{
		throw std::runtime_error("Formatting error: Too many argument");
	}

	i++;
	
	return (0);
}

int ConfigFile::getMultipleArgumentLocation(size_t &i, Location &location)
{
	const int key = i;

	i++;

	if (checkToken(key, "error_page"))
	{
		if (checkErrorPages(i))
		{
			return (1);
		}
		location.setErrorPages(std::atoi(_tokens[i].c_str()), _tokens[i + 1]);

		i += 3;
	}
	else if (checkToken(key, "index"))
	{
		if (checkIndexLocation(i, location))
		{
			return (1);
		}
	}
	else if (checkToken(key, "cgi"))
	{
		if (checkCgi(i))
		{
			return (1);
		}
		
		std::istringstream iss(_tokens[i] + " " + _tokens[i + 1]);
		location.addCGI(iss);

		i += 3;
	}
	else if (checkToken(key, "return"))
	{
		if (checkRedirect(i))
		{
			return (1);
		}
		
		std::istringstream iss(_tokens[i] + " " + _tokens[i + 1]);
		location.setRedirect(iss);

		i += 3;
	}
	else if (checkToken(key, "allow_methods"))
	{
		if (checkMethodsLocation(i, location))
		{
			return (1);
		}
	}
	
	return (0);
}

int ConfigFile::getLocation(size_t &i, Server &server)
{
	Location *location = new Location(&server);
	
	server.addLocation(location);

	i++;
	
	if (checkTokenFunction(i, isSeparator))
	{
		throw std::runtime_error("Formatting error: Missing location path");
	}

	std::istringstream path(_tokens[i]); 
	location->setPath(path);

	if (location->getPath() != "/" && server.getLocations().size() == 1)
	{
		throw std::runtime_error("Formatting error: Wrong location order"); 
	}
	i++;

	if (!checkToken(i, "{"))
	{
		throw std::runtime_error("Formatting error: Missing opening bracket");
	}

	i++;

	while (i < _tokens.size() && _tokens[i] != "}")
	{
		if (isKeyword(i) == 1)
		{
			getArgumentLocation(i, *location);
		}
		else if (isKeyword(i) == 2)
		{
			getMultipleArgumentLocation(i, *location);
		}
		else
		{
			throw std::runtime_error("Formatting error: Wrong keyword");
		}
	}

	i++;
	
	return (0);
}

int ConfigFile::createServer(size_t &i, std::vector<Server *> &serverList)
{
	Server *server = new Server;

	serverList.push_back(server);

	size_t bracket = 0;

	i++;

	if (!checkToken(i, "{"))
	{
		throw std::runtime_error("Formatting error: Missing bracket");
	}

	bracket++;
	i++;

	while (bracket > 0 && i < _tokens.size())
	{
		if (checkToken(i, "{"))
		{
			bracket++;
		}
		else if (checkToken(i, "}"))
		{
			bracket--;
		}
		else
		{
			if (isKeyword(i) == 1)
			{
				getArgument(i, *server);
			}
			else if (isKeyword(i) == 2)
			{
				getMultipleArgument(i, *server);
			}
			else if (isKeyword(i) == 3)
			{
				getLocation(i, *server);
			}
			else
			{
				throw std::runtime_error("Formatting error: Wrong keyword");
			}
		}
	}

	if (bracket != 0)
	{
		throw std::runtime_error("Formatting error: Bracket error");
	}

	// servername listen root client size uploaddir

	if (server->getName().empty()
	|| server->getHostName().empty()
	|| server->getPort() == 0
	|| server->getUploadDir().empty()
	|| server->getRootPath().empty()
	|| server->getMaxBodySize() == 0)
	{
		throw std::runtime_error("Formatting error: Missing keyword");
	}

	return (0);
}