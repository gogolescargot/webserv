/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:07:26 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/26 00:07:44 by ggalon           ###   ########.fr       */
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


	for (size_t i = 0; i < serverList.size(); i++)
	{
		(*(serverList[i])).setErrorPages(404, "error_pages/404.html");
		if ((*(serverList[i])).getErrorPage(204) == (*(serverList[i])).getErrorPage(404))
			(*(serverList[i])).setErrorPages(204, "error_pages/204.html");
		if ((*(serverList[i])).getErrorPage(400) == (*(serverList[i])).getErrorPage(404))
			(*(serverList[i])).setErrorPages(400, "error_pages/400.html");
		if ((*(serverList[i])).getErrorPage(403) == (*(serverList[i])).getErrorPage(404))
			(*(serverList[i])).setErrorPages(403, "error_pages/403.html");
		if ((*(serverList[i])).getErrorPage(405) == (*(serverList[i])).getErrorPage(404))
			(*(serverList[i])).setErrorPages(405, "error_pages/405.html");
		if ((*(serverList[i])).getErrorPage(410) == (*(serverList[i])).getErrorPage(404))
			(*(serverList[i])).setErrorPages(410, "error_pages/410.html");
		if ((*(serverList[i])).getErrorPage(413) == (*(serverList[i])).getErrorPage(404))
			(*(serverList[i])).setErrorPages(413, "error_pages/413.html");
		if ((*(serverList[i])).getErrorPage(500) == (*(serverList[i])).getErrorPage(404))
			(*(serverList[i])).setErrorPages(500, "error_pages/500.html");
	}

	return (0);
}
