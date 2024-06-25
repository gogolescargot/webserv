/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:10:14 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/24 20:05:46 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, const char *argv[])
{
	std::map<std::string, Server> serverList;

	if (argc != 2)
	{
		error("Wrong argument number");
		return (1);
	}
	try 
	{
		if (config(argv[1], serverList))
		{
			return (1);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	Socket socket(serverList);
	return (0);
}
