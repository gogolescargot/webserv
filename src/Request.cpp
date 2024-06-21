/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:08:55 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/21 15:26:24 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// Header file format
//GET /index.html HTTP/1.1
//Host: example.com
//User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36
//Accept-Language: en-US,en;q=0.9
//Accept-Charsets: utf-8, iso-8859-1;q=0.5
//Referer: http://example.com/previous-page
//Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l
//Connection: Keep-Alive

void	Request::initialize()
{
	std::string extension;

	extension = _filename.rfind(".") != std::string::npos ? _filename.substr(_filename.rfind(".") + 1) : "";
	_mimeTypes[".html"] = "text/html";
	_mimeTypes[".css"] = "text/css";
	_mimeTypes[".js"] = "application/javascript";
	_mimeTypes[".jpg"] = "image/jpeg";
	_mimeTypes[".png"] = "image/png";
	_mimeTypes[".gif"] = "image/gif";
	_mimeTypes[".pdf"] = "application/pdf";
	_mimeTypes[".txt"] = "text/plain";
	_mimeTypes[".xml"] = "application/xml";
	_mimeTypes[".json"] = "application/json";
	_mimeTypes[".mp4"] = "video/mp4";
	_mimeTypes[".mp3"] = "audio/mpeg";
	_headers["Connection"] = "Keep-Alive";
	_headers["Content-Type"] = _mimeTypes[extension];
	_headers["Cache-Control"] = "no-cache, private";
	_errorCodes[200] = "OK";
	_errorCodes[201] = "Created";
	_errorCodes[400] = "Bad Request";
	_errorCodes[403] = "Forbidden";
	_errorCodes[404] = "Not Found";
	_errorCodes[405] = "Method Not Allowed";
	_errorCodes[413] = "Payload Too Large";
	_errorCodes[418] = "I'm a teapot";
	_errorCodes[502] = "Bad Gateway";
	_errorCodes[503] = "Service Unavailable";
}

void	Request::getFileContent(std::string filename)
{
	std::ifstream file(".\\wwwroot" + filename);
	if (file.good())
	{
		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		_content = str;
		_headers["Content-Length"] = std::to_string(_content.size());
	}
	else
	{
		getFileContent("/404.html");
		_headers["Status"] = "404 Not Found";
		
	}
}

void	Request::onMessageReceived(std::string &msg, Server &server)
{
	(void)server;
	if (_method != "GET")
	{
		_headers["Content-Length"] = "0";
		_headers["Status"] = "405 Method Not Allowed";
		_headers["Content-Type"] = "text/html";
		getFileContent("405.html");
	}
	else if (_filename == "/")
	{
		_headers["Server"] = "200" + _errorCodes[200];
		_headers["Content-Type"] = "text/html";
		_headers["Status"] = "200 OK";
		getFileContent("/index.html");
	}
	else
	{
		_headers["Status"] = "200 OK";
		_headers["Content-Type"] = _mimeTypes[_filename.substr(_filename.rfind("."))];
		getFileContent(_filename);
	}
	
	std::string response = _httpVersion + " " + _headers["Status"] + CRLB;
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		response += it->first + ": " + it->second + CRLB;
	}
	response += CRLB + _content;
	std::cout << response << std::endl;
	//sendToClient(socket, response.c_str(), response.size() + 1);
}

void	Request::parseRequest(std::string &msg)
{
	std::istringstream iss(msg);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string line;
	for (size_t i = 0; i < parsed.size(); i++)
	{
		if (i == 0)
			_method = parsed[i];
		else if (i == 1)
			_filename = parsed[i];
		else if (i == 2)
			_httpVersion = parsed[i];
		else if (parsed[i] == "Host:")
			_headers["Host"] = parsed[i + 1];
	}
	initialize();
}