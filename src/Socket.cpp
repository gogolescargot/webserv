/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:46:21 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/25 17:07:01 by ggalon           ###   ########.fr       */
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

std::vector<int> const &Socket::getClientFD() const
{
	return _client_fds;
}

void Socket::launchSocket(Server server)
{
 std::vector<Location *> locations = server.getLocations();
    std::cout << locations.front()->getPath() << std::endl;

    // Create a socket
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1) {
        std::cerr << "Error: socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set the socket options
    int opt = 1;
    if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Error: setsockopt failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set the socket to non-blocking mode
    fcntl(_server_fd, F_SETFL, O_NONBLOCK);

    // Bind the socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server.getPort());
    if (bind(_server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error: bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(_server_fd, 3) < 0) {
        std::cerr << "Error: listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set up the select call
    fd_set read_fds;
    int max_sd = _server_fd;

    while (true)
	{
        FD_ZERO(&read_fds);
        FD_SET(_server_fd, &read_fds);

        for (std::vector<int>::iterator it = _client_fds.begin(); it != _client_fds.end(); ++it) {
            int client_fd = *it;
            FD_SET(client_fd, &read_fds);
            if (client_fd > max_sd) {
                max_sd = client_fd;
            }
        }

        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

        if (activity < 0) {
            std::cerr << "Error: select failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Check for new incoming connections
        if (FD_ISSET(_server_fd, &read_fds)) {
            int new_socket;
            struct sockaddr_in address;
            socklen_t addrlen = sizeof(address);

            if ((new_socket = accept(_server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
                std::cerr << "Error: accept failed" << std::endl;
                exit(EXIT_FAILURE);
            }

            fcntl(new_socket, F_SETFL, O_NONBLOCK); // Set the new socket to non-blocking mode
            _client_fds.push_back(new_socket);
        }

        // Check for I/O operations on existing clients
        for (std::vector<int>::iterator it = _client_fds.begin(); it != _client_fds.end(); ) {
            int client_fd = *it;

            if (FD_ISSET(client_fd, &read_fds)) {
                char buffer[1024] = {0};
                int valread = recv(client_fd, buffer, sizeof(buffer), 0);

                if (valread == 0) {
                    // Connection closed by the client
                    close(client_fd);
                    it = _client_fds.erase(it);
                } else {
                    Request req;
                    std::string tmp(buffer, valread);
                    req.parseRequest(tmp);
                    req.onMessageReceived(client_fd, server);
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    close(_server_fd);
}
