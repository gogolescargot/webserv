/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:00:10 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/28 12:41:50 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

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

bool startsWith(const std::string& str1, const std::string& str2)
{
    if (str2.size() > str1.size())
    {
        return (false);
    }
    return (std::equal(str2.begin(), str2.end(), str1.begin()));
}