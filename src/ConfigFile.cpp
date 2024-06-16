/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:47:07 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/16 22:10:27 by ggalon           ###   ########.fr       */
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