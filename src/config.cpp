/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:07:26 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/25 16:40:18 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int config(const char *filePath, std::vector<Server*> &serverList)
{
	ConfigFile config(filePath);
	
	if (config.open())
	{
		return (1);
	}

	config.read();

	config.token();

	config.parse(serverList);
	
	config.close();

	return (0);
}
