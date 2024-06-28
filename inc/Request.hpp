/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:08:59 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/28 16:34:22 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <dirent.h>
#include "Location.hpp"
#include "Server.hpp"
#include "utils.hpp"

#define CRLF "\r\n"


class Request
{
	private:
		std::string _method;
		std::string _filename;
		std::string _path;
		std::string _prefix;
		std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::map<std::string, std::string> _mimeTypes;
		std::map<int, const std::string> _errorPages;
		std::string _content;
		std::string _body;
        std::string _rootPath;
        std::string _uploadDir;
		std::string _redirectPath;
		int 		_redirectCode;
		bool _is_redirect;
		bool is_bad_request;
		bool _allowed_method;
        bool _auto_index;
	public:
		Request();
		~Request();
		void initialize();
		void parseRequest(std::string &msg);
		void getFileContent(const std::string &filename, const Server &server);
        void getDirectoryListing(const std::string &path, const Server &server);
        void initializeVariables(const Server &server);
		void handlePostRequest(const Server &server);
		void handleDeleteRequest(const Server &server);
		void handleGetRequest(const Server &server);
		void onMessageReceived(int client_fd, const Server &server);
		int	checkRequest(std::string &msg);
		void fillContent(const std::string &status, const std::string &contenttype, const std::string &filename, const Server &server);

		const std::string &getErrorPage(int ErrorCode) const;
};