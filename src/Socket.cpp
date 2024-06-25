/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:46:21 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/25 16:09:24 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Socket::Socket()
{
}

Socket::~Socket()
{
}

Socket::Socket(std::map<std::string, Server> serverList)
{
	for (std::map<std::string, Server>::iterator it = serverList.begin(); it != serverList.end(); it++)
	{
		std::cout << "Listening on: " << it->second.getPort() << std::endl;
		launchSocket(it->second);
	}
}

int const &Socket::getServerFD() const
{
	return _server_fd;
}

int const &Socket::getClientFD() const
{
	return _client_fd;
}

void Socket::launchSocket(Server server)
{
	std::vector<Location *> locations = server.getLocations();

	//std::cout << locations.front()->getPath() << std::endl;
	// Create a socket
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1)
	{
		std::cerr << "Error: socket creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Set the socket options
	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		std::cerr << "Error: setsockopt failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Bind the socket
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if (bind(_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Error: bind failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	if (listen(_server_fd, 3) < 0)
	{
		std::cerr << "Error: listen failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Accept incoming connections
	int addrlen = sizeof(address);
	if ((_client_fd = accept(_server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		std::cerr << "Error: accept failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Read the request from the client
	char buffer[1024] = { 0 }; 
	recv(_client_fd, buffer, sizeof(buffer), 0); 
	Request req;
	std::string tmp(buffer);
	// std::cout << tmp << std::endl;
	req.parseRequest(tmp);
	req.onMessageReceived(_client_fd);
	close(_server_fd);
	}