/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:10:27 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/29 13:19:49 by marvin           ###   ########.fr       */
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
# include <sys/time.h>

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
