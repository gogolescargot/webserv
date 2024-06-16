/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:07:26 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/16 22:10:07 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int config(const char *filePath)
{
	ConfigFile config(filePath);
	
	if (config.open())
	{
		return (1);
	}

	config.read();

	// config.parse();
	
	config.close();

	return (0);
}
