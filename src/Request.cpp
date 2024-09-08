/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luynagda <luynagda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:08:55 by lunagda           #+#    #+#             */
/*   Updated: 2024/09/08 15:45:15 by luynagda         ###   ########.fr       */
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
    _errorPages.clear();
    _content.clear();
    _body.clear();
    _rootPath.clear();
    _uploadDir.clear();
	_prefix.clear();
	_redirectCode = 0;
	_redirectPath.clear();
    _cgi.clear();
	_is_redirect = false;
    timeout = false;
    _is_cgi = false;
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
	if (_headers["Content-Type"].empty())
	{
		std::string extension = _path.rfind(".") != std::string::npos ? _path.substr(_path.rfind(".") + 1) : ".txt";
		_headers["Content-Type"] = _mimeTypes[extension];
	}
	_headers["Server"] = "Webserv/1.0";
	_headers["Connection"] = "Keep-Alive";
	_headers["Cache-Control"] = "no-cache, private";
}

const std::string &Request::getErrorPage(int ErrorCode) const
{
	if (_errorPages.find(ErrorCode) != _errorPages.end())
		return _errorPages.find(ErrorCode)->second;
	else
		return _errorPages.find(404)->second;
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
        getFileContent(getErrorPage(404), server);
    }
}

void	Request::getFileContent(const std::string &filename, const Server &server)
{
	struct stat fileStat;

	if (access(filename.c_str(), F_OK) == -1)
	{
		_headers["Status"] = "404 Not Found";
		_headers["Content-Type"] = "text/html";
		getFileContent(getErrorPage(404), server);
	}
	else if (access(filename.c_str(), R_OK) == -1)
	{
		_headers["Status"] = "403 Forbidden";
		_headers["Content-Type"] = "text/html";
		getFileContent(getErrorPage(403), server);
	}
	else if (stat(filename.c_str(), &fileStat) == 0 && S_ISREG(fileStat.st_mode))
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
		getFileContent(getErrorPage(404), server);
	}
}

void	Request::fillContent(const std::string &status, const std::string &contenttype, const std::string &filename, const Server &server)
{
	_headers["Status"] = status;
	_headers["Content-Type"] = contenttype;
	getFileContent(filename, server);
}

