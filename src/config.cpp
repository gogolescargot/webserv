/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:07:26 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/25 18:45:34 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int config(const char *filePath, std::map<std::string, Server> &serverList)
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

	for (std::map<std::string, Server>::iterator it = serverList.begin(); it != serverList.end(); it++)
	{
		Server server = it->second;
		server.setErrorPages(404, "error_pages/404.html");
		if (server.getErrorPage(204) == server.getErrorPage(404))
			server.setErrorPages(204, "error_pages/204.html");
		if (server.getErrorPage(400) == server.getErrorPage(404))
			server.setErrorPages(400, "error_pages/400.html");
		if (server.getErrorPage(403) == server.getErrorPage(404))
			server.setErrorPages(403, "error_pages/403.html");
		if (server.getErrorPage(405) == server.getErrorPage(404))
			server.setErrorPages(405, "error_pages/405.html");
		if (server.getErrorPage(410) == server.getErrorPage(404))
			server.setErrorPages(410, "error_pages/410.html");
		if (server.getErrorPage(413) == server.getErrorPage(404))
			server.setErrorPages(413, "error_pages/413.html");
		if (server.getErrorPage(500) == server.getErrorPage(404))
			server.setErrorPages(500, "error_pages/500.html");
	}

	return (0);
}
