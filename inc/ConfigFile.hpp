/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:40:32 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/16 22:10:16 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

class ConfigFile
{
private:
	const std::string _path;
	std::ifstream _stream;
	std::string _content;

public:
	ConfigFile(const std::string&);
	~ConfigFile();

	std::string &getContent();

	int read();
	int open();
	int close();
};
