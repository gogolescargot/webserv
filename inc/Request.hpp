/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:08:59 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/24 13:46:06 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "Location.hpp"
#include "Server.hpp"
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>

#define CRLF "\r\n"


class Request
{
	private:
		std::string _method;
		std::string _filename;
		std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::map<std::string, std::string> _mimeTypes;
		std::map<long, std::string> _errorCodes;
		std::string _content;
		std::string _body;
		bool is_bad_request;
	public:
		void initialize();
		void parseRequest(std::string &msg);
		void getFileContent(const std::string &filename);
		void onMessageReceived(Server &server);
		int	checkRequest(std::string &msg);
};