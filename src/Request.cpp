/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:08:55 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/28 15:22:58 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
	is_bad_request = false;
	_allowed_method = false;
    _auto_index = false;
    _method.clear();
    _filename.clear();
    _path.clear();
    _httpVersion.clear();
    _headers.clear();
    _mimeTypes.clear();
    _errorCodes.clear();
    _content.clear();
    _body.clear();
    _rootPath.clear();
    _uploadDir.clear();
	_prefix.clear();
	_redirectCode = 0;
	_redirectPath.clear();
	_is_redirect = false;
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
	_errorCodes[301] = "Moved Permanently";
	_errorCodes[308] = "Permanent Redirect";
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
		std::string extension = _path.rfind(".") != std::string::npos ? _path.substr(_path.rfind(".") + 1) : ".txt";
		_headers["Content-Type"] = _mimeTypes[extension];
	}
	_headers["Server"] = "Webserv/1.0";
	_headers["Connection"] = "Keep-Alive";
	_headers["Cache-Control"] = "no-cache, private";
}

void    Request::getDirectoryListing(const std::string &path, const Server &server)
{
    DIR *dir;

    if ((dir = opendir(path.c_str())) != NULL)
    {
        std::string content = "<html><head><title>Index of " + _path + "</title></head><body><h1>Index of " + _path + "</h1><hr><pre>";
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL)
            content += "<a href=\"" + _path + ent->d_name + "\">" + ent->d_name + "</a><br>";
        closedir(dir);
        content += "</pre><hr></body></html>";
        _content = content;
        _headers["Content-Length"] = ToString(_content.size());
    }
    else
    {
        _headers["Status"] = "404 Not Found";
        getFileContent(server.getErrorPage(404), server);
    }
}

void	Request::getFileContent(const std::string &filename, const Server &server)
{
	struct stat fileStat;
	if (stat(filename.c_str(), &fileStat) == 0 && S_ISREG(fileStat.st_mode))
	{
		std::ifstream file(filename.c_str());
		if (file.good())
		{
			std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			_content = str;
			_headers["Content-Length"] = ToString(_content.size());
		}
	}
	else
	{
		_headers["Status"] = "404 Not Found";
		getFileContent(server.getErrorPage(404), server);
	}
}

void    Request::initializeVariables(const Server &server)
{
	Location *location;
	std::vector<std::string> indexes;
	std::vector<std::string> allow_methods;
    std::vector<Location *> locations = server.getLocations();

    _rootPath = server.getRootPath();
    _uploadDir = server.getUploadDir();
	for (std::vector<Location *>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (startsWith(_path, (*it)->getPath()))
		{
			if (_prefix.size() < (*it)->getPath().size())
			{
				if ((*it)->getRedirectCode() != 0)
				{
					_is_redirect = true;
					_redirectPath = (*it)->getRedirectPath();
					_redirectCode = (*it)->getRedirectCode();
				}
				_prefix = (*it)->getPath();
				location = *it;
			}
		}
	}
	if (_prefix.empty())
		_prefix = "/";
	_filename = _path;
	indexes = location->getIndexes();
	allow_methods = location->getAllowMethods();
	_auto_index = location->getAutoIndex();
	if (location->getUploadDir() != "")
		_uploadDir = location->getUploadDir();
	for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
	{
		if (std::ifstream((_rootPath + *it).c_str()) && _path == _prefix)
		{
			_filename = *it;
			break ;
		}
	}
	for (size_t i = 0; i < allow_methods.size(); i++)
	{
		if (allow_methods[i] == _method && (_method == "GET" || _method == "POST" || _method == "DELETE"))
		{
			_allowed_method = true;
			break ;
		}
	}
}

