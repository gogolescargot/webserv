/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 23:30:40 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/26 14:13:15 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

ServerList::ServerList()
{
}

ServerList::~ServerList()
{
	for (size_t i = 0; i < _serverList.size(); i++)
	{
		delete _serverList[i];
	}
}

std::vector<Server*> &ServerList::getServerList()
{
	return (_serverList);
}
