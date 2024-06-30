/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:46:21 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/25 17:31:22 by ggalon           ###   ########.fr       */

/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

Socket::Socket()
{
    _rawRequest.clear();
    _server_map.clear();
}

Socket::~Socket()
{
    for (size_t i = 0; i < _server_fds.size(); i++)
    {
        close(_server_fds[i]);
    }
}

Socket::Socket(std::vector<Server*> serverList)
{
	for (size_t i = 0; i < serverList.size(); i++)
	{
		std::cout << "Listening on: " << (*serverList[i]).getPort() << std::endl;
		launchSocket(serverList[i]);
	}

    eventLoop();
}

std::vector<int> const &Socket::getServerFD() const
{
    return _server_fds;
}

std::vector<int> const &Socket::getClientFD() const
{
	return _client_fds;
}

void Socket::launchSocket(Server *server) {
    int server_fd;

    // Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        throw std::runtime_error("Error: socket creation failed");
    }

    // Set the socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        throw std::runtime_error("Error: setsockopt failed");
    }

    // Set the socket to non-blocking mode
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    // Bind the socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server->getPort());
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Error: bind failed");
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        throw std::runtime_error("Error: listen failed");
    }

    _server_fds.push_back(server_fd);
    _server_map[server_fd] = server; // Map server file descriptor to Server object
}

void Socket::eventLoop() {
    fd_set read_fds;
    int max_sd;

    while (true) {
        FD_ZERO(&read_fds);
        max_sd = 0;

        // Add server sockets to set
        for (size_t i = 0; i < _server_fds.size(); i++) {
            FD_SET(_server_fds[i], &read_fds);
            if (_server_fds[i] > max_sd) {
                max_sd = _server_fds[i];
            }
        }

        // Add client sockets to set
        for (std::vector<int>::iterator it = _client_fds.begin(); it != _client_fds.end(); ++it) {
            int client_fd = *it;
            FD_SET(client_fd, &read_fds);
            if (client_fd > max_sd) {
                max_sd = client_fd;
            }
        }

        struct timeval timeout;
        timeout.tv_sec = 5; // 5 seconds timeout
        timeout.tv_usec = 0; // 0 microseconds

        int activity = select(max_sd + 1, &read_fds, NULL, NULL, &timeout);

        if (activity < 0) {
            throw std::runtime_error("Error: select failed");
        } else if (activity == 0) {
            for (std::vector<int>::iterator it = _client_fds.begin(); it != _client_fds.end(); ++it) {
                Request req;
                _rawRequest.clear();
                _rawRequest = "timeout";
                req.parseRequest(_rawRequest);
                req.onMessageReceived(*it, *_client_map[*it]); // Identify the server for this client
                continue;
            }
        }

        // Check for new incoming connections
        for (size_t i = 0; i < _server_fds.size(); i++) {
            if (FD_ISSET(_server_fds[i], &read_fds)) {
                int new_socket;
                struct sockaddr_in address;
                socklen_t addrlen = sizeof(address);

                if ((new_socket = accept(_server_fds[i], (struct sockaddr *)&address, &addrlen)) < 0) {
                    throw std::runtime_error("Error: accept failed");
                }

                fcntl(new_socket, F_SETFL, O_NONBLOCK); // Set the new socket to non-blocking mode
                _client_fds.push_back(new_socket);
                _client_map[new_socket] = _server_map[_server_fds[i]]; // Map client file descriptor to corresponding server
            }
        }

        // Check for I/O operations on existing clients
        for (std::vector<int>::iterator it = _client_fds.begin(); it != _client_fds.end();) {
            int client_fd = *it;
            char buffer[3000];
            int total_read = 0;
            bool read_done = false;
            int tries = 0;

            memset(buffer, 0, 3000);
            if (FD_ISSET(client_fd, &read_fds)) {
                while (!read_done) {
                    if (tries > 5)
                        break;
                    int valread = recv(client_fd, buffer, sizeof(buffer), 0);
                    if (valread <= 0) {
                        // Connection closed by the client
                        close(client_fd);
                        it = _client_fds.erase(it);
                        _client_map.erase(client_fd); // Remove from client map
                        break;
                    }
                    total_read += valread;
                    _rawRequest.append(buffer, valread);
                    buffer[valread] = '\0';
                    if (valread > 0) {
                        tries++;
                        if (buffer[valread - 1] == '\n' && buffer[valread - 2] == '\r') {
                            read_done = true;
                            // std::cout << "Request received: [" + _rawRequest +"]" << std::endl;
                        }
                    }
                }
                if (read_done) {
                    Request req;
                    req.parseRequest(_rawRequest);
                    req.onMessageReceived(client_fd, *_client_map[client_fd]); // Identify the server for this client
                    _rawRequest.clear();
                }
                else if (tries > 5) {
                    Request req;
                    _rawRequest.clear();
                    _rawRequest = "timeout";
                    req.parseRequest(_rawRequest);
                    req.onMessageReceived(client_fd, *_client_map[client_fd]); // Identify the server for this client
                }
            } else {
                ++it;
            }
        }
    }
}