void	Request::onMessageReceived(int client_fd, const Server &server)
{
	if (is_bad_request)
	{
		_headers["Status"] = "400 Bad Request";
		_headers["Content-Type"] = "text/html";
		getFileContent(server.getErrorPage(400), server);
	}
	else
	{
        initializeVariables(server);
		if (_is_redirect)
		{
			_headers.clear();
			_headers["Status"] = ToString(_redirectCode) + " " + _errorCodes[_redirectCode];
			_headers["Location"] = _redirectPath;
			_headers["Content-Length"] = "0";
		}
        else if (!_allowed_method)
        {
            _headers["Status"] = "405 Method Not Allowed";
            _headers["Content-Type"] = "text/html";
            getFileContent(server.getErrorPage(405), server);
        }
        else if (_method == "GET" && _path != "/" && _path[_path.size() - 1] == '/' && _auto_index)
        {
            _headers["Status"] = "200 OK";
            _headers["Content-Type"] = "text/html";
            getDirectoryListing(_rootPath + _path, server);
        }
        else if (_method == "GET")
        {
            if (_filename.find_last_of(".") != std::string::npos)
                _headers["Content-Type"] = _mimeTypes[_filename.substr(_filename.find_last_of("."))];
            else
                _headers["Content-Type"] = "text/html";
            _headers["Status"] = "200 OK";
            getFileContent(_rootPath + _filename, server);
        }
        else if (_method == "POST")
        {
            if (_headers["boundary"].empty())
            {
                _headers["Status"] = "400 Bad Request";
                _headers["Content-Type"] = "text/html";
                getFileContent(server.getErrorPage(400), server);
            }
            std::string boundary = _headers["boundary"];
            _headers["boundary"].erase();
            
            // Find the filename
            size_t filenameStart = _body.find("filename=\"");
            if (filenameStart == std::string::npos)
            {
                _headers["Status"] = "400 Bad Request";
                _headers["Content-Type"] = "text/html";
                getFileContent(server.getErrorPage(400), server);
            }
            filenameStart += 10;
            size_t filenameEnd = _body.find("\"", filenameStart);
            std::string filename = _body.substr(filenameStart, filenameEnd - filenameStart);

            // Find the start of the file content
            size_t fileContentStart = _body.find(CRLF CRLF, filenameEnd) + 4;
            if (fileContentStart == std::string::npos)
            {
                _headers["Status"] = "400 Bad Request";
                _headers["Content-Type"] = "text/html";
                getFileContent(server.getErrorPage(400), server);
            }

            // Find the end of the file content
            size_t fileContentEnd = _body.find(boundary, fileContentStart) - 4;
            if (fileContentEnd == std::string::npos)
            {
                _headers["Status"] = "400 Bad Request";
                _headers["Content-Type"] = "text/html";
                getFileContent(server.getErrorPage(400), server);
            }

            // Extract the file content
            std::string fileContent = _body.substr(fileContentStart, fileContentEnd - fileContentStart);
            _body = fileContent;
            if (fileContent.size() > server.getMaxBodySize())
            {
                _headers["Status"] = "413 Payload Too Large";
                _headers["Content-Type"] = "text/html";
                _body.clear();
                getFileContent(server.getErrorPage(413), server);
            }
            // Write the file content to a file
            std::ofstream file((_rootPath + _uploadDir + filename).c_str(), std::ios::binary);
            if (file && file.is_open())
            {
                file.write(fileContent.c_str(), fileContent.size());
                file.close();
                _headers["Status"] = "201 Created";
                _headers["Content-Type"] = "text/html";
                _headers["Location"] = _uploadDir + filename;
                getFileContent(_rootPath + _filename, server);
            }
            else
            {
                _headers["Status"] = "502 Bad Gateway";
                _headers["Content-Type"] = "text/html";
                getFileContent(server.getErrorPage(502), server);
            }
        }
        else if (_method == "DELETE")
        {
            if (remove((_rootPath + _path).c_str()) == 0)
            {
                _headers["Status"] = "204 No Content";
                _headers["Content-Type"] = "text/html";
                getFileContent(server.getErrorPage(204), server);
            }
            else
            {
                _headers["Status"] = "404 Not Found";
                _headers["Content-Type"] = "text/html";
                getFileContent(server.getErrorPage(404), server);
            }
        }
        else
        {
            _headers["Status"] = "404 Not Found";
            _headers["Content-Type"] = "text/html";
            getFileContent(server.getErrorPage(404), server);
        }
	}
	
	std::string response = _httpVersion + " " + _headers["Status"] + CRLF;
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
		response += it->first + ": " + it->second + CRLF;
	response += CRLF + _content;
	if (_method == "POST" && !_body.empty())
		response += CRLF + _body;
	//std::cout << "Response: [" << response  << "]" << std::endl;
	send(client_fd, response.c_str(), response.size() + 1, 0);
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
        request_line >> _method >> _path >> _httpVersion;
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
		size_t boundaryPos = _headers["Content-Type"].find("boundary=");
		if (boundaryPos != std::string::npos)
			_headers["boundary"] = "--" + _body.substr(boundaryPos + 9);
	}
	initialize();
}
