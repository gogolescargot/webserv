/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:10:14 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/25 13:19:07 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Socket.hpp"

int main(int argc, const char *argv[])
{
	std::map<std::string, Server> serverList;

	if (argc != 2)
	{
		error("Usage: ./webserv <config_file>");
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
