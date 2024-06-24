/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:08:55 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/24 18:04:19 by ggalon           ###   ########.fr       */
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

Request::Request()
{
	is_bad_request = false;
}

Request::~Request()
{
	
}

template <typename T>
std::string ToString(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

void	Request::initialize()
{
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
	if (_headers["Content-Type"].empty())
	{
		std::string extension = _filename.rfind(".") != std::string::npos ? _filename.substr(_filename.rfind(".") + 1) : "";
		_headers["Content-Type"] = _mimeTypes[extension];
	}
	_headers["Server"] = "Webserv/1.0";
	_headers["Connection"] = "Keep-Alive";
	_headers["Cache-Control"] = "no-cache, private";
}

void	Request::getFileContent(const std::string &filename)
{
	std::ifstream file((".\\wwwroot" + filename).c_str());
	if (file.good())
	{
		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		_content = str;
		_headers["Content-Length"] = ToString(_content.size());
	}
	else
	{
		getFileContent("/404.html");
		_headers["Status"] = "404 Not Found";
		
	}
}

void	Request::onMessageReceived(int client_fd)
{
	(void)client_fd;
	if (is_bad_request)
	{
		_headers["Status"] = "400 Bad Request";
		_headers["Content-Type"] = "text/html";
		getFileContent("/400.html");
	}
	else
	{
		if (_method != "GET" || _method != "POST" || _method != "DELETE")
		{
			_headers["Content-Length"] = "0";
			_headers["Status"] = "405 Method Not Allowed";
			_headers["Content-Type"] = "text/html";
			getFileContent("/405.html");
		}
		if (_method == "GET")
		{
			if (_filename == "/")
			{
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
		}
		else if (_method == "POST")
		{
			std::ofstream file(("files/" + _filename).c_str());
			if (file && file.is_open())
			{
				file << _body;
				file.close();
				_headers["Status"] = "201 Created";
				_headers["Content-Type"] = "text/html";
				getFileContent("/index.html");
			}
			else
			{
				_headers["Status"] = "502 Bad Gateway";
				_headers["Content-Type"] = "text/html";
				getFileContent("/502.html");
			}
		}
		else if (_method == "DELETE")
		{
			if (remove(("files/" + _filename).c_str()) != 0)
			{
				_headers["Status"] = "404 Not Found";
				_headers["Content-Type"] = "text/html";
				getFileContent("/404.html");
			}
			else
			{
				_headers["Status"] = "204 No Content";
				_headers["Content-Type"] = "text/html";
				getFileContent("/204.html");
			}
		}
	}
	
	std::string response = _httpVersion + " " + _headers["Status"] + CRLF;
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		response += it->first + ": " + it->second + CRLF;
	}
	response += CRLF + _content;
	std::cout << response << std::endl;
	//sendToClient(socket, response.c_str(), response.size() + 1);
}

int	Request::checkRequest(std::string &msg)
{
	std::string line;
	std::istringstream iss(msg);
	std::getline(iss, line);
	std::istringstream lineStream(line);
	std::vector<std::string> parsedLine((std::istream_iterator<std::string>(lineStream)), std::istream_iterator<std::string>());

	if (parsedLine.size() != 3 || parsedLine[2] != "HTTP/1.1")
		return -1;
	return 0;
}

void	Request::parseRequest(std::string &msg)
{
	if (checkRequest(msg) == -1)
	{
		is_bad_request = true;
		return ;
	}
	 // Use a stringstream to parse the HTTP request
    std::istringstream request_stream(msg);
    std::string line;

	// Parse the request line
    if (std::getline(request_stream, line)) {
        std::istringstream request_line(line);
        request_line >> _method >> _filename >> _httpVersion;
    }

	// Parse headers
    while (std::getline(request_stream, line) && !line.empty()) {
        if (line == "\r") {
            break; // End of headers
        }
        std::string::size_type pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2);
            _headers[key] = value;
        }
    }

	// Parse body
	if (_method == "POST")
	{
		std::string body;
		while (std::getline(request_stream, line)) {
			body += line + "\n";
		}
		_body = body;
	}
	initialize();
	
}
