/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:47:07 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/19 15:38:36 by ggalon           ###   ########.fr       */
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
		error(errorOutput + ": " + _path);
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

int ConfigFile::isSeparator(char c)
{
	if (c == '{' || c == '}' || c == ';')
	{
		return (1);
	}

	return (0);
}

int ConfigFile::parse()
{
	std::vector<Server> serverList;

	for (size_t i = 0; i < _tokens.size(); i++)
	{
		std::cout << _tokens[i] << std::endl;
	}
	
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		if (_tokens[i] == "server")
		{
			if (createServer(i))
			{
				return (1);
			}
		}
	}
	return (0);
}

int ConfigFile::checkToken(const std::string &str, const size_t i)
{
	if (i < _tokens.size() && _tokens[i] == str)
	{
		return (1);
	}

	return (0);
}

int ConfigFile::isKeyword(size_t &i)
{
	const std::string keyword_single[] = {"hostname", "root", "client_max_body_size", "server_name", "listen", "cgi_pass", "autoindex"};
	const std::string keyword_multiple[] = {"error_pages", "return"};
	const std::string keyword_bracket[] = {"location"};

	if (i >= _tokens.size())
	{
		return (0);
	}

	for (size_t j = 0; j < 6; j++)
	{
		if (_tokens[i] == keyword_single[j])
		{
			return (1);
		}
	}

	for (size_t j = 0; j < 1; j++)
	{
		if (_tokens[i] == keyword_multiple[j])
		{
			return (2);
		}
	}

	for (size_t j = 0; j < 1; j++)
	{
		if (_tokens[i] == keyword_bracket[j])
		{
			return (3);
		}
	}

	return (0);
}

int ConfigFile::getArgument(size_t &i, Server &server)
{
	const int key = i;

	i++;

	if (checkToken(";", i))
	{
		error("Formatting error: Not enough argument");
		return (1);
	}

	if (checkToken("hostname", key))
	{
		server.setHostName(_tokens[i]);
	}
	else if (checkToken("client_max_body_size", key))
	{
		server.setMaxBodySize(std::atoi(_tokens[i].c_str()));
	}

	i++;

	if (!checkToken(";", i))
	{
		error("Formatting error: Too much argument");
		return (1);
	}

	i++;
	
	return (0);
}

int ConfigFile::createServer(size_t &i)
{
	Server server;
	size_t bracket = 0;

	i++;

	if (!checkToken("{", i))
	{
		error("Formatting error: Missing bracket");
		return (1);
	}

	bracket++;
	i++;

	while (bracket > 0 && i < _tokens.size())
	{
		if (checkToken("{", i))
		{
			bracket++;
		}
		else if (checkToken("}", i))
		{
			bracket--;
		}
		else
		{
			if (isKeyword(i) == 1)
			{
				if (getArgument(i, server))
				{
					return (1);
				}
			}
			else if (isKeyword(i) == 2)
			{
				// getMultipleArgument();
			}
			else if (isKeyword(i) == 3)
			{
				// getBracketArgument();
			}
			else
			{
				error("Formatting error: Wrong keyword");
				return (1);
			}
		}
	}

	if (bracket != 0)
	{
		error("Formatting error: Bracket error");
		return (1);
	}
	
	return (0);
}