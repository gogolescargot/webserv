/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:10:27 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/24 13:38:43 by lunagda          ###   ########.fr       */
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

# include "ConfigFile.hpp"
# include "Location.hpp"
# include "Server.hpp"

void error(const std::string& message);
int config(const char *filepath, std::map<std::string, Server> &serverList);
int isSeparator(int c);
