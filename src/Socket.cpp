/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:46:21 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/24 13:28:21 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket()
{
}

Socket::~Socket()
{
}

int const &Socket::getServerFD() const
{
	return _server_fd;
}

int const &Socket::getClientFD() const
{
	return _client_fd;
}

void Socket::launchSocket()
{
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
	address.sin_port = htons(8080);
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
	while (1)
	{
		if ((_client_fd = accept(_server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		{
			std::cerr << "Error: accept failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		// Read the request from the client
		char buffer[1024] = { 0 }; 
		recv(_client_fd, buffer, sizeof(buffer), 0); 
		std::cout << "Message from client: " << buffer << std::endl; 
	}
	close(_server_fd);
}