void    Request::initializeVariables(const Server &server)
{
	Location *location;
	std::vector<std::string> indexes;
	std::vector<std::string> allow_methods;
    std::vector<Location *> locations = server.getLocations();
    std::string extension = _path.rfind(".") != std::string::npos ? _path.substr(_path.rfind(".")) : "";

    _rootPath = server.getRootPath();
    _uploadDir = server.getUploadDir();
	_errorPages = server.getErrorPages();
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
                _cgi = (*it)->getCGI();
                if (!_cgi.empty() && !_cgi[extension].empty())
                    _is_cgi = true;
				if ((*it)->getErrorPageSet())
					_errorPages = (*it)->getErrorPages();
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
	if (!location->getUploadDir().empty())
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

void    Request::handleCgiRequest(const Server &server)
{
    char    buffer[2000];
    char    cwd[PATH_MAX];
    
    if (getcwd(cwd, PATH_MAX) == NULL)
        throw std::runtime_error("Error: getcwd failed");
    std::string filename = std::string(cwd) + "/" + _rootPath + _path;
    std::string extension = _path.rfind(".") != std::string::npos ? _path.substr(_path.rfind(".")) : "";
    char    *list_buffer[100] = {const_cast<char *>((_cgi[extension]).c_str()), const_cast<char *>((filename).c_str()), NULL};
    int     pipefd[2];
    pid_t   pid;
    std::ifstream file((_rootPath + _path).c_str());
    struct stat fileStat;

    stat((_rootPath + _path).c_str(), &fileStat);
    if (file && file.is_open() && !S_ISDIR(fileStat.st_mode))
    {
        if (pipe(pipefd) == -1)
            throw std::runtime_error("Error: pipe failed");
        if ((pid = fork()) == -1)
            throw std::runtime_error("Error: fork failed");
        if (pid == 0)
        {
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
            if (execve((_cgi.begin()->second).c_str(), list_buffer, NULL) == -1)
                throw std::runtime_error("Error: execve failed");
        }
        else
        {
            close(pipefd[1]);
            ssize_t len_read;
            len_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
            if (len_read == -1)
                throw std::runtime_error("read issue");
            buffer[len_read] = '\0';
            close(pipefd[0]);
            waitpid(pid, NULL, 0);
            _headers["Content-Type"] = "text/html";
            _headers["Status"] = "200 OK";
            _content = buffer;
        }
    }
    else
    {
        _headers["Status"] = "404 Not Found";
        getFileContent(getErrorPage(404), server);
    }
}

void	Request::handleGetRequest(const Server &server)
{
	std::string contenttype;

    if (_is_cgi)
        handleCgiRequest(server);
	else if (_path != "/" && _path[_path.size() - 1] == '/' && _auto_index)
	{
		_headers["Status"] = "200 OK";
		_headers["Content-Type"] = "text/html";
		getDirectoryListing(_rootPath + _path, server);
	}
	else
	{	
		if (_filename.find_last_of(".") != std::string::npos)
			contenttype = _mimeTypes[_filename.substr(_filename.find_last_of("."))];
		else
			contenttype = "text/html";
		fillContent("200 OK", contenttype, _rootPath + _filename, server);
	}
}

void	Request::handlePostRequest(const Server &server)
{
	if (_headers["boundary"].empty())
		fillContent("400 Bad Request", "text/html", getErrorPage(400), server);
	std::string boundary = _headers["boundary"];
	_headers["boundary"].erase();
	
	// Find the filename
	size_t filenameStart = _body.find("filename=\"");
	if (filenameStart == std::string::npos)
		fillContent("400 Bad Request", "text/html", getErrorPage(400), server);
	filenameStart += 10;
	size_t filenameEnd = _body.find("\"", filenameStart);
	std::string filename = _body.substr(filenameStart, filenameEnd - filenameStart);

	// Find the start of the file content
	size_t fileContentStart = _body.find(CRLF CRLF, filenameEnd) + 4;
	if (fileContentStart == std::string::npos)
		fillContent("400 Bad Request", "text/html", getErrorPage(400), server);

	// Find the end of the file content
    std::string::size_type fileContentEnd = _body.find("--" + boundary, fileContentStart);
	if (fileContentEnd == std::string::npos)
		fillContent("400 Bad Request", "text/html", getErrorPage(400), server);
    else
    {
        // Remove trailing dashes
        while (_body[fileContentEnd - 1] == '-' && fileContentEnd > fileContentStart)
            fileContentEnd--;
        // Remove trailing CRLF
        fileContentEnd -= 2;
    }
	// Extract the file content
	std::string fileContent = _body.substr(fileContentStart, fileContentEnd - fileContentStart);
	if (fileContent.size() > server.getMaxBodySize())
	{
		fillContent("413 Payload Too Large", "text/html", getErrorPage(413), server);
		_body.clear();
	}
	// Write the file content to a file
	std::ofstream file((_rootPath + _uploadDir + filename).c_str(), std::ios::binary);
	if (file && file.is_open())
	{
		file.write(fileContent.c_str(), fileContent.size());
		file.close();
		_headers["Location"] = _uploadDir + filename;
		fillContent("201 Created", "text/html", _rootPath + _filename, server);
	}
	else
		fillContent("502 Bad Gateway", "text/html", getErrorPage(502), server);
}

void Request::handleDeleteRequest(const Server &server)
{
	if (access((_rootPath + _path).c_str(), F_OK) == -1)
		fillContent("404 Not Found", "text/html", getErrorPage(404), server);
	else if (access((_rootPath + _path).c_str(), R_OK) == -1)
		fillContent("403 Forbidden", "text/html", getErrorPage(403), server);
	else if (remove((_rootPath + _path).c_str()) == 0)
		fillContent("204 No Content", "text/html", getErrorPage(204), server);
	else
		fillContent("404 Not Found", "text/html", getErrorPage(404), server);
}

void	Request::onMessageReceived(int client_fd, const Server &server)
{
    if (timeout)
        fillContent("408 Request Timeout", "text/html", getErrorPage(408), server);
    else if (is_bad_request)
		fillContent("400 Bad Request", "text/html", getErrorPage(400), server);
	else
	{
        initializeVariables(server);
		if (_is_redirect)
		{
			_headers.clear();
			if (_redirectCode == 301)
				_headers["Status"] = ToString(_redirectCode) + " " + "Moved Permanently";
			else
				_headers["Status"] = ToString(_redirectCode) + " " + "Permanent Redirect";
			_headers["Location"] = _redirectPath;
			_headers["Content-Length"] = "0";
		}
        else if (!_allowed_method)
			fillContent("405 Method Not Allowed", "text/html", getErrorPage(405), server);
        else if (_method == "GET")
			handleGetRequest(server);
        else if (_method == "POST")
		{
            handlePostRequest(server);
		}
        else if (_method == "DELETE")
			handleDeleteRequest(server);
        else
			fillContent("404 Not Found", "text/html", getErrorPage(404), server);
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
    if (msg == "timeout" || checkRequest(msg) == -1 || msg == "payload")
    {
        Server server;
        _errorPages = server.getErrorPages();
        _method = "GET";
        _httpVersion = "HTTP/1.1";
        if (msg == "timeout")
            timeout = true;
        else
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
        {
			_headers["boundary"] = _body.substr(boundaryPos);
            _headers["boundary"] = _headers["boundary"].substr(0, _headers["boundary"].find(CRLF));
        }
	}
	initialize();
}
