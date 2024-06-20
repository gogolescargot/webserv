/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:00:10 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/20 14:55:30 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void error(const std::string& message)
{
	std::cerr << "Error: " << message << std::endl;
}

int isSeparator(int c)
{
	if (c == '{' || c == '}' || c == ';')
	{
		return (1);
	}

	return (0);
}