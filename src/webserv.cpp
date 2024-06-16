/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:10:14 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/16 22:05:53 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, const char *argv[])
{
	if (argc != 2)
	{
		error("Wrong argument number");
		return (1);
	}
	
	if (config(argv[1]))
	{
		return (1);
	}

	return (0);
}
