/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:10:14 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/26 00:06:14 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, const char *argv[])
{
	if (argc != 2)
	{
		error("Usage: ./webserv <config_file>");
		return (1);
	}
	try 
	{
		ServerList serverList;
		config(argv[1], serverList.getServerList());
		Socket socket(serverList.getServerList());
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}


	return (0);
}
