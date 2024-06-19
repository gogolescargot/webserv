/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:08:59 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/19 16:38:04 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "Location.hpp"
#include "Server.hpp"

class Request
{
	private:
		std::string	_host;
		std::string	_contentLen;
		std::string	_contentType;
		std::string	_contentTypeResponse;
		std::string	_accept;
		std::string	_acceptEncoding;
		std::string _encoding;
		std::string	_secFetchDest;
		std::string	_secFetchMode;
	public:

};