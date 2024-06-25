/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerList.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 23:30:36 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/25 23:30:37 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

class ServerList
{
private:
	std::vector<Server*> _serverList;
public:
	ServerList();
	~ServerList();
	std::vector<Server*> &getServerList();
};
