/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:46:56 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/26 13:33:14 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include "Server.hpp"
#include <cstring>
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

class Socket {
	public:
		Socket();
		~Socket();
		Socket(std::vector<Server*>);
		void launchSocket(const Server&);
		int const &getServerFD() const;
		std::vector<int> const &getClientFD() const;
	private:
		std::vector<Server> _servers;
		int _server_fd;
		std::vector<int> _client_fds;
		std::string			 _rawRequest;
};