/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:10:27 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/24 22:37:10 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <fstream>
# include <string>
# include <cstring>
# include <vector>
# include <map>
# include <stdlib.h>
# include <sys/epoll.h>

# include "ConfigFile.hpp"
# include "Location.hpp"
# include "Server.hpp"
# include "Socket.hpp"
# include "Request.hpp"
# include "Printer.hpp"
# include "ServerList.hpp"

void error(const std::string& message);
int config(const char *filepath, std::vector<Server*> &serverList);
int isSeparator(int c);
