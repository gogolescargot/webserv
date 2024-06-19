/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:10:27 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/19 15:40:41 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

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
int config(const char *filepath);

#endif