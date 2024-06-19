/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:47:07 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/19 13:10:06 by ggalon           ###   ########.fr       */
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
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		if (_tokens[i] == "server")
		{
			
		}
	}

	for (size_t i = 0; i < _tokens.size(); i++)
	{
		std::cout << _tokens[i] << std::endl;
	}

	return (0);